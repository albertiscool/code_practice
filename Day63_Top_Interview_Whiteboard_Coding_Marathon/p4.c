#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// ============================================================================
// 標準滿分解答：2 的冪次方判定 (Power of Two) - $O(1)$ 極速位元運算法
// ============================================================================
bool isPowerOfTwo(int n)
{
    // 🌟 兩大面試關鍵細節：
    // 1. 邊界防禦：必須 n > 0 (排除 0 與 負數)
    // 2. 運算子優先順序：== 優先級高於 &，因此 (n & (n - 1)) 必須加括號！
    return (n > 0) && ((n & (n - 1)) == 0);
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    // 測試 1：正整數 2 的冪次方 (1, 2, 4, 16, 1024) -> 預期: true
    assert(isPowerOfTwo(1) == true);
    assert(isPowerOfTwo(2) == true);
    assert(isPowerOfTwo(4) == true);
    assert(isPowerOfTwo(16) == true);
    assert(isPowerOfTwo(1024) == true);
    printf("測試 1 (2 的冪次方 1, 2, 4, 16, 1024) -> 全部正確回傳 true！\n");

    // 測試 2：非 2 的冪次方 (3, 5, 6, 18) -> 預期: false
    assert(isPowerOfTwo(3) == false);
    assert(isPowerOfTwo(5) == false);
    assert(isPowerOfTwo(6) == false);
    assert(isPowerOfTwo(18) == false);
    printf("測試 2 (非 2 的冪次方 3, 5, 6, 18) -> 全部正確回傳 false！\n");

    // 測試 3：邊界極端值 (0, 負數) -> 預期: false
    assert(isPowerOfTwo(0) == false);
    assert(isPowerOfTwo(-16) == false);
    assert(isPowerOfTwo(-2147483648) == false);
    printf("測試 3 (邊界極端值 0, -16, INT_MIN) -> 全部正確回傳 false！\n");

    printf("\n🎉 所有測試案例全部 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（核心位元概念完全正確！）：
bool isPowerOfTwo_original(int n)
{
    // 💡 盲點 1（致命運算子優先級）：
    // C 語言中 == 的優先級「高於」&！
    // 所以 (n & (n - 1) == 0) 會被編譯器解讀成 n & ((n - 1) == 0)，導致邏輯出錯！
    // 正確寫法：((n & (n - 1)) == 0) 必須把 & 括起來！
    
    // 💡 盲點 2（邊界防呆）：
    // 必須加上 n > 0，因為 0 和負數（如 0 & -1 == 0）不是 2 的冪次方！

    return (n & (n-1) == 0) ? true : false;
}
*/