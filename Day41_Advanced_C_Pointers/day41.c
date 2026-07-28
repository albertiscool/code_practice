#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// 範例 1: 雙重指標修正 Memory Leak 與指針修改
// ============================================================================
void allocate_buffer_correct(char **ptr) {
    *ptr = (char *)malloc(1024); // 解開一層指標，直接修改 main 裡的 my_ptr 本體
}

// ============================================================================
// 範例 2: 利用「函式指標陣列」打造無人機狀態機 (FSM)
// ============================================================================
typedef enum {
    STATE_IDLE = 0,
    STATE_TAKEOFF,
    STATE_FLYING,
    STATE_LANDING,
    STATE_MAX
} State_t;

void do_state_idle(void) {
    printf("[狀態 0: IDLE] 解鎖馬達，等待起飛指令...\n");
}

void do_state_takeoff(void) {
    printf("[狀態 1: TAKEOFF] 馬達推力 80%%，升空至 10 公尺...\n");
}

void do_state_flying(void) {
    printf("[狀態 2: FLYING] 啟動 AI YOLO 辨識與 GPS 巡航...\n");
}

void do_state_landing(void) {
    printf("[狀態 3: LANDING] 開啟向下雷達，緩慢降落接地...\n");
}

// 定義函式指標型態
typedef void (*StateFunc_t)(void);

// 建立狀態對照表 (函式指標陣列)
static StateFunc_t g_state_table[STATE_MAX] = {
    [STATE_IDLE]    = do_state_idle,
    [STATE_TAKEOFF] = do_state_takeoff,
    [STATE_FLYING]  = do_state_flying,
    [STATE_LANDING] = do_state_landing
};

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 41: Advanced C Pointers & State Machine Special Test ---\n");
    printf("=================================================================\n\n");

    // 測試 1: 雙重指標配置
    printf("--- 1. Double Pointer Allocation Test ---\n");
    char *my_ptr = NULL;
    allocate_buffer_correct(&my_ptr);
    if (my_ptr != NULL) {
        printf("↳ Allocation Success via Double Pointer! Address: %p\n", (void*)my_ptr);
        free(my_ptr);
    }

    // 測試 2: 函式指標狀態機
    printf("\n--- 2. Function Pointer State Machine (FSM) ---\n");
    for (int i = 0; i < STATE_MAX; i++) {
        State_t current_state = (State_t)i;
        g_state_table[current_state](); // 直接以索引查表執行對應函式
    }

    // 測試 3: 端序與指標算術測試
    printf("\n--- 3. Endianness & Pointer Arithmetic Test ---\n");
    uint32_t arr[3] = {0x12345678, 0x9ABCDEF0, 0x11223344};
    uint8_t *p = (uint8_t *)(arr + 1);

    printf("↳ (arr + 1) Address Offset: %ld Bytes\n", (uintptr_t)(arr + 1) - (uintptr_t)arr);
    printf("↳ Value at *p (Little-Endian lowest byte): 0x%X (Expected: 0xF0)\n", *p);

    printf("\n=================================================================\n");
    return 0;
}
