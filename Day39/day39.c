#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

// Simulated Hardware Register state
typedef struct {
    int  packet_id;
    char raw_payload[32];
    bool pending_interrupt;
} Mock_Hardware_t;

static Mock_Hardware_t g_hw_sensor = {
    .packet_id = 101,
    .raw_payload = "TEMP_SENSOR_RAW_DATA_0x7F",
    .pending_interrupt = false
};

// --- Mocking Linux Kernel Workqueue Structure ---
typedef struct {
    void (*work_func)(void *data); // The deferred function pointer
    void *data;                    // Data passed to the bottom half
    bool is_scheduled;
} Mock_Workqueue_t;

// Deferred task handler (Bottom Half / Workqueue Function)
void my_driver_bottom_half_worker(void *data) {
    Mock_Hardware_t *hw_data = (Mock_Hardware_t *)data;
    
    printf("\n  [Kernel Workqueue: Bottom Half] Worker Thread Woken Up!\n");
    printf("  ↳ Executing heavy data parsing in background: \"%s\"\n", hw_data->raw_payload);
    printf("  ↳ Parsing finished for Packet ID: %d. System fully synchronized.\n", hw_data->packet_id);
}

static Mock_Workqueue_t g_my_work = {
    .work_func = my_driver_bottom_half_worker,
    .data = &g_hw_sensor,
    .is_scheduled = false
};

// --- Linux Kernel Interrupt Handler (Top Half / ISR) ---
void my_driver_top_half_isr(void) {
    printf("[Kernel Interrupt: Top Half / ISR] Hardware Interrupt Fired!\n");
    printf("  ↳ Quickly acknowledging interrupt and clearing hardware flags...\n");
    g_hw_sensor.pending_interrupt = false; // Clear HW flag instantly

    // Schedule the heavy work to be done later by the Kernel Workqueue
    if (!g_my_work.is_scheduled) {
        printf("  ↳ Deferring heavy processing to Bottom Half (Workqueue)...\n");
        g_my_work.is_scheduled = true;
    }
    printf("[Kernel Interrupt: Top Half / ISR] Exiting ISR in microsecond!\n");
}

int main() {
    printf("=================================================================\n");
    printf("--- Day 39: Linux Kernel Interrupt Top/Bottom Half Simulation ---\n");
    printf("=================================================================\n\n");

    // 1. Hardware triggers an interrupt event
    printf("--- Event: Physical Sensor Hardware Sends Data ---\n");
    g_hw_sensor.pending_interrupt = true;

    // 2. CPU immediately jumps to execution of Top Half ISR
    my_driver_top_half_isr();

    // 3. Main Kernel Thread simulates picking up scheduled work from Workqueue
    printf("\n--- Main Kernel Event Loop Running --- \n");
    if (g_my_work.is_scheduled) {
        usleep(100000); // Simulate brief kernel schedule delay (100ms)
        
        // Execute deferred work
        g_my_work.work_func(g_my_work.data);
        g_my_work.is_scheduled = false;
    }

    printf("\n[Main System] Day 39 Simulation Complete.\n");
    return 0;
}