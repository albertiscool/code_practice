#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 3【缺失的數字 (Missing Number - LeetCode #268)】
// 難度：🟢 Easy
// 寫法 1：高斯等差級數總和法 ($O(N)$ 時間, $O(1)$ 空間)
// ============================================================================
int missingNumber(int* nums, int numsSize)
{
    // 1. 計算理論上 0 ~ n 的完整總和：n * (n + 1) / 2
    int sum = numsSize * (numsSize + 1) / 2;

    // 2. 扣掉陣列中實際出現的每一個數字
    for (int i = 0; i < numsSize; i++)
    {
        sum -= nums[i];
    }

    // 3. 剩下的差額就是唯一缺失的數字！
    return sum;
}

// ============================================================================
// 寫法 2：XOR 互斥或抵消法 (徹底防禦大數據溢位)
// ============================================================================
int missingNumber_XOR(int* nums, int numsSize)
{
    int res = numsSize; // 初始化為 n
    for (int i = 0; i < numsSize; i++)
    {
        // 索引 i (0 ~ n-1) 與陣列數字 nums[i] 連做 XOR
        // 成對出現的數字會自動抵消 (x ^ x = 0)，最後留下的即為缺號！
        res ^= i ^ nums[i];
    }
    return res;
}

int main()
{
    // 測試 1: [3, 0, 1] -> 缺 2
    int nums1[3] = {3, 0, 1};
    int ans1 = missingNumber(nums1, 3);
    printf("測試 1 缺少的數字: %d (預期: 2)\n", ans1);
    assert(ans1 == 2);
    assert(missingNumber_XOR(nums1, 3) == 2);

    // 測試 2: [0, 1] -> 缺 2
    int nums2[2] = {0, 1};
    int ans2 = missingNumber(nums2, 2);
    printf("測試 2 缺少的數字: %d (預期: 2)\n", ans2);
    assert(ans2 == 2);
    assert(missingNumber_XOR(nums2, 2) == 2);

    // 測試 3: [9,6,4,2,3,5,7,0,1] -> 缺 8
    int nums3[9] = {9, 6, 4, 2, 3, 5, 7, 0, 1};
    int ans3 = missingNumber(nums3, 9);
    printf("測試 3 缺少的數字: %d (預期: 8)\n", ans3);
    assert(ans3 == 8);
    assert(missingNumber_XOR(nums3, 9) == 8);

    printf("\n🎉 p3.c 缺失的數字 所有測試案例全數 100%% 通過！\n");
    return 0;
}


void SWAP32(x)
{
    x = (x & 0xff000000) >> 24 | (x & 0x00ff0000) >> 8 | (x & 0x0000ff00) << 8 | (x & 0x000000ff) << 24;
}