#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 題目 2【輪轉陣列 (Rotate Array - LeetCode #189)】
// 難度：🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥 (一線科技廠/IC設計廠白板三步翻轉經典題)
// 題目說明：給定一個整數陣列 nums，將陣列中的元素向右輪轉 k 個位置，其中 k 是非負數。
// 範例 1：
//    輸入：nums = [1,2,3,4,5,6,7], k = 3
//    輸出：[5,6,7,1,2,3,4]
//    解釋：
//    向右輪轉 1 步: [7,1,2,3,4,5,6]
//    向右輪轉 2 步: [6,7,1,2,3,4,5]
//    向右輪轉 3 步: [5,6,7,1,2,3,4]
// 範例 2：
//    輸入：nums = [-1,-100,3,99], k = 2
//    輸出：[3,99,-1,-100]
// 限制條件：
//    - 1 <= nums.length <= 10^5
//    - -2^31 <= nums[i] <= 2^31 - 1
//    - 0 <= k <= 10^5
//    - ⚠️ 進階挑戰：使用空間複雜度為 O(1) 的「原地 (in-place)」演算法解決此問題！
// ============================================================================
void reverse(int* nums, int left, int right) {
    while (left < right) {
        int temp = nums[left];
        nums[left] = nums[right];
        nums[right] = temp;
        left++;
        right--;
    }
}

void rotate(int* nums, int numsSize, int k) {
    /*
     * 正確解答 (經典三步反轉法 - 時間 O(N)、空間 O(1)):
     * 1. 取模運算：k %= numsSize (若輪轉步數大於等於陣列長度，取餘數避免越界)。
     * 2. 第一步：反轉整個陣列 (0 到 numsSize - 1)。
     * 3. 第二步：反轉前 k 個元素 (0 到 k - 1)。
     * 4. 第三步：反轉後續剩下的元素 (k 到 numsSize - 1)。
     */
    if (nums == NULL || numsSize <= 1) {
        return;
    }
    k %= numsSize;
    if (k == 0) {
        return;
    }

    reverse(nums, 0, numsSize - 1);
    reverse(nums, 0, k - 1);
    reverse(nums, k, numsSize - 1);
}

/* ============================================================================
 * ❌ 原始實作盲點分析 (已保留供複習檢討):
 * ============================================================================
 * void reverse_wrong(int* nums, int left, int right)
 * {
 *     while(left < right)
 *     {
 *         int temp = nums[left];
 *         nums[left] = nums[right];
 *         nums[right] = temp;
 *         // ⚠️ 盲點 1: 漏掉了 left++ 與 right--！
 *         //           雙指針沒有向內收縮，left < right 永遠成立，導致無窮迴圈 (Infinite Loop) 卡死！
 *     }
 * }
 * 
 * void rotate_wrong(int* nums, int numsSize, int k) {
 *     // ⚠️ 盲點 2: 缺少 k %= numsSize 取模保護！
 *     //           當 k >= numsSize (例如陣列長度 2，k = 3)，直接呼叫 reverse(nums, 0, k - 1)
 *     //           會傳入索引 2，導致嚴重的陣列越界非法存取 (Out-of-Bounds Buffer Overflow)！
 *     reverse(nums,0,numsSize-1);
 *     reverse(nums,0,k-1);
 *     reverse(nums,k,numsSize-1);
 * }
 * ============================================================================ */

int main(void) {
    /* 測試測資 1: [1,2,3,4,5,6,7], k = 3 -> [5,6,7,1,2,3,4] */
    int nums1[] = {1, 2, 3, 4, 5, 6, 7};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    rotate(nums1, n1, 3);
    printf("測資 1 結果: ");
    for (int i = 0; i < n1; i++) printf("%d ", nums1[i]);
    printf("(預期: 5 6 7 1 2 3 4)\n");

    /* 測試測資 2: [-1,-100,3,99], k = 2 -> [3,99,-1,-100] */
    int nums2[] = {-1, -100, 3, 99};
    int n2 = sizeof(nums2) / sizeof(nums2[0]);
    rotate(nums2, n2, 2);
    printf("測資 2 結果: ");
    for (int i = 0; i < n2; i++) printf("%d ", nums2[i]);
    printf("(預期: 3 99 -1 -100)\n");

    /* 測試測資 3: k > numsSize 邊界, [1, 2], k = 3 -> [2, 1] */
    int nums3[] = {1, 2};
    int n3 = sizeof(nums3) / sizeof(nums3[0]);
    rotate(nums3, n3, 3);
    printf("測資 3 結果: ");
    for (int i = 0; i < n3; i++) printf("%d ", nums3[i]);
    printf("(預期: 2 1)\n");

    return 0;
}
