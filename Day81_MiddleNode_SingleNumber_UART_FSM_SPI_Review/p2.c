#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 2【只出現一次的數字 (Single Number - LeetCode #136)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大 IC 廠/系統廠必考經典題)
// 演算法：全域 XOR 異或消去法 ($a \oplus a = 0$) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

int singleNumber(int* nums, int numsSize)
{
    // 防呆處理
    if (nums == NULL || numsSize <= 0)
    {
        return 0;
    }
    
    int result = 0;

    // 利用 XOR 特性：相同數字兩兩相消為 0，最後留下的就是只出現一次的數字！
    for (int i = 0; i < numsSize; i++)
    {
        result ^= nums[i];
    }
    
    return result;
}

int main()
{
    // 測試 1: [2, 2, 1] -> 1
    int a1[] = {2, 2, 1};
    int ans1 = singleNumber(a1, 3);
    printf("測試 1 通過: [2, 2, 1] 中只出現一次的數字是: %d (預期: 1)\n", ans1);
    assert(ans1 == 1);

    // 測試 2: [4, 1, 2, 1, 2] -> 4
    int a2[] = {4, 1, 2, 1, 2};
    int ans2 = singleNumber(a2, 5);
    printf("測試 2 通過: [4, 1, 2, 1, 2] 中只出現一次的數字是: %d (預期: 4)\n", ans2);
    assert(ans2 == 4);

    // 測試 3: [1] -> 1
    int a3[] = {1};
    int ans3 = singleNumber(a3, 1);
    printf("測試 3 通過: [1] 中只出現一次的數字是: %d (預期: 1)\n", ans3);
    assert(ans3 == 1);

    // 測試 4: [7, 3, 5, 4, 5, 3, 4] -> 7
    int a4[] = {7, 3, 5, 4, 5, 3, 4};
    int ans4 = singleNumber(a4, 7);
    printf("測試 4 通過: [7, 3, 5, 4, 5, 3, 4] 中只出現一次的數字是: %d (預期: 7)\n", ans4);
    assert(ans4 == 7);

    printf("\n🎉 p2.c 只出現一次的數字所有測試案例全數 100%% 通過！\n");
    return 0;
}
