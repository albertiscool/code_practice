#include <stdio.h>
#include <stdbool.h>

#define QUEUE_SIZE 5 // Total array size. Effective capacity will be QUEUE_SIZE - 1 (4 elements)

typedef struct {
    int data[QUEUE_SIZE];
    int head;
    int tail;
} CircularQueue_t;

// Initialize the queue
void init_queue(CircularQueue_t *q) {
    q->head = 0;
    q->tail = 0;
}

// Check if the queue is full
bool is_full(CircularQueue_t *q) {
    return ((q->tail + 1) % QUEUE_SIZE) == q->head;
}

// Check if the queue is empty
bool is_empty(CircularQueue_t *q) {
    return q->head == q->tail;
}

// Enqueue: Insert an element to the tail
bool enqueue(CircularQueue_t *q, int value) {
    if (is_full(q)) {
        printf("[Error] Queue is FULL! Cannot enqueue %d\n", value);
        return false;
    }
    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % QUEUE_SIZE; // Wrap around using modulo
    printf("[Enqueue] Success: %d | Head index: %d, Tail index: %d\n", value, q->head, q->tail);
    return true;
}

// Dequeue: Remove and return an element from the head
bool dequeue(CircularQueue_t *q, int *p_value) {
    if (is_empty(q)) {
        printf("[Error] Queue is EMPTY! Cannot dequeue\n");
        return false;
    }
    *p_value = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE; // Wrap around using modulo
    printf("[Dequeue] Success: %d | Head index: %d, Tail index: %d\n", *p_value, q->head, q->tail);
    return true;
}

int main() {
    CircularQueue_t my_queue;
    init_queue(&my_queue);

    printf("--- Part 1: Filling Up the Queue ---\n");
    enqueue(&my_queue, 10);
    enqueue(&my_queue, 20);
    enqueue(&my_queue, 30);
    enqueue(&my_queue, 40);
    enqueue(&my_queue, 50); // This should fail because effective capacity is 4

    printf("\n--- Part 2: Consuming and Reusing the Space ---\n");
    int popped_val;
    dequeue(&my_queue, &popped_val); // Frees up index 0
    dequeue(&my_queue, &popped_val); // Frees up index 1

    printf("\n--- Part 3: Verifying the Circular \"Wrap Around\" ---\n");
    // These should wrap around to the beginning of the array seamlessly
    enqueue(&my_queue, 60); 
    enqueue(&my_queue, 70); 

    return 0;
}