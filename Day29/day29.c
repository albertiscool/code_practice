#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Define Bit Masks for our system events (Bits 0 to 2)
#define EVENT_BIT_CAMERA_READY   (1 << 0) // Bit 0: Camera has finished capturing a frame
#define EVENT_BIT_THERMAL_SAFE   (1 << 1) // Bit 1: Jetson Nano GPU temperature is below 80C
#define EVENT_BIT_BUFFER_FREE    (1 << 2) // Bit 2: Memory cache has been cleared

typedef enum { READY, RUNNING, BLOCKED } TaskState_t;

typedef struct {
    const char *task_name;
    TaskState_t state;
} Mock_TCB_t;

// The AI task that needs ALL conditions met before execution
Mock_TCB_t TaskYOLO = { .task_name = "Task_YOLO_Inference", .state = BLOCKED };

// Simulated Event Group (A single 24-bit/32-bit register)
volatile uint32_t g_system_event_group = 0;

// --- Core Function: Wait for Multiple Event Bits (AND Logic) ---
// Simulates xEventGroupWaitBits()
void Mock_xEventGroupWaitBits(uint32_t bits_to_wait_for, bool wait_for_all) {
    printf("\n[%s] Checking system conditions...\n", TaskYOLO.task_name);
    
    // Perform bitwise AND to see if all demanded bits are set
    uint32_t current_status = (g_system_event_group & bits_to_wait_for);
    bool conditions_met = false;

    if (wait_for_all) {
        conditions_met = (current_status == bits_to_wait_for);
    } else {
        conditions_met = (current_status > 0);
    }

    if (conditions_met) {
        TaskYOLO.state = READY;
        printf("  ↳ [STATUS: SUCCESS] All %d conditions met! %s is now READY to compute.\n", 
               __builtin_popcount(bits_to_wait_for), TaskYOLO.task_name);
    } else {
        TaskYOLO.state = BLOCKED;
        printf("  ↳ [STATUS: FAILED] Missing conditions (Current: 0x%02X, Required: 0x%02X). %s remains BLOCKED.\n", 
               g_system_event_group, bits_to_wait_for, TaskYOLO.task_name);
    }
}

// --- Core Function: Set Event Bits (xEventGroupSetBits) ---
// This would typically be called by peripheral ISRs or separate driver tasks
void Mock_xEventGroupSetBits(uint32_t bits_to_set) {
    printf("\n[Hardware / Driver Update] Triggering Event Bits: 0x%02X...\n", bits_to_set);
    
    // Set the bits in our simulated event register
    g_system_event_group |= bits_to_set;

    // Evaluate if this update wakes up our waiting AI task
    uint32_t required_bits = (EVENT_BIT_CAMERA_READY | EVENT_BIT_THERMAL_SAFE);
    Mock_xEventGroupWaitBits(required_bits, true); // Wait for both
}

int main() {
    printf("--- FreeRTOS Event Group Synchronization Simulation ---\n");
    printf("Target Conditions Required: Camera Ready (0x01) AND Thermal Safe (0x02)\n\n");

    // 1. Initial State: Both conditions are 0, AI task tries to wait
    uint32_t required_bits = (EVENT_BIT_CAMERA_READY | EVENT_BIT_THERMAL_SAFE);
    Mock_xEventGroupWaitBits(required_bits, true);

    // 2. Camera driver fires and sets the camera ready bit
    Mock_xEventGroupSetBits(EVENT_BIT_CAMERA_READY);

    // 3. Thermal controller finishes checking tegrastats, GPU is safe, sets the thermal safe bit
    Mock_xEventGroupSetBits(EVENT_BIT_THERMAL_SAFE);

    return 0;
}