#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// 題目 1: FreeRTOS 搶佔式排程與 Task 狀態切換模擬
// ============================================================================
typedef enum { TASK_READY, TASK_RUNNING, TASK_BLOCKED } TaskState_t;

typedef struct {
    const char *name;
    uint32_t priority; // 數字越大優先級越高
    TaskState_t state;
} Mock_TCB_t;

static Mock_TCB_t g_task_high = { .name = "Task_High", .priority = 3, .state = TASK_RUNNING };
static Mock_TCB_t g_task_low  = { .name = "Task_Low",  .priority = 1, .state = TASK_READY };

void simulate_scheduler_tick(bool is_100ms_expired) {
    if (g_task_high.state == TASK_RUNNING) {
        printf("↳ [Scheduler] Task_High calls vTaskDelay(100) -> Moves to BLOCKED state.\n");
        g_task_high.state = TASK_BLOCKED;
        g_task_low.state  = TASK_RUNNING;
        printf("↳ [Scheduler] Context Switch -> Task_Low (Priority 1) is now RUNNING.\n");
    } else if (g_task_high.state == TASK_BLOCKED && is_100ms_expired) {
        printf("\n↳ [SysTick 100ms Timer] Task_High Timer Expired -> Moves to READY state.\n");
        g_task_high.state = TASK_RUNNING;
        g_task_low.state  = TASK_READY;
        printf("↳ [Scheduler Preemption] Task_High (Priority 3) PREEMPTS Task_Low! Task_High is RUNNING.\n");
    }
}

// ============================================================================
// 題目 2: Priority Inheritance (優先權繼承) 模擬
// ============================================================================
typedef struct {
    Mock_TCB_t *owner;
    uint32_t original_priority;
} Mock_Mutex_t;

void simulate_priority_inheritance(Mock_TCB_t *task_high, Mock_TCB_t *task_low, Mock_Mutex_t *mutex) {
    printf("\n--- Priority Inheritance Simulation ---\n");
    printf("↳ Initial: Task_Low Priority = %d, Task_High Priority = %d\n", task_low->priority, task_high->priority);
    printf("↳ Task_High tries to take Mutex owned by Task_Low...\n");

    // 觸發優先權繼承 (Priority Inheritance)
    mutex->original_priority = task_low->priority;
    task_low->priority = task_high->priority; // 暫時提升低優先級 Task 的優先級！

    printf("↳ [Priority Inheritance] Task_Low Priority temporarily ELEVATED to %d!\n", task_low->priority);
    printf("↳ Medium Priority Task_B (Priority 2) CANNOT preempt Task_Low now!\n");

    // Task_Low 歸還 Mutex
    task_low->priority = mutex->original_priority; // 恢復原始優先級
    printf("↳ Task_Low releases Mutex -> Priority restored to %d. Task_High takes Mutex and runs!\n", task_low->priority);
}

// ============================================================================
// 題目 3: Task Notification 模擬 (ISR -> Task 1-to-1 快速通知)
// ============================================================================
typedef struct {
    uint32_t notification_value;
    bool is_notified;
} Mock_TaskNotification_t;

static Mock_TaskNotification_t g_process_task_notif = { .notification_value = 0, .is_notified = false };

void UART_ISR_Handler(void) {
    printf("\n--- UART ISR Handler ---\n");
    printf("[ISR] Data Received -> Triggering vTaskNotifyGiveFromISR()...\n");
    
    // ⭐️ 核心優勢：無動態配置開銷，直接修改 TCB 內部的 32-bit 通知數值！
    g_process_task_notif.notification_value++;
    g_process_task_notif.is_notified = true;
    printf("[ISR] Notification Sent! Process Task will unblock immediately without Queue/Semaphore RAM overhead.\n");
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 45: FreeRTOS Scheduler, Priority Inheritance & Notif ---\n");
    printf("=================================================================\n\n");

    // 測試 1: 搶佔式排程時間線
    printf("--- 1. Preemptive Scheduler & vTaskDelay Timeline ---\n");
    simulate_scheduler_tick(false); // Task_High 進入 Delay 100ms
    simulate_scheduler_tick(true);  // 100ms 時間到，Task_High 喚醒搶佔

    // 測試 2: 優先權繼承除錯
    Mock_TCB_t t_high = { .name = "Task_A (High)", .priority = 3, .state = TASK_READY };
    Mock_TCB_t t_low  = { .name = "Task_C (Low)",  .priority = 1, .state = TASK_RUNNING };
    Mock_Mutex_t mutex = { .owner = &t_low, .original_priority = 1 };
    simulate_priority_inheritance(&t_high, &t_low, &mutex);

    // 測試 3: ISR 任務通知 (Task Notification)
    printf("\n--- 3. ISR to Task Notification Test ---\n");
    UART_ISR_Handler();

    printf("\n=================================================================\n");
    return 0;
}
