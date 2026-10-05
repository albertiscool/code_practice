#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum { READY, RUNNING, BLOCKED } TaskState_t;

typedef struct {
    const char *task_name;
    uint32_t base_priority;    // The task's original starting priority
    uint32_t current_priority; // Current priority (Might change due to inheritance!)
    TaskState_t state;
} Mock_TCB_t;

typedef struct {
    Mock_TCB_t *owner;        // Pointer to the TCB holding the Mutex (NULL if unlocked)
    bool is_locked;
} Mock_Mutex_t;

// Define our three iconic tasks
Mock_TCB_t TaskHigh = { .task_name = "Task_High (Urgent Video)", .base_priority = 3, .current_priority = 3, .state = READY };
Mock_TCB_t TaskMid  = { .task_name = "Task_Mid (Standard Temp)", .base_priority = 2, .current_priority = 2, .state = READY };
Mock_TCB_t TaskLow  = { .task_name = "Task_Low (Background Log)", .base_priority = 1, .current_priority = 1, .state = READY };

Mock_Mutex_t g_uart_mutex = { .owner = NULL, .is_locked = false };

// --- Core Function: Take Mutex with Priority Inheritance ---
void Mock_Mutex_Take(Mock_TCB_t *task, Mock_Mutex_t *mutex) {
    printf("[%s] Trying to ACQUIRE the Mutex...\n", task->task_name);

    if (!mutex->is_locked) {
        // Mutex is free! Successfully lock it.
        mutex->is_locked = true;
        mutex->owner = task;
        printf("  ↳ Success! %s now OWNERS the Mutex.\n", task->task_name);
    } else {
        // Mutex is occupied!
        printf("  ↳ Conflict! Mutex is held by %s.\n", mutex->owner->task_name);
        
        // --- PRIORITY INHERITANCE TRIGGER ---
        if (task->current_priority > mutex->owner->current_priority) {
            printf("  [RTOS Action] Priority Inheritance Active!\n");
            printf("  Boosting owner %s's priority from %d to %d to prevent inversion!\n", 
                   mutex->owner->task_name, mutex->owner->current_priority, task->current_priority);
            
            mutex->owner->current_priority = task->current_priority; // Inherit!
        }
        
        // Force the high priority task to block and wait
        task->state = BLOCKED;
        printf("  ↳ %s is now BLOCKED waiting for Mutex.\n", task->task_name);
    }
}

// --- Core Function: Release Mutex with Priority Restoration ---
void Mock_Mutex_Give(Mock_Mutex_t *mutex) {
    if (!mutex->is_locked) return;

    Mock_TCB_t *old_owner = mutex->owner;
    printf("\n[%s] RELEASING the Mutex...\n", old_owner->task_name);

    // Restore original base priority
    if (old_owner->current_priority != old_owner->base_priority) {
        printf("  [RTOS Action] Restoring %s's priority back to original: %d\n", 
               old_owner->task_name, old_owner->base_priority);
        old_owner->current_priority = old_owner->base_priority;
    }

    // Release ownership
    mutex->is_locked = false;
    mutex->owner = NULL;

    // Wake up the waiting High Priority Task
    if (TaskHigh.state == BLOCKED) {
        printf("  ↳ [Scheduler Trigger] Mutex free. Waking up %s!\n", TaskHigh.task_name);
        TaskHigh.state = READY;
    }
}

int main() {
    printf("--- FreeRTOS Mutex & Priority Inheritance Simulation ---\n\n");

    // 1. Low Priority task runs first and takes the Mutex safely
    printf("--- Timeline 1: Low Priority starts ---\n");
    Mock_Mutex_Take(&TaskLow, &g_uart_mutex);

    // 2. High Priority task wakes up, wants to run, but needs that same Mutex
    printf("\n--- Timeline 2: High Priority wakes up and demands Mutex ---\n");
    Mock_Mutex_Take(&TaskHigh, &g_uart_mutex);

    // 3. Middle Priority task wakes up. 
    // Without priority inheritance, TaskMid (2) would preempt TaskLow (1) and block TaskHigh (3) forever.
    // Let's see if TaskMid can preempt TaskLow now that TaskLow's priority is inherited to 3!
    printf("\n--- Timeline 3: Mid Priority wakes up and tries to preempt CPU ---\n");
    if (TaskMid.current_priority > TaskLow.current_priority) {
        printf("[PREEMPTION FAIL] Mid Priority (Pri: %d) successfully preempted TaskLow!\n", TaskMid.current_priority);
    } else {
        printf("[PREEMPTION PREVENTED] TaskMid (Pri: %d) CANNOT preempt TaskLow (Temporary Pri: %d)!\n", 
               TaskMid.current_priority, TaskLow.current_priority);
    }

    // 4. TaskLow finishes its critical work and releases the lock
    Mock_Mutex_Give(&g_uart_mutex);

    return 0;
}