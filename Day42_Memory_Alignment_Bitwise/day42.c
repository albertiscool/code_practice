#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// 題目 1: Struct Padding 與記憶體對齊實證
// ============================================================================
struct SensorDataA {
    uint8_t  flag;      // 1 Byte (Padding 7 Bytes)
    uint64_t timestamp; // 8 Bytes
    uint16_t id;        // 2 Bytes (Padding 6 Bytes) -> Total 24 Bytes
};

struct SensorDataB {
    uint8_t  flag;      // 1 Byte (Padding 1 Byte)
    uint16_t id;        // 2 Bytes (Padding 4 Bytes)
    uint64_t timestamp; // 8 Bytes -> Total 16 Bytes (Optimized)
};

// ============================================================================
// 題目 2: Volatile 暫存器指標讀取與防護
// ============================================================================
static uint32_t g_simulated_hw_reg = 0x00;

void simulate_hw_register_read(volatile uint32_t *reg) {
    printf("↳ Initial Register Value: 0x%08X\n", *reg);
    // 強迫每一次循環都真槍實彈去記憶體抓最新值
    *reg = 0x01; // 模擬硬體腳位變高
    printf("↳ Hardware updated register to: 0x%08X (Bit 0 Ready)\n", *reg);
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 42: Memory Alignment, Volatile & Bitwise Special Test ---\n");
    printf("=================================================================\n\n");

    // 測試 1: Struct Padding 算術
    printf("--- 1. Struct Alignment & Padding Test ---\n");
    printf("↳ sizeof(SensorDataA) = %zu Bytes (Unoptimized)\n", sizeof(struct SensorDataA));
    printf("↳ sizeof(SensorDataB) = %zu Bytes (Optimized)\n", sizeof(struct SensorDataB));

    // 測試 2: Volatile 防護測試
    printf("\n--- 2. Volatile Pointer Protection Test ---\n");
    volatile uint32_t *status_reg = (volatile uint32_t *)&g_simulated_hw_reg;
    simulate_hw_register_read(status_reg);

    // 測試 3: Bitwise 暫存器遮罩操作
    printf("\n--- 3. Bitwise Register Mask Operations ---\n");
    uint32_t CR_REG = 0x00000000;
    printf("↳ Initial CR_REG: 0x%08X\n", CR_REG);

    // 1. 置位 (Set Bit 4)
    CR_REG |= (1 << 4);
    printf("↳ After Set Bit 4   (CR_REG |= (1<<4)) : 0x%08X\n", CR_REG);

    // 2. 翻轉 (Toggle Bit 2)
    CR_REG ^= (1 << 2);
    printf("↳ After Toggle Bit 2(CR_REG ^= (1<<2)) : 0x%08X\n", CR_REG);

    // 3. 清零 (Clear Bit 4)
    CR_REG &= ~(1 << 4);
    printf("↳ After Clear Bit 4 (CR_REG &= ~(1<<4)): 0x%08X\n", CR_REG);

    printf("\n=================================================================\n");
    return 0;
}
