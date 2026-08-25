#include <stdio.h>
#include <stdint.h>
#include <assert.h>

// ============================================================================
// 題目 1【韌體暫存器位元操作大四喜 (Register Bit Manipulation)】
// 難度：🟢 Easy | 韌體出題頻率：🔥🔥🔥🔥🔥 (MCU 廠 100% 必考底層功)
// ============================================================================

// 1. 將 val 的第 n 個 bit 設定為 1 (Set Bit)
uint32_t set_bit(uint32_t val, int n) {
    // 🌟 專業提示：使用 1U 代替 1，避免當 n = 31 時 signed int 溢位引發 UB
    val |= (1U << n);
    return val;
}

// 2. 將 val 的第 n 個 bit 清除為 0 (Clear Bit)
uint32_t clear_bit(uint32_t val, int n) {
    val &= ~(1U << n);
    return val;
}

// 3. 將 val 的第 n 個 bit 反轉 (0變1, 1變0) (Toggle Bit)
uint32_t toggle_bit(uint32_t val, int n) {
    val ^= (1U << n);
    return val;
}

// 4. 讀取 val 的第 n 個 bit 數值 (回傳 0 或 1) (Read Bit)
uint32_t read_bit(uint32_t val, int n) {
    return (val >> n) & 1U;
}

int main()
{
    uint32_t reg = 0x00000000; // 0b0000...0000

    // 測試 Set Bit
    reg = set_bit(reg, 3); // 第 3 bit 設為 1 -> 0x00000008
    assert(reg == 0x08);
    assert(read_bit(reg, 3) == 1);
    assert(read_bit(reg, 2) == 0);

    // 測試 Clear Bit
    reg = set_bit(reg, 5); // 0x28
    reg = clear_bit(reg, 3); // 清除第 3 bit -> 0x20
    assert(reg == 0x20);
    assert(read_bit(reg, 3) == 0);

    // 測試 Toggle Bit
    reg = toggle_bit(reg, 5); // 第 5 bit 從 1 變 0 -> 0x00
    assert(reg == 0x00);
    reg = toggle_bit(reg, 5); // 第 5 bit 從 0 變 1 -> 0x20
    assert(reg == 0x20);

    printf("\n🎉 p1.c 暫存器位元操作大四喜 所有測試案例全數 100%% 通過！\n");
    return 0;
}
