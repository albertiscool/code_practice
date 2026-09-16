#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 題目 1【最大子陣列和 (Maximum Subarray - LeetCode #53)】
// 難度：🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥 (一線科技廠/IC設計廠白板手寫必考題)
// 題目說明：給你一個整數陣列 nums，請你找出一個具有最大和的「連續子陣列」（子陣列最少包含一個元素），
//           並返回其最大和。
// 範例 1：
//    輸入：nums = [-2,1,-3,4,-1,2,1,-5,4]
//    輸出：6
//    解釋：連續子陣列 [4,-1,2,1] 的和最大，為 6。
// 範例 2：
//    輸入：nums = [1]
//    輸出：1
// 範例 3：
//    輸入：nums = [5,4,-1,7,8]
//    輸出：23
// 限制條件：
//    - 1 <= nums.length <= 10^5
//    - -10^4 <= nums[i] <= 10^4
//    - 進階挑戰：你能否設計出時間複雜度為 O(N) 且空間複雜度為 O(1) 的 Kadane 演算法？
// ============================================================================

int maxSubArray(int* nums, int numsSize) {
    /* 
     * 正確解答 (Kadane 演算法 - 動態規劃 / 貪婪思維):
     * 1. 初始 max_sum 必須設為 nums[0]，不能設為 0 (避免全負數時回傳 0)。
     * 2. 遍歷陣列時，curr_sum 累加當前元素，並隨時更新最大值 max_sum。
     * 3. 若 curr_sum < 0，代表此段累積和為負數，對後續的子陣列只有負面拖累效果，應歸零重置。
     */
    int max_sum = nums[0];
    int curr_sum = 0;
    
    for (int i = 0; i < numsSize; i++) {
        curr_sum += nums[i];
        if (curr_sum > max_sum) {
            max_sum = curr_sum;
        }
        if (curr_sum < 0) {
            curr_sum = 0;
        }
    }
    return max_sum;
}

/* ============================================================================
 * ❌ 原始實作盲點分析 (已保留供複習檢討):
 * ============================================================================
 * int maxSubArray_wrong(int* nums, int numsSize) {
 *     int max_sum = 0;    // ⚠️ 盲點 1: 初始值設為 0。若陣列全為負數 (如 [-3,-2,-5,-1])，
 *     int curr_sum = 0;   //           最大值應該是 -1，但因 max_sum=0，最終會錯誤回傳 0。
 *     for(int i = 0; i < numsSize; i++)
 *     {
 *         curr_sum += nums[i];
 *         if(curr_sum > max_sum)
 *         {
 *             max_sum = curr_sum;
 *         }
 *         else if(curr_sum < 0) // ⚠️ 盲點 2: 使用 else if 會使得更新與歸零互斥；
 *         {                     //           且全負數時 curr_sum 永遠不大於 0，只會一直進到歸零。
 *             curr_sum = 0;
 *         }
 *     }
 *     return max_sum;
 * }
 * ============================================================================ */

int main(void) {
    /* 測試測資 1: [-2,1,-3,4,-1,2,1,-5,4] -> 6 */
    int nums1[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    int ans1 = maxSubArray(nums1, n1);
    printf("測資 1 結果: %d (預期: 6)\n", ans1);

    /* 測試測資 2: [1] -> 1 */
    int nums2[] = {1};
    int n2 = sizeof(nums2) / sizeof(nums2[0]);
    int ans2 = maxSubArray(nums2, n2);
    printf("測資 2 結果: %d (預期: 1)\n", ans2);

    /* 測試測資 3: 全負數 [-3, -2, -5, -1] -> -1 */
    int nums3[] = {-3, -2, -5, -1};
    int n3 = sizeof(nums3) / sizeof(nums3[0]);
    int ans3 = maxSubArray(nums3, n3);
    printf("測資 3 結果: %d (預期: -1)\n", ans3);

    return 0;
}
