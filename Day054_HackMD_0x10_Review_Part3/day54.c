#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// HackMD 0x10 Q7: const 關鍵字 5 大宣告變體驗證
// ============================================================================
void demo_q7_const(void) {
    int val = 100;
    const int *p1 = &val;       // 內容唯讀 (*p1 不能改)
    int * const p2 = &val;      // 位址唯讀 (p2 不能改)
    const int * const p3 = &val;// 內容與位址皆唯讀

    printf("--- HackMD Q7: const Pointer Combinations ---\n");
    printf("↳ const int *p1: Read-only content (*p1 = %d)\n", *p1);
    printf("↳ int * const p2: Read-only address (p2 = %p)\n", (void*)p2);
    printf("↳ const int * const p3: Both read-only (*p3 = %d, p3 = %p)\n", *p3, (void*)p3);
}

// ============================================================================
// HackMD 0x10 Q8: volatile 關鍵字與 square(volatile int *ptr) 修正
// ============================================================================
int square_safe(volatile int *ptr) {
    int a = *ptr; // ⭐️ 鎖定：從實體 RAM 僅讀取一次，防止中斷/硬體隨機改變!
    return a * a;
}

void demo_q8_volatile(void) {
    volatile int sensor_val = 5;
    int res = square_safe(&sensor_val);

    printf("\n--- HackMD Q8: volatile & Safe square() ---\n");
    printf("↳ square_safe(&sensor_val = 5) = %d (100%% Safe from double-read bug!)\n", res);
    printf("↳ 3 Classic volatile Examples:\n");
    printf("  1. Hardware Memory-Mapped Registers (e.g. GPIO IDR)\n");
    printf("  2. Global variables modified in ISR\n");
    printf("  3. Shared variables between multi-threaded/RTOS tasks\n");
}

// ============================================================================
// HackMD 0x10 Q9: 位元操作 (SET_BIT & CLEAR_BIT) 巨集
// ============================================================================
#define SET_BIT(p, n)   ((p) |= (1 << (n)))
#define CLEAR_BIT(p, n) ((p) &= ~(1 << (n)))

void demo_q9_bitwise(void) {
    uint32_t reg = 0x00000000;

    printf("\n--- HackMD Q9: Bitwise SET & CLEAR Macros ---\n");
    SET_BIT(reg, 3);
    printf("↳ After SET_BIT(reg, 3)  : 0x%08X (Bit 3 = 1)\n", reg);
    
    CLEAR_BIT(reg, 3);
    printf("↳ After CLEAR_BIT(reg, 3): 0x%08X (Bit 3 = 0)\n", reg);
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 54: HackMD 0x10 Firmware Interview Review Part 3 ---\n");
    printf("=================================================================\n\n");

    demo_q7_const();
    demo_q8_volatile();
    demo_q9_bitwise();

    printf("\n=================================================================\n");
    return 0;
}
