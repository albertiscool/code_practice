#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// HackMD 0x10 Q1: 16-bit 溢位防禦與 SECONDS_PER_YEAR 巨集
// ============================================================================
#define SECONDS_PER_YEAR (60UL * 60UL * 24UL * 365UL)

void demo_q1_seconds(void) {
    printf("--- HackMD Q1: SECONDS_PER_YEAR Macro ---\n");
    printf("↳ SECONDS_PER_YEAR = %lu UL (Prevents 16-bit 32767 Overflow!)\n", SECONDS_PER_YEAR);
}

// ============================================================================
// HackMD 0x10 Q2: 標準 MIN 巨集與副作用 (Side Effects)
// ============================================================================
#define MIN(A, B) ((A) <= (B) ? (A) : (B))

void demo_q2_min_macro(void) {
    int arr[] = { 10, 20, 30 };
    int *p = arr;
    int b = 15;

    printf("\n--- HackMD Q2: Standard MIN Macro & Side Effects ---\n");
    printf("↳ Initial *p = %d, b = %d\n", *p, b);

    // ⭐️ 核心副作用：*p++ 會在三元運算子比較與取值時被展開評估 2 次！
    int least = MIN(*p++, b);
    printf("↳ MIN(*p++, b) result = %d\n", least);
    printf("↳ Pointer p advanced TWICE! Current *p is now: %d (Array index skipped!)\n", *p);
}

// ============================================================================
// HackMD 0x10 Q3: 預處理指令 #error 實戰模組條件檢查
// ============================================================================
#define BOARD_VERSION 2

#ifndef BOARD_VERSION
    #error "BOARD_VERSION is not defined! Halting compilation."
#endif

void demo_q3_error_directive(void) {
    printf("\n--- HackMD Q3: Preprocessor #error Check ---\n");
    printf("↳ BOARD_VERSION is defined (%d). Preprocessor #error check passed!\n", BOARD_VERSION);
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 52: HackMD 0x10 Firmware Interview Review Part 1 ---\n");
    printf("=================================================================\n\n");

    demo_q1_seconds();
    demo_q2_min_macro();
    demo_q3_error_directive();

    printf("\n=================================================================\n");
    return 0;
}
