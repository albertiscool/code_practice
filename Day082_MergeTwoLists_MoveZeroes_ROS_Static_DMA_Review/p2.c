#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 2【移動零 (Move Zeroes - LeetCode #283)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠白板高頻題)
// 演算法：快慢雙指標原地覆蓋法 (Fast & Slow Pointers In-Place) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

void moveZeroes(int* nums, int numsSize)
{
    // 防呆處理
    if (nums == NULL || numsSize <= 1)
    {
        return;
    }

    int slow = 0;
    int fast = 0;

    // 步驟 1：快指標 fast 走訪整個陣列，將所有非 0 元素緊湊移到 slow 位置
    while (fast < numsSize)
    {
        if (nums[fast] != 0)
        {
            nums[slow] = nums[fast];
            slow++;
        }
        fast++;
    }

    // 步驟 2：將 slow 後面剩下的所有剩餘位置全部補 0
    while (slow < numsSize)
    {
        nums[slow] = 0;
        slow++;
    }
}

int main()
{
    // 測試 1: [0, 1, 0, 3, 12] -> [1, 3, 12, 0, 0]
    int a1[] = {0, 1, 0, 3, 12};
    moveZeroes(a1, 5);
    int exp1[] = {1, 3, 12, 0, 0};
    for (int i = 0; i < 5; i++) {
        assert(a1[i] == exp1[i]);
    }
    printf("測試 1 通過: [0, 1, 0, 3, 12] 成功移動為 [1, 3, 12, 0, 0]\n");

    // 測試 2: [0] -> [0]
    int a2[] = {0};
    moveZeroes(a2, 1);
    assert(a2[0] == 0);
    printf("測試 2 通過: [0] 原地處理正常 [0]\n");

    // 測試 3: [1, 2, 3] (無零) -> [1, 2, 3]
    int a3[] = {1, 2, 3};
    moveZeroes(a3, 3);
    int exp3[] = {1, 2, 3};
    for (int i = 0; i < 3; i++) {
        assert(a3[i] == exp3[i]);
    }
    printf("測試 3 通過: [1, 2, 3] (無零) 保持 [1, 2, 3]\n");

    // 測試 4: [0, 0, 1] -> [1, 0, 0]
    int a4[] = {0, 0, 1};
    moveZeroes(a4, 3);
    assert(a4[0] == 1 && a4[1] == 0 && a4[2] == 0);
    printf("測試 4 通過: [0, 0, 1] 成功移動為 [1, 0, 0]\n");

    printf("\n🎉 p2.c 移動零所有測試案例全數 100%% 通過！\n");
    return 0;
}
