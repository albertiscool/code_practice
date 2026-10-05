#include <stdio.h>
#include <stdint.h>
#include <assert.h>

// ============================================================================
// 題目 3【反轉 32 位元整數的 Bit (Reverse Bits - LeetCode #190)】
// 寫法 1：移位暫存器法 (Shift Register) - $O(1)$ 時間 (固定 32 步), $O(1)$ 空間
// ============================================================================
uint32_t reverseBits(uint32_t n)
{
    uint32_t result = 0; // 必須使用無號整數 uint32_t (避免最高位 31 bit 被當成有號負數溢位)

    for (int i = 0; i < 32; i++)
    {
        // 1. result 左移 1 位 (騰出最右邊的空位)，並將 n 的最後一個 bit (n & 1) 塞入
        result = (result << 1) | (n & 1);

        // 2. n 右移 1 位 (丟掉已處理完的最後一位)
        n >>= 1;
    }

    return result;
}

// ============================================================================
// 寫法 2：位置鏡射法 (順著你的原版思路修正)
// ============================================================================
uint32_t reverseBits_mirror(uint32_t n)
{
    uint32_t result = 0;

    for (int i = 0; i < 32; i++)
    {
        // 第 0 bit 要搬到第 31 bit，第 i bit 要搬到第 (31 - i) bit！
        result |= ((n & 1) << (31 - i));
        n >>= 1;
    }

    return result;
}

int main()
{
    // 測試 1: 43261596 -> 反轉後為 964176192
    uint32_t n1 = 43261596;
    uint32_t ans1 = reverseBits(n1);
    printf("測試 1: %u -> 反轉為: %u (預期: 964176192)\n", n1, ans1);
    assert(ans1 == 964176192);

    // 測試 2: 1 (0b...0001) -> 反轉後第 31 bit 為 1 (2147483648 / 0x80000000)
    uint32_t n2 = 1;
    uint32_t ans2 = reverseBits(n2);
    printf("測試 2: 1 -> 反轉為: %u (預期: 2147483648)\n", ans2);
    assert(ans2 == 2147483648U);

    printf("\n🎉 p3.c 所有測試案例全數 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（位元操作結構非常漂亮！）：
uint32_t reverseBits_original(uint32_t n)
{
    // 💡 盲點 1：建議宣告為 uint32_t（避免 int 左移至第 31 bit 觸發有號整數未定義行為）
    uint32_t result = 0;
    
    for(int i = 0; i < 32; i++)
    {
        // 💡 盲點 2（反轉方向）：
        // 你的寫法 ((n & 1) << i) 會把第 0 bit 依然放到第 0 bit、第 1 bit 放到第 1 bit，
        // 這樣是在「原樣複製」而不是「反轉」！
        // 修正：第 0 bit 應該左移 31 格放到第 31 bit，因此要左移 (31 - i) 格！
        result |= ((n & 1) << (31 - i));
        n = n >> 1;
    }
    return result;
}
*/
