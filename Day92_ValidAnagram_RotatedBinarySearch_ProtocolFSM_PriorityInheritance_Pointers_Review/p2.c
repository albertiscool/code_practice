#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 題目 2【搜尋旋轉排序陣列 (Search in Rotated Sorted Array - LeetCode #33)】
// 難度：🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥 (聯發科/瑞昱/外商一線廠白板手寫大魔王題)
// 題目說明：整數陣列 nums 按升序排列，陣列中的值「互不相同」。
//           在傳遞給函式之前，nums 在預先未知的某個下標 k (0 <= k < nums.length) 上進行了旋轉，
//           使陣列變為 [nums[k], nums[k+1], ..., nums[n-1], nums[0], nums[1], ..., nums[k-1]]。
//           給你旋轉後的陣列 nums 和一個整數 target，如果 nums 中存在這個目標值 target，
//           則返回它的下標，否則返回 -1。
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
//    - nums 中的每個值都「獨一無二」
//    - 題目保證 nums 在某個點上進行了旋轉
//    - ⚠️ 你必須設計一個時間複雜度為 O(log N) 的演算法！
// ============================================================================

int search(int* nums, int numsSize, int target) {
    /*
     * 正確解答 (旋轉陣列二分搜尋法 - 時間 O(log N)、空間 O(1)):
     * 關鍵性質：旋轉排序陣列切成兩半後，必有「其中一半是嚴格遞增有序」的！
     * 1. 每次先檢查 nums[mid] 是否等於 target。
     * 2. 若 nums[left] <= nums[mid]，代表「左半段」是有序的：
     *    - 檢查 target 是否落在 [nums[left], nums[mid]) 區間內，若是則搜左半邊，否則搜右半邊。
     * 3. 否則代表「右半段」是有序的：
     *    - 檢查 target 是否落在 (nums[mid], nums[right]] 區間內，若是則搜右半邊，否則搜左半邊。
     */
    if (nums == NULL || numsSize == 0) {
        return -1;
    }
    int left = 0;
    int right = numsSize - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            return mid;
        }

        // 判斷左半段是否完全有序
        if (nums[left] <= nums[mid]) {
            if (target >= nums[left] && target < nums[mid]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } 
        // 否則右半段必完全有序
        else {
            if (target > nums[mid] && target <= nums[right]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    return -1;
}

/* ============================================================================
 * ❌ 原始實作盲點分析 (已保留供複習檢討):
 * ============================================================================
 * int search_wrong(int* nums, int numsSize, int target) {
 *     if(nums == NULL) return -1;
 * 
 *     int first = 0;
 *     int last = numsSize - 1;
 * 
 *     // ⚠️ 盲點 1: 迴圈條件寫成 while(last >= 0)，而非 while(first <= last)。
 *     //           當 first 已經超過 last 時，last 只要 >= 0 就會導致無窮迴圈 (Infinite Loop) 卡死！
 *     while(last >= 0)
 *     {
 *         int mid = first + (last - first) / 2;
 * 
 *         // ⚠️ 盲點 2: 這裡比較的是 nums[first] > nums[last]，但二分搜尋的核心是判斷
 *         //           「mid 的左邊或右邊哪一半是有序的」，應該比較 nums[first] <= nums[mid]！
 *         if(nums[first] > nums[last])
 *         {
 *             // ⚠️ 盲點 3: 沒有先檢查 nums[mid] == target；且若 target == nums[first]，
 *             //           程式卻回傳了 mid (兩者根本不在同一個索引位置)！
 *             if(target > nums[first])
 *             {
 *                 first = mid + 1;
 *             }
 *             else if(target < nums[first])
 *             {
 *                 last = mid - 1;
 *             }
 *             else
 *             {
 *                 return mid;
 *             }
 *         }
 *         else
 *         {
 *             if(target > nums[last])
 *             {
 *                 last = mid - 1;
 *             }
 *             else if(target < nums[last])
 *             {
 *                 first = mid + 1;
 *             }
 *             else
 *             {
 *                 return mid;
 *             }
 *         }
 *     }
 *     return -1;
 * }
 * ============================================================================ */

int main(void) {
    /* 測試測資 1: [4,5,6,7,0,1,2], target = 0 -> 4 */
    int nums1[] = {4, 5, 6, 7, 0, 1, 2};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    int ans1 = search(nums1, n1, 0);
    printf("測資 1 結果: %d (預期: 4)\n", ans1);

    /* 測試測資 2: [4,5,6,7,0,1,2], target = 3 -> -1 */
    int nums2[] = {4, 5, 6, 7, 0, 1, 2};
    int n2 = sizeof(nums2) / sizeof(nums2[0]);
    int ans2 = search(nums2, n2, 3);
    printf("測資 2 結果: %d (預期: -1)\n", ans2);

    /* 測試測資 3: [1], target = 0 -> -1 */
    int nums3[] = {1};
    int n3 = sizeof(nums3) / sizeof(nums3[0]);
    int ans3 = search(nums3, n3, 0);
    printf("測資 3 結果: %d (預期: -1)\n", ans3);

    /* 測試測資 4: [3, 1], target = 1 -> 1 */
    int nums4[] = {3, 1};
    int n4 = sizeof(nums4) / sizeof(nums4[0]);
    int ans4 = search(nums4, n4, 1);
    printf("測資 4 結果: %d (預期: 1)\n", ans4);

    return 0;
}
