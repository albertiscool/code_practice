#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// --- Mocking FreeRTOS Task Control Block (TCB) ---
typedef struct {
    const char *task_name;
    uint32_t stack_pointer;    // Simulated saved CPU Stack Pointer (SP)
    uint32_t priority;         // Task Priority (Higher meaning more urgent)
    uint32_t simulated_stack[16]; // Each task has its own isolated memory space
} TCB_t;

// Define two distinct, independent tasks
TCB_t Task_A = { .task_name = "Task_AI_Inference", .priority = 2 };
TCB_t Task_B = { .task_name = "Task_Thermal_Ctrl", .priority = 1 };

// Global pointer to track which task is currently occupying the CPU
TCB_t *pxCurrentTCB = &Task_A;

// --- Core Function: The Core of RTOS Scheduler ---
// Simulates a Context Switch triggered by a hardware SysTick timer interrupt
void vTaskSwitchContext(void) {
    printf("\n[RTOS SysTick Interrupt Triggered] Performing Context Switch...\n");
    
    // 1. Save current state (Mocking backing up CPU registers into current stack)
    printf("  Saving Context: %s's registers safely pushed into its Stack.\n", pxCurrentTCB->task_name);
    pxCurrentTCB->stack_pointer = 0xDEADBEEF; // Simulated stack save address

    // 2. Scheduler Decision: Toggle between tasks for this basic demonstration
    if (pxCurrentTCB == &Task_A) {
        pxCurrentTCB = &Task_B;
    } else {
        pxCurrentTCB = &Task_A;
    }

    // 3. Restore next state (Pop registers from the new task's stack into CPU)
    printf("  Restoring Context: Popping %s's registers from Stack back to CPU!\n", pxCurrentTCB->task_name);
}

int main() {
    printf("==================================================\n");
    printf("--- Starting FreeRTOS Core Scheduler Simulation ---\n");
    printf("==================================================\n\n");

    // Simulate 4 system clock ticks to observe the multi-tasking timeline
    for (int system_tick = 1; system_tick <= 4; system_tick++) {
        printf("\n--- System Timeline: Time = %d ms ---\n", system_tick);
        printf("[CPU Active Task] Running: %s\n", pxCurrentTCB->task_name);
        
        // Every task runs its slice of application code here...
        if (pxCurrentTCB == &Task_A) {
            printf("  ↳ [Executing App Code] YOLO model processing frame data...\n");
        } else {
            printf("  ↳ [Executing App Code] Reading Jetson Nano fan PWM and core thermals...\n");
        }

        // Simulate a hardware SysTick slice firing to switch tasks
        vTaskSwitchContext();
    }

    printf("\n==================================================\n");
    printf("--- Scheduler Simulation Completed Successfully ---\n");
    printf("==================================================\n");
    return 0;
}