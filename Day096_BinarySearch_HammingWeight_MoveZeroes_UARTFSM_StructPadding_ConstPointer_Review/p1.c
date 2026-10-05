#include <stdio.h>

// ============================================================================
// 題目 1【二分搜尋法 (Binary Search - LeetCode #704)】
// 難度：🟢 系統廠高頻基本題 | 出題頻率：🔥🔥🔥🔥🔥 (廣達/緯創/台達電/和碩白板手寫必備)
// 題目說明：給定一個升序排序的整數陣列 nums 和一個目標值 target，
//           請寫一個函式搜尋 nums 中的 target，如果目標值存在返回下標，否則返回 -1。
// 範例 1：
//    輸入：nums = [-1,0,3,5,9,12], target = 9
//    輸出：4
//    解釋：9 出現在 nums 中並且下標為 4
// 範例 2：
//    輸入：nums = [-1,0,3,5,9,12], target = 2
//    輸出：-1
//    解釋：2 不存在 nums 中因此返回 -1
// 限制條件：
//    - 1 <= nums.length <= 10^4
//    - -10^4 < nums[i], target < 10^4
//    - nums 中的所有元素是不重複且升序排序的
//    - ⚠️ 時間複雜度必須為 O(log n)，請注意防範整數溢位 (Integer Overflow)！
// ============================================================================

int search(int* nums, int numsSize, int target) {
    // TODO: 請在純白板模式下完成實作 (請勿尋求提示)
    if(nums == NULL)
    {
        return -1;
    }
    if(numsSize == 0)
    {
        return -1;
    }
    int left = 0;
    int right = numsSize - 1;
    while(left <= right)
    {
        int mid = left + (right - left) / 2;
        if(nums[mid] == target)
        {
            return mid;
        }
        else if(nums[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return -1;
}

/*
 * ============================================================================
 * 🔍 原始盲點檢討 (Mistake Analysis Archive)
 * ============================================================================
 * 【原始寫法】：
 *     else if(nums[mid] < target)
 *     {
 *         right = mid - 1;   // ⚠️ 盲點：搜尋方向顛倒！
 *     }
 *     else
 *     {
 *         left = mid + 1;
 *     }
 * 
 * 【邏輯盲點剖析】：
 *   - 陣列為「升序排序 (Ascending Order, 小 -> 大)」。
 *   - 當 nums[mid] < target 時，代表 target 數值比中間元素大，因此 target 必然位於「右半邊區間」！
 *     正確操作應該是將搜尋起點往右推：`left = mid + 1`。
 *     若寫成 `right = mid - 1` 反而跑到更小的左半區間搜尋，導致永遠找不到目標值 (回傳 -1)！
 *   - 反之，當 nums[mid] > target 時，target 位於「左半邊區間」，才需要將右界收縮：`right = mid - 1`。
 * 
 * 【修正方式】：
 *     else if (nums[mid] < target) {
 *         left = mid + 1;
 *     } else {
 *         right = mid - 1;
 *     }
 * ============================================================================
 */

int main(void) {
    /* 測試測資 1: [-1,0,3,5,9,12], target = 9 -> 4 */
    int nums1[] = {-1, 0, 3, 5, 9, 12};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    int ans1 = search(nums1, n1, 9);
    printf("測資 1 (target=9) : %d (預期: 4)\n", ans1);

    /* 測試測資 2: [-1,0,3,5,9,12], target = 2 -> -1 */
    int ans2 = search(nums1, n1, 2);
    printf("測資 2 (target=2) : %d (預期: -1)\n", ans2);

    /* 測試測資 3: 單一元素 [5], target = 5 -> 0 */
    int nums3[] = {5};
    int ans3 = search(nums3, 1, 5);
    printf("測資 3 (target=5) : %d (預期: 0)\n", ans3);

    /* 測試測資 4: 單一元素 [5], target = -5 -> -1 */
    int ans4 = search(nums3, 1, -5);
    printf("測資 4 (target=-5): %d (預期: -1)\n", ans4);

    return 0;
}
