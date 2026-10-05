#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// ============================================================================
// 題目 1【2 的冪次方 (Power of Two - LeetCode #231)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (IC 廠經典位元題)
// 演算法：Brian Kernighan 位元消除法 - $O(1)$ 時間, $O(1)$ 空間
// ============================================================================

bool isPowerOfTwo(int n)
{
    // 1. 負數與 0 必不是 2 的冪次方 (n > 0)
    // 2. 2 的冪次方二進位恰好只有 1 個 1，因此 n & (n - 1) 必等於 0
    // ⚠️ 注意：C 語言中 == 的優先級高於 &，因此 (n & (n - 1)) 必須加括號！
    return (n > 0) && ((n & (n - 1)) == 0);
}

int main()
{
    // 測試 1: n = 1 -> true
    assert(isPowerOfTwo(1) == true);
    printf("測試 1 通過: n = 1 是 2 的冪 (true)\n");

    // 測試 2: n = 16 -> true
    assert(isPowerOfTwo(16) == true);
    printf("測試 2 通過: n = 16 是 2 的冪 (true)\n");

    // 測試 3: n = 3 -> false
    assert(isPowerOfTwo(3) == false);
    printf("測試 3 通過: n = 3 不是 2 的冪 (false)\n");

    // 測試 4: n = 0 與負數 -> false
    assert(isPowerOfTwo(0) == false);
    assert(isPowerOfTwo(-16) == false);
    printf("測試 4 通過: n = 0 與負數不是 2 的冪 (false)\n");

    printf("\n🎉 p1.c 2 的冪次方所有測試案例全數 100%% 通過！\n");
    return 0;
}
