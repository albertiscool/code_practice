#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    READY,
    RUNNING,
    BLOCKED
} TaskState_t;

typedef struct {
    const char *task_name;
    uint32_t priority;      // Higher number = Higher Priority (FreeRTOS rule)
    TaskState_t state;
} Mock_TCB_t;

// Define three tasks with different priorities aligned with your future Jetson Nano setup
Mock_TCB_t InputTask   = { .task_name = "Task_UART_RX_Tegrastats (High)", .priority = 3, .state = BLOCKED }; // Blocked waiting for data
Mock_TCB_t ControlTask = { .task_name = "Task_YOLO_Inference (Mid)",     .priority = 2, .state = READY   }; // Ready to compute
Mock_TCB_t IdleTask    = { .task_name = "Task_Idle_PowerSave (Low)",     .priority = 0, .state = READY   }; // System idle fallback

Mock_TCB_t *current_running_task = &IdleTask;

// --- Core Function: Preemptive Priority Scheduler ---
// This aligns with FreeRTOS's task selection algorithm (vTaskSwitchContext)
void vSelectHighestPriorityTask(void) {
    Mock_TCB_t *next_task = &IdleTask; // Default to lowest priority fallback

    // Array of all tasks in the system
    Mock_TCB_t *task_list[3] = { &InputTask, &ControlTask, &IdleTask };

    // Search for the HIGHEST priority task that is in the READY state
    for (int i = 0; i < 3; i++) {
        if (task_list[i]->state == READY) {
            if (task_list[i]->priority > next_task->priority) {
                next_task = task_list[i];
            }
        }
    }

    // Context Switch execution if a higher priority task demands the CPU
    if (current_running_task != next_task) {
        printf("\n>>> [PREEMPTION] Scheduler triggered context switch: %s yielded CPU to %s <<<\n", 
               current_running_task->task_name, next_task->task_name);
        
        // If the old task wasn't blocked, it goes back to being READY
        if (current_running_task->state == RUNNING) {
            current_running_task->state = READY;
        }
        
        current_running_task = next_task;
        current_running_task->state = RUNNING;
    }
}

int main() {
    printf("=================================================================\n");
    printf("--- FreeRTOS Preemptive Priority Simulation Initialized ---\n");
    printf("=================================================================\n\n");

    // Timeline 1: Standard state. High priority task is blocked, Mid priority runs.
    printf("[Timeline: Tick 1] YOLO Inference task is calculating frames...\n");
    vSelectHighestPriorityTask();
    printf("Currently occupying CPU: %s\n", current_running_task->task_name);

    // Timeline 2: External hardware interrupt arrives! 
    // Data arrives on UART (tegrastats packet), forcing the High Priority task to wake up (Blocked -> Ready)
    printf("\n[Timeline: Tick 2] ====== Hardware Interrupt: Tegrastats Data Arrived! ======\n");
    printf("[ISR Action] Waking up UART Task, moving it from BLOCKED to READY status.\n");
    InputTask.state = READY; // ISR changes the state flag

    // The scheduler runs immediately upon exiting the ISR or at the next SysTick
    vSelectHighestPriorityTask();
    printf("Currently occupying CPU: %s\n", current_running_task->task_name);

    // Timeline 3: High priority task finishes processing the packet and goes back to block (e.g., vTaskDelay)
    printf("\n[Timeline: Tick 3] UART Task finishes reading data, calling vTaskDelay()...\n");
    InputTask.state = BLOCKED;

    // Scheduler recalculates who should get the CPU now
    vSelectHighestPriorityTask();
    printf("Currently occupying CPU: %s\n", current_running_task->task_name);

    return 0;
}