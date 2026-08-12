#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// HackMD 0x10 Q13: malloc(0) 與 Heap 記憶體碎片化
// ============================================================================
void demo_q13_malloc_zero(void) {
    char *ptr = (char *)malloc(0);
    printf("--- HackMD Q13: malloc(0) Behavior ---\n");
    if (ptr != NULL) {
        printf("↳ malloc(0) returned a non-NULL valid pointer: %p\n", (void*)ptr);
        printf("↳ MUST call free(ptr) to avoid memory leak!\n");
        free(ptr);
    } else {
        printf("↳ malloc(0) returned NULL.\n");
    }
}

// ============================================================================
// HackMD 0x10 Q14: typedef vs #define 型態別名與指標範例
// ============================================================================
#define dps char *
typedef char * dscp;

void demo_q14_typedef_vs_define(void) {
    dps p1, p2;   // p1 是 char*, p2 是 char!
    dscp p3, p4;  // p3 與 p4 通通都是 char*!

    p1 = "Hello";
    p2 = 'A';
    p3 = "World";
    p4 = "Embedded";

    printf("\n--- HackMD Q14: typedef vs #define ---\n");
    printf("↳ dps p1, p2 -> p1 is char* (\"%s\"), p2 is char ('%c')\n", p1, p2);
    printf("↳ dscp p3, p4 -> p3 is char* (\"%s\"), p4 is char* (\"%s\")\n", p3, p4);
}

// ============================================================================
// HackMD 0x10 Q15: 前加與後加純潔拆解 (避免 UB 未定義行為)
// ============================================================================
void demo_q15_inc_dec(void) {
    int a = 5;
    int x = a++; // 後加: x 拿舊值 5，a 變成 6
    int y = ++a; // 前加: a 先變成 7，y 拿新值 7

    printf("\n--- HackMD Q15: Sequence Points & Post/Pre Increment ---\n");
    printf("↳ int a = 5; int x = a++; int y = ++a;\n");
    printf("↳ x = %d (Post-increment returns 5)\n", x);
    printf("↳ y = %d (Pre-increment returns 7)\n", y);
    printf("↳ a = %d\n", a);
}

// ============================================================================
// HackMD 0x10 Q16: 工業級 Busy-Waiting Timeout 超時防線
// ============================================================================
static volatile uint8_t fake_status_reg = 0x00; // Bit 7 預設為 0 (未就緒)

bool wait_for_ready_safe(uint32_t timeout_cnt) {
    while ((fake_status_reg & 0x80) == 0) {
        if (--timeout_cnt == 0) {
            return false; // 超時離開，防止 CPU 無限死鎖引爆 Watchdog!
        }
    }
    return true;
}

void demo_q16_timeout_defense(void) {
    printf("\n--- HackMD Q16: Hardware Busy-Waiting Timeout Defense ---\n");
    bool success = wait_for_ready_safe(1000);
    printf("↳ Busy-waiting with 1000 counter timeout -> Result: %s (Prevented System Deadlock!)\n", 
            success ? "SUCCESS" : "TIMEOUT_FAILED");
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 56: HackMD 0x10 Firmware Interview Final Capstone ---\n");
    printf("=================================================================\n\n");

    demo_q13_malloc_zero();
    demo_q14_typedef_vs_define();
    demo_q15_inc_dec();
    demo_q16_timeout_defense();

    printf("\n=================================================================\n");
    printf("🎉 CONGRATULATIONS! HACKMD 0x10 (16 QUESTIONS) ALL PASSED! 🎉\n");
    printf("=================================================================\n");
    return 0;
}
