#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// 1. 巨集括號陷阱與修正 (Macro Trap vs Safe Macro)
// ============================================================================
#define SQUARE_UNSAFE(x) (x * x)
#define SQUARE_SAFE(x)   ((x) * (x))

void demo_macro_trap(void) {
    int val = 3 + 1;
    int unsafe_res = SQUARE_UNSAFE(3 + 1); // 擴展為 3 + 1 * 3 + 1 = 7
    int safe_res   = SQUARE_SAFE(3 + 1);   // 擴展為 ((3 + 1) * (3 + 1)) = 16

    printf("--- 1. Macro Parentheses Trap Demo ---\n");
    printf("↳ SQUARE_UNSAFE(3 + 1) = %d (Expected 16, got 7 due to operator precedence!)\n", unsafe_res);
    printf("↳ SQUARE_SAFE(3 + 1)   = %d (100%% Safe Parentheses)\n", safe_res);
}

// ============================================================================
// 2. 位元單行操作 (Bit Set & Bit Check)
// ============================================================================
void demo_bit_operations(void) {
    uint32_t reg = 0x00000000;
    
    // 設 Bit 4 為 1
    reg |= (1 << 4);

    // 檢查 Bit 4 是否為 1 (3 種安全寫法)
    bool is_bit4_set_v1 = ((reg >> 4) & 1);
    bool is_bit4_set_v2 = ((reg & (1 << 4)) != 0);

    printf("\n--- 2. Bitwise Operations (Set & Check) ---\n");
    printf("↳ reg Value after Set Bit 4: 0x%08X (Bit 4 = 1)\n", reg);
    printf("↳ Check Bit 4 (Right-Shift & 1): %s\n", is_bit4_set_v1 ? "TRUE (1)" : "FALSE (0)");
    printf("↳ Check Bit 4 ((reg & Mask) != 0): %s\n", is_bit4_set_v2 ? "TRUE (1)" : "FALSE (0)");
}

// ============================================================================
// 3. 16-bit 大小端序位元組對調 (Byte Swap 16-bit)
// ============================================================================
uint16_t swap_uint16(uint16_t val) {
    return ((val & 0x00FF) << 8) | ((val & 0xFF00) >> 8);
}

void demo_byte_swap(void) {
    uint16_t original = 0x1234;
    uint16_t swapped  = swap_uint16(original);

    printf("\n--- 3. 16-Bit Byte Swap (Endianness Conversion) ---\n");
    printf("↳ Original 16-Bit Value: 0x%04X\n", original);
    printf("↳ Swapped 16-Bit Value : 0x%04X (0x1234 -> 0x3412)\n", swapped);
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 51: Firmware 0x10 Review - Macros, Bitwise & ByteSwap ---\n");
    printf("=================================================================\n\n");

    demo_macro_trap();
    demo_bit_operations();
    demo_byte_swap();

    printf("\n=================================================================\n");
    return 0;
}
