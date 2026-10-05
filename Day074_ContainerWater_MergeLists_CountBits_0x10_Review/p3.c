#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// ============================================================================
// 題目 3【位元計數 (Counting Bits - LeetCode #338)】
// 難度：🟢 Easy / 🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥 (IC 廠經典位元動態轉移題)
// 演算法：位元動態規劃轉移 (Bitwise Dynamic Programming) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

int* countBits(int n, int* returnSize)
{
    *returnSize = n + 1;
    
    // 1. 必須使用 malloc 動態配置記憶體回傳 (避免回傳區域變數之懸空指標 Dangling Pointer)
    int* ans = (int*)malloc((n + 1) * sizeof(int));

    // 2. 初始狀態：0 的二進位 1 的數量為 0
    ans[0] = 0;

    // 3. 核心位元轉移 DP 公式：
    // 數字 i 的二進位 1 的數量 ＝ (i >> 1 的 1 的數量) ＋ (i 的最低位是否為 1)
    for (int i = 1; i <= n; i++)
    {
        ans[i] = ans[i >> 1] + (i & 1);
    }

    return ans;
}

int main()
{
    // 測試 1: n = 2 -> [0, 1, 1]
    int size1 = 0;
    int* res1 = countBits(2, &size1);
    assert(size1 == 3);
    assert(res1[0] == 0 && res1[1] == 1 && res1[2] == 1);
    printf("測試 1 通過: n = 2 ➔ [%d, %d, %d]\n", res1[0], res1[1], res1[2]);
    free(res1);

    // 測試 2: n = 5 -> [0, 1, 1, 2, 1, 2]
    int size2 = 0;
    int* res2 = countBits(5, &size2);
    assert(size2 == 6);
    int expected2[6] = {0, 1, 1, 2, 1, 2};
    for (int i = 0; i < 6; i++) {
        assert(res2[i] == expected2[i]);
    }
    printf("測試 2 通過: n = 5 ➔ [0, 1, 1, 2, 1, 2]\n");
    free(res2);

    printf("\n🎉 p3.c 位元計數 (Counting Bits) 所有測試案例全數 100%% 通過！\n");
    return 0;
}
