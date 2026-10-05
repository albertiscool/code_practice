#include <stdio.h>

// ============================================================================
// 題目 2【搜尋旋轉排序陣列 (Search in Rotated Sorted Array - LeetCode #33)】
// 難度：🟡 一線 IC 設計廠 (聯詠/聯發科) & 系統廠高頻演算法題
// 出題頻率：🔥🔥🔥🔥🔥 (二分搜尋進階變形經典考題)
// 題目說明：整數陣列 nums 按升序排列，陣列中的值互不相同。
//           在傳遞給函式之前，nums 在預先未知的某個下標 k (0 <= k < nums.length) 上進行了旋轉，
//           例如，[0,1,2,4,5,6,7] 在下標 3 處經旋轉後可能變為 [4,5,6,7,0,1,2] 。
//           給你旋轉後的陣列 nums 和一個整數 target，如果 nums 中存在這個目標值 target，
//           則返回它的下標，否則返回 -1 。
// 範例 1：
//    輸入：nums = [4,5,6,7,0,1,2], target = 0
//    輸出：4
// 範例 2：
//    輸入：nums = [4,5,6,7,0,1,2], target = 3
//    輸出：-1
// 範例 3：
//    輸入：nums = [1], target = 0
//    輸出：-1
// 限制條件：
//    - 1 <= nums.length <= 5000
//    - -10^4 <= nums[i] <= 10^4
//    - nums 中的每個值都獨一無二
//    - ⚠️ 你必須設計一個時間複雜度為 O(log n) 的演算法解決此問題！
// ============================================================================

int search(int* nums, int numsSize, int target) {
    // TODO: 請在純白板模式下完成實作 (請勿尋求提示)
    int left = 0;
    int right = numsSize - 1;
    while(left <= right)
    {
        int mid = left + (right - left) / 2;
        if(nums[mid] == target)
        {
            return mid;
        } 
        if(nums[left] <= nums[mid]) // 左半部有序
        {
            // 只有當 target 落在左半部的閉區間 [nums[left], nums[mid]) 內，才往左找
            if(nums[left] <= target && target < nums[mid])
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }
        else // 右半部有序
        {
            // 只有當 target 落在右半部的閉區間 (nums[mid], nums[right]] 內，才往右找
            if(nums[mid] < target && target <= nums[right])
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
    }
    return -1;
}

/*
 * ============================================================================
 * 🔍 使用者原始白板程式碼存檔 (Original Implementation Archive)
 * ============================================================================
 * 【原始完整程式碼】：
 * int search(int* nums, int numsSize, int target) {
 *     int left = 0;
 *     int right = numsSize - 1;
 *     while(left <= right)
 *     {
 *         int mid = left + (right - left) / 2;
 *         if(nums[mid] == target)
 *         {
 *             return mid;
 *         } 
 *         if(nums[left] < nums[mid]) //左邊遞增
 *         {
 *             if(nums[mid] < target)
 *             {
 *                 left = mid + 1;
 *             }
 *             else
 *             {
 *                 right = mid - 1;
 *             }
 *         }
 *         else
 *         {
 *             if(nums[mid] < target)
 *             {
 *                 right = mid - 1;
 *             }
 *             else
 *             {
 *                 left = mid + 1;
 *             }
 *         }
 *     }
 *     return -1;
 * }
 * 
 * 【盲點剖析】：
 *   以 nums = [4, 5, 6, 7, 0, 1, 2], target = 0 為例：
 *   - mid = 3 (數值為 7), nums[left] = 4。
 *   - 雖然左半部 [4, 5, 6, 7] 是遞增的，且 nums[mid] (7) > target (0)，
 *     但 target (0) 根本不在 [4, 5, 6, 7] 裡面，因為它比最左邊的 4 還要小！
 *   - 原寫法僅判斷 nums[mid] > target 就盲目把 right 移到 mid - 1，
 *     直接把含有 target=0 的右半部 [0, 1, 2] 丟棄，導致搜尋失敗回傳 -1！
 * 
 * 【正確判斷法則】：
 *   在有序的那半邊，必須檢查 target 是否「同時大於等於該邊下界，且小於等於該邊上界」：
 *   - 若左半有序：檢查 `nums[left] <= target && target < nums[mid]`。
 *   - 若右半有序：檢查 `nums[mid] < target && target <= nums[right]`。
 *   - 另外，判斷左半有序需用 `<=` 涵蓋 left == mid 的 2 元素邊界情況。
 * ============================================================================
 */

int main(void) {
    /* 測試測資 1: [4,5,6,7,0,1,2], target = 0 -> 4 */
    int nums1[] = {4, 5, 6, 7, 0, 1, 2};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    int ans1 = search(nums1, n1, 0);
    printf("測資 1 (target=0): %d (預期: 4)\n", ans1);

    /* 測試測資 2: [4,5,6,7,0,1,2], target = 3 -> -1 */
    int ans2 = search(nums1, n1, 3);
    printf("測資 2 (target=3): %d (預期: -1)\n", ans2);

    /* 測試測資 3: 單一元素 [1], target = 0 -> -1 */
    int nums3[] = {1};
    int ans3 = search(nums3, 1, 0);
    printf("測資 3 (target=0): %d (預期: -1)\n", ans3);

    /* 測試測資 4: 兩元素旋轉 [3, 1], target = 1 -> 1 */
    int nums4[] = {3, 1};
    int ans4 = search(nums4, 2, 1);
    printf("測資 4 (target=1): %d (預期: 1)\n", ans4);

    return 0;
}
