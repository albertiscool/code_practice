#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// 題目 1: Event Group 與多條件同步解鎖 (Logical AND) 模擬
// ============================================================================
#define BIT_GPS_READY     (1 << 0)
#define BIT_GYRO_READY    (1 << 1)
#define BIT_BATTERY_OK    (1 << 2)
#define ALL_SYNC_BITS     (BIT_GPS_READY | BIT_GYRO_READY | BIT_BATTERY_OK)

typedef struct {
    uint32_t event_bits;
} Mock_EventGroup_t;

static Mock_EventGroup_t g_drone_events = { .event_bits = 0 };

void simulate_event_group_wait(void) {
    printf("[Event Group] Task_Takeoff waiting for ALL bits (GPS, GYRO, BATTERY)...\n");
    
    // 模擬 3 個條件依序完成
    g_drone_events.event_bits |= BIT_GPS_READY;
    printf("  ↳ Bit 0 Set: GPS Ready. Current Bits: 0x%02X (Waiting...)\n", g_drone_events.event_bits);

    g_drone_events.event_bits |= BIT_GYRO_READY;
    printf("  ↳ Bit 1 Set: Gyro Ready. Current Bits: 0x%02X (Waiting...)\n", g_drone_events.event_bits);

    g_drone_events.event_bits |= BIT_BATTERY_OK;
    printf("  ↳ Bit 2 Set: Battery OK. Current Bits: 0x%02X\n", g_drone_events.event_bits);

    // 檢查 Logical AND (xWaitForAllBits = pdTRUE)
    if ((g_drone_events.event_bits & ALL_SYNC_BITS) == ALL_SYNC_BITS) {
        printf("  ↳ [UNLOCKED] All 3 conditions MET! Task_Takeoff Unblocked and Executing!\n");
    }
}

// ============================================================================
// 題目 3: FreeRTOS Queue Pass-by-Value (複製值) 模擬
// ============================================================================
typedef struct {
    uint32_t sensor_id;
    float temp;
} SensorPayload_t;

typedef struct {
    SensorPayload_t buffer[5];
    uint32_t count;
    uint32_t max_size;
} Mock_Queue_t;

bool mock_xQueueSend(Mock_Queue_t *q, const SensorPayload_t *item_to_send, uint32_t timeout_ms) {
    if (q->count >= q->max_size) {
        printf("  ↳ [Queue Full] Task_A blocked waiting for %u ms...\n", timeout_ms);
        printf("  ↳ [Timeout] 1000ms expired, Queue still full! Returning errQUEUE_FULL (pdFAIL / 0).\n");
        return false; // pdFAIL / errQUEUE_FULL (0)
    }

    // ⭐️ 核心物理：FreeRTOS 預設是 Copy-by-Value (memcpy 整個結構體內容)
    memcpy(&q->buffer[q->count], item_to_send, sizeof(SensorPayload_t));
    q->count++;
    printf("  ↳ [Queue Send] Copied %zu bytes (Pass-by-Value) into Queue slot %u.\n", 
           sizeof(SensorPayload_t), q->count - 1);
    return true; // pdPASS (1)
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 46: FreeRTOS Queue, Event Groups & Heap 4/5 Test ---\n");
    printf("=================================================================\n\n");

    // 測試 1: Event Group AND 多條件同步
    printf("--- 1. Event Group Logical AND Synchronization ---\n");
    simulate_event_group_wait();

    // 測試 2: FreeRTOS Queue 複製值與滿佇列逾時
    printf("\n--- 2. Queue Pass-by-Value & Full Queue Timeout Simulation ---\n");
    Mock_Queue_t q = { .count = 5, .max_size = 5 }; // 模擬已滿的 Queue
    SensorPayload_t payload = { .sensor_id = 101, .temp = 36.5f };

    bool result = mock_xQueueSend(&q, &payload, 1000);
    printf("↳ Result of xQueueSend on Full Queue: %s (Status: %d)\n", 
           result ? "pdPASS" : "errQUEUE_FULL", result);

    printf("\n=================================================================\n");
    return 0;
}
