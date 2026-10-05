#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// HackMD 0x10 Q10: 絕對記憶體位址存取單行語法
// ============================================================================
static uint32_t fake_hardware_reg = 0x0000;

void demo_q10_absolute_memory(void) {
    uintptr_t addr = (uintptr_t)&fake_hardware_reg;
    
    // ⭐️ 核心單行寫法 (One-liner): 轉型為 (volatile uint32_t *) 再解引用 (*)
    *( (volatile uint32_t *)addr ) = 0xAA55;

    printf("--- HackMD Q10: Absolute Memory Access ---\n");
    printf("↳ Address: 0x%p, Written Value: 0x%04X\n", (void*)addr, fake_hardware_reg);
}

// ============================================================================
// HackMD 0x10 Q11: ISR 正確規範結構 (無傳參、無回傳、無 printf、無浮點)
// ============================================================================
static volatile bool g_sensor_triggered = false;

// ✅ 正確的 ISR 宣告：void 回傳、void 傳參、極速離開
void EXTI0_IRQHandler_Clean(void) {
    g_sensor_triggered = true; // 僅修改旗標，不寫 printf/malloc/float
}

void demo_q11_isr_rules(void) {
    printf("\n--- HackMD Q11: ISR Code Review Rules ---\n");
    printf("↳ 4 Fatal ISR Errors:\n");
    printf("  1. Return type MUST be void (No software caller receives return value)\n");
    printf("  2. Arguments MUST be (void) (Hardware triggers ISR asynchronously)\n");
    printf("  3. Strictly NO printf()/malloc() (Non-reentrant, IO blocking, causes deadlocks)\n");
    printf("  4. Avoid float/double (High FPU context-switch latency)\n");
}

// ============================================================================
// HackMD 0x10 Q12: 隱式型態轉換與二補數 42 億算術
// ============================================================================
void demo_q12_unsigned_trap(void) {
    unsigned int a = 6;
    int b = -20;
    
    // ⭐️ 二補數解密: -20 被轉為 unsigned int 時變成 2^32 - 20 = 4,294,967,276
    unsigned int b_unsigned = (unsigned int)b;

    printf("\n--- HackMD Q12: Unsigned Implicit Casting & Two's Complement ---\n");
    printf("↳ (int)-20 cast to unsigned int = %u (0xFFFFFFEC)\n", b_unsigned);
    printf("↳ (a + b > 6) Result: %s\n", (a + b > 6) ? "TRUE (> 6)" : "FALSE (<= 6)");
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 55: HackMD 0x10 Firmware Interview Review Part 4 ---\n");
    printf("=================================================================\n\n");

    demo_q10_absolute_memory();
    demo_q11_isr_rules();
    demo_q12_unsigned_trap();

    printf("\n=================================================================\n");
    return 0;
}
