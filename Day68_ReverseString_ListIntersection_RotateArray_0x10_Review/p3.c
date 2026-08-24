#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 3【旋轉陣列 (Rotate Array - LeetCode #189)】
// 難度：🟡 Medium (中等題) | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：三次反轉法 (Three Reversals Trick) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

// 輔助反轉函式：將 nums[left ~ right] 區間反轉
void reverse(int* nums, int left, int right) {
    while (left < right) {
        int temp = nums[left];
        nums[left] = nums[right];
        nums[right] = temp;
        left++;
        right--;
    }
}

void rotate(int* nums, int numsSize, int k)
{
    // 🌟 防呆第一步：取模計算！防止 k 大於陣列長度 (例如長度為 3, k=10 等同於 k=1)
    k = k % numsSize;

    // 三次反轉法 (Three Reversals Trick)
    // 1. 反轉整個陣列 [0 ~ numsSize - 1]
    reverse(nums, 0, numsSize - 1);

    // 2. 反轉前 k 個元素 [0 ~ k - 1]
    reverse(nums, 0, k - 1);

    // 3. 反轉剩餘元素 [k ~ numsSize - 1]
    reverse(nums, k, numsSize - 1);
}

int main()
{
    // 測試 1: [1, 2, 3, 4, 5, 6, 7], k = 3 -> [5, 6, 7, 1, 2, 3, 4]
    int nums1[7] = {1, 2, 3, 4, 5, 6, 7};
    rotate(nums1, 7, 3);
    printf("測試 1 旋轉後: [%d, %d, %d, %d, %d, %d, %d]\n",
           nums1[0], nums1[1], nums1[2], nums1[3], nums1[4], nums1[5], nums1[6]);
    assert(nums1[0] == 5 && nums1[1] == 6 && nums1[2] == 7 && nums1[3] == 1);

    // 測試 2: [-1, -100, 3, 99], k = 2 -> [3, 99, -1, -100]
    int nums2[4] = {-1, -100, 3, 99};
    rotate(nums2, 4, 2);
    assert(nums2[0] == 3 && nums2[1] == 99 && nums2[2] == -1 && nums2[3] == -100);

    // 測試 3: 大 k 測試 (k > numsSize): [1, 2], k = 3 -> [2, 1]
    int nums3[2] = {1, 2};
    rotate(nums3, 2, 3);
    assert(nums3[0] == 2 && nums3[1] == 1);

    printf("\n🎉 p3.c 旋轉陣列 所有測試案例全數 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（三次反轉法核心 100% 正確！）：
void rotate_original(int* nums, int numsSize, int k)
{
    // 💡 唯一微小隱藏陷阱：
    // 忘記加 k = k % numsSize;
    // 若 k 大於陣列長度（例如 numsSize=2, k=3），k-1 就會變成 2，
    // reverse(nums, 0, 2) 會存取越界引發 Segmentation Fault！
    // 只要補上 k = k % numsSize 即可達到 100 分防呆！
    reverse(nums, 0, numsSize - 1);
    reverse(nums, 0, k - 1);
    reverse(nums, k, numsSize - 1);
}
*/
