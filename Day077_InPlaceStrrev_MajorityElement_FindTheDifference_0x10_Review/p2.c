#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 2【主要元素 (Majority Element - LeetCode #169)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠經典演算法)
// 演算法：摩爾投票演算法 (Boyer-Moore Voting Algorithm) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

int majorityElement(int* nums, int numsSize)
{
    // 防呆處理
    if (nums == NULL || numsSize <= 0)
    {
        return 0;
    }

    int candidate = nums[0];
    int count = 0;

    // 陣營抵消投票過程
    for (int i = 0; i < numsSize; i++)
    {
        if (count == 0)
        {
            candidate = nums[i]; // 更換新候選人
            count = 1;
        }
        else if (nums[i] == candidate)
        {
            count++;             // 同陣營，票數 +1
        }
        else
        {
            count--;             // 不同陣營，互相抵消 -1
        }
    }

    return candidate;
}

int main()
{
    // 測試 1: [3, 2, 3] -> 3
    int num1[3] = {3, 2, 3};
    int ans1 = majorityElement(num1, 3);
    printf("測試 1 通過: [3, 2, 3] 主要元素為: %d (預期: 3)\n", ans1);
    assert(ans1 == 3);

    // 測試 2: [2, 2, 1, 1, 1, 2, 2] -> 2
    int num2[7] = {2, 2, 1, 1, 1, 2, 2};
    int ans2 = majorityElement(num2, 7);
    printf("測試 2 通過: [2, 2, 1, 1, 1, 2, 2] 主要元素為: %d (預期: 2)\n", ans2);
    assert(ans2 == 2);

    // 測試 3: [1] -> 1
    int num3[1] = {1};
    int ans3 = majorityElement(num3, 1);
    printf("測試 3 通過: [1] 主要元素為: %d (預期: 1)\n", ans3);
    assert(ans3 == 1);

    printf("\n🎉 p2.c 主要元素 (摩爾投票法) 所有測試案例全數 100%% 通過！\n");
    return 0;
}
