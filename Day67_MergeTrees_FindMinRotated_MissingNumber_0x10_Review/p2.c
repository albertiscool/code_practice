#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 2【尋找旋轉排序陣列中的最小值 (Find Minimum in Rotated Sorted Array - LeetCode #153)】
// 難度：🟡 Medium (中等題)
// 演算法：二分搜尋法 (Binary Search) - $O(\log N)$ 時間, $O(1)$ 空間
// ============================================================================
int findMin(int* nums, int numsSize)
{
    int low = 0;
    int high = numsSize - 1;

    // 當 low < high 時持續二分對半縮小範圍
    while (low < high)
    {
        // 防溢位計算中間點 mid
        int mid = low + (high - low) / 2;

        // 🌟 核心比較觀念：將 nums[mid] 與右邊界 nums[high] 比較
        // 情況 1：nums[mid] > nums[high] (例如 [4, 5, 6, 7, 0, 1, 2]，mid=7, high=2)
        // 代表左半邊包含 mid 全都是較大的遞增序列，最小值必在「右半邊」 (不包含 mid)！
        if (nums[mid] > nums[high])
        {
            low = mid + 1;
        }
        // 情況 2：nums[mid] <= nums[high] (例如 [4, 5, 6, 7, 0, 1, 2]，mid=0, high=2)
        // 代表右半邊為正常遞增，最小值必在「mid 本身或左半邊」！
        else
        {
            high = mid; // 不能寫 mid - 1，因為 mid 本身可能就是最小值！
        }
    }

    // 當 low == high 時，必定收斂在最小值的位置上！
    return nums[low];
}

int main()
{
    // 測試 1: [3, 4, 5, 1, 2] -> 最小值 1
    int nums1[5] = {3, 4, 5, 1, 2};
    int ans1 = findMin(nums1, 5);
    printf("測試 1 最小值: %d (預期: 1)\n", ans1);
    assert(ans1 == 1);

    // 測試 2: [4, 5, 6, 7, 0, 1, 2] -> 最小值 0
    int nums2[7] = {4, 5, 6, 7, 0, 1, 2};
    int ans2 = findMin(nums2, 7);
    printf("測試 2 最小值: %d (預期: 0)\n", ans2);
    assert(ans2 == 0);

    // 測試 3: [11, 13, 15, 17] (未旋轉/旋轉 0 次) -> 最小值 11
    int nums3[4] = {11, 13, 15, 17};
    int ans3 = findMin(nums3, 4);
    printf("測試 3 最小值: %d (預期: 11)\n", ans3);
    assert(ans3 == 11);

    // 測試 4: 單一元素 [2] -> 最小值 2
    int nums4[1] = {2};
    int ans4 = findMin(nums4, 1);
    assert(ans4 == 2);

    printf("\n🎉 p2.c 尋找旋轉陣列最小值 所有測試案例全數 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（防溢位 mid 計算 100% 正確！）：
int findMin_original(int* nums, int numsSize)
{
    int low = 0;
    int high = numsSize - 1;
    while(low <= high)
    {
        int mid = low + (high - low) / 2;
        
        // 💡 盲點 1：將 nums[mid] 同時與 low 和 high 比較（if nums[mid] > nums[low] && nums[mid] > nums[high]）
        // 條件太過複雜且極易漏掉邊界狀況（例如未旋轉陣列 [11, 13, 15, 17]）。
        // 🌟 解題神技：只需要把 nums[mid] 跟最右邊的 nums[high] 比較即可！
        // 如果 nums[mid] > nums[high] -> 代表最小值在右半邊 (low = mid + 1)
        // 否則 -> 最小值在左半邊或就是 mid (high = mid)
        
        // 💡 盲點 2（區域變數作用域 Scope）：
        // 原版 int result = nums[mid]; 宣告在 else if 的大括號內部，
        // 導致最後第 38 行 return result; 讀不到變數 (變數未宣告錯誤)！
    }
    return nums[low];
}
*/
