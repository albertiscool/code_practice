#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define QUEUE_SIZE 2

// --- Mocking FreeRTOS Queue Structure ---
typedef struct {
    int storage[QUEUE_SIZE];
    int head; // Point to write index
    int tail; // Point to read index
    int count; // Current number of items inside
} Mock_Queue_t;

typedef enum { READY, BLOCKED } TaskState_t;
typedef struct {
    const char *task_name;
    TaskState_t state;
} Mock_TCB_t;

// Define two tasks interacting with the Queue
Mock_TCB_t ProducerTask = { .task_name = "Task_Tegrastats_Producer", .state = READY };
Mock_TCB_t ConsumerTask = { .task_name = "Task_YOLO_Consumer",       .state = READY };

// Initialize an empty Queue
Mock_Queue_t g_telemetry_queue = { .head = 0, .tail = 0, .count = 0 };

// --- Core Function: Send Data to Queue (xQueueSend Mock) ---
bool Mock_xQueueSend(Mock_Queue_t *q, int value) {
    if (q->count >= QUEUE_SIZE) {
        printf("[Queue Status] QUEUE FULL! %s cannot write. Moving to BLOCKED state.\n", ProducerTask.task_name);
        ProducerTask.state = BLOCKED;
        return false;
    }

    // Write item into the ring buffer safely
    q->storage[q->head] = value;
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count++;
    
    printf("[xQueueSend] Successfully wrote %d. Queue count: %d/%d\n", value, q->count, QUEUE_SIZE);

    // If consumer was blocked waiting for data, wake it up!
    if (ConsumerTask.state == BLOCKED) {
        printf("  ↳ [Queue Trigger] Data available. Waking up %s back to READY!\n", ConsumerTask.task_name);
        ConsumerTask.state = READY;
    }
    return true;
}

// --- Core Function: Receive Data from Queue (xQueueReceive Mock) ---
bool Mock_xQueueReceive(Mock_Queue_t *q, int *p_rx_buffer) {
    if (q->count == 0) {
        printf("[Queue Status] QUEUE EMPTY! %s cannot read. Moving to BLOCKED state.\n", ConsumerTask.task_name);
        ConsumerTask.state = BLOCKED;
        return false;
    }

    // Read item from the ring buffer safely
    *p_rx_buffer = q->storage[q->tail];
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    q->count--;

    printf("[xQueueReceive] Successfully read %d. Queue count: %d/%d\n", *p_rx_buffer, q->count, QUEUE_SIZE);

    // If producer was blocked waiting for space, wake it up!
    if (ProducerTask.state == BLOCKED) {
        printf("  ↳ [Queue Trigger] Space freed up. Waking up %s back to READY!\n", ProducerTask.task_name);
        ProducerTask.state = READY;
    }
    return true;
}

int main() {
    printf("--- FreeRTOS Queue Synchronization Simulation ---\n\n");

    // Scenario 1: Producer fills up the queue
    printf("--- Scenario 1: Filling the Queue ---\n");
    Mock_xQueueSend(&g_telemetry_queue, 101); // 1st element (e.g. GPU Temp)
    Mock_xQueueSend(&g_telemetry_queue, 102); // 2nd element (Queue now full)
    Mock_xQueueSend(&g_telemetry_queue, 103); // Fails, blocks Producer Task

    // Scenario 2: Consumer reads from the queue, which wakes up the blocked Producer
    printf("\n--- Scenario 2: Consumer reading from a Full Queue ---\n");
    int rx_data = 0;
    Mock_xQueueReceive(&g_telemetry_queue, &rx_data); // Freeing 1 slot, wakes up Producer

    // Scenario 3: Reading until empty
    printf("\n--- Scenario 3: Draining the Queue ---\n");
    Mock_xQueueReceive(&g_telemetry_queue, &rx_data); 
    Mock_xQueueReceive(&g_telemetry_queue, &rx_data); // Fails, blocks Consumer Task

    return 0;
}