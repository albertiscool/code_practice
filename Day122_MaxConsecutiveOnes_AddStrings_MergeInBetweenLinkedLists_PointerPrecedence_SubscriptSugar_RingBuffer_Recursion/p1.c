#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #485 - Max Consecutive Ones
 * 
 * 題目描述：
 * 給定一個二進位陣列 nums，請計算並回傳陣列中「連續 1 的最大個數」。
 * 
 * 範例 1：
 * 輸入：nums = [1, 1, 0, 1, 1, 1]
 * 輸出：3
 * 解釋：開頭有 2 個連續的 1，結尾有 3 個連續的 1，因此最大連續個數為 3。
 * 
 * 範例 2：
 * 輸入：nums = [1, 0, 1, 1, 0, 1]
 * 輸出：2
 * 
 * 限制條件：
 * 1 <= nums.length <= 10^5
 * nums[i] 不是 0 就是 1
 */

/* =========================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * -------------------------------------------------------------------------
 * int findMaxConsecutiveOnes(int* nums, int numsSize) {
 *     int slow = 0;
 *     int fast = 0;
 *     int count = 0;
 *     int max = 0;
 *     while(fast < numsSize)
 *     {
 *         while(nums[slow] == 0)
 *         {
 *             slow++;
 *             fast++;
 *         }
 * 
 *         while((nums[fast] == 1) && (fast < numsSize))
 *         {
 *             fast++;
 *             count++;
 *         }
 * 
 *         if(count > max)
 *         {
 *             max = count;
 *         }
 *         count = 0;
 *         slow = fast;
 *     }
 *     return max;
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點剖析】：
 * 1. 越界存取與短路求值 (Short-circuit Evaluation) 順序陷阱：
 *    - 盲點 A：`while(nums[slow] == 0)` 缺少 `slow < numsSize` 邊界檢查。
 *      若陣列全為 0（例如 [0, 0]），`slow` 會一路加到超越陣列長度，觸發 Heap-buffer-overflow！
 *    - 盲點 B：`while((nums[fast] == 1) && (fast < numsSize))` 的求值順序顛倒！
 *      在 C 語言中，`&&` 是由左至右求值。當 `fast == numsSize` 時，
 *      程式會先執行左邊的 `nums[fast] == 1`（此時已陣列越界！），之後才看右邊。
 *      正確寫法必須「邊界檢查放左側」：`while (fast < numsSize && nums[fast] == 1)`。
 * 2. 架構簡化小亮點（單層計數）：
 *    - 本題其實可以用最純粹的單層 for 迴圈：遇到 1 累加並更新 max，遇到 0 歸零。
 * =========================================================================
 */

int findMaxConsecutiveOnes(int* nums, int numsSize) {
    // 方案 A：修復使用者雙指標邊界
    int slow = 0;
    int fast = 0;
    int count = 0;
    int max = 0;

    while (fast < numsSize) {
        // 修復 1：增加邊界檢查 slow < numsSize
        while (slow < numsSize && nums[slow] == 0) {
            slow++;
            fast++;
        }

        // 修復 2：邊界檢查置於左側利用短路求值保護
        while (fast < numsSize && nums[fast] == 1) {
            fast++;
            count++;
        }

        if (count > max) {
            max = count;
        }
        count = 0;
        slow = fast;
    }
    return max;
}

int main() {
    int nums1[] = {1, 1, 0, 1, 1, 1};
    printf("Test 1: %d (Expected: 3)\n", findMaxConsecutiveOnes(nums1, 6));

    int nums2[] = {1, 0, 1, 1, 0, 1};
    printf("Test 2: %d (Expected: 2)\n", findMaxConsecutiveOnes(nums2, 6));

    return 0;
}
