#include <stdio.h>
#include <assert.h>

/**
 * LeetCode #283: Move Zeroes
 * 
 * 題目描述：
 * 給定一個陣列 nums，編寫一個函式將所有 0 移動到陣列的末尾，同時保持非零元素的相對順序。
 * 
 * 限制條件：
 * - 必須在不複製陣列的情況下「原地（in-place）」對陣列進行修改。
 * - 1 <= numsSize <= 10^4
 * - -2^31 <= nums[i] <= 2^31 - 1
 * 
 * 範例 1:
 *   輸入: nums = [0,1,0,3,12]
 *   輸出: [1,3,12,0,0]
 * 
 * 範例 2:
 *   輸入: nums = [0]
 *   輸出: [0]
 */

void moveZeroes(int* nums, int numsSize) {
    // 請在此處實現純白板程式碼
    int slow = 0;
    int fast = 0;
    while(fast < numsSize)
    {
        if(nums[fast] != 0)
        {
            nums[slow] = nums[fast];
            slow++;
            fast++;
        }
        else
        {
            fast++;
        }
    }

    while(slow < numsSize)
    {
        nums[slow] = 0;
        slow++;
    }
}

int main(void) {
    printf("=== Test P2: Move Zeroes ===\n");
    // Test 1: [0, 1, 0, 3, 12] -> [1, 3, 12, 0, 0]
    int nums1[] = {0, 1, 0, 3, 12};
    moveZeroes(nums1, 5);
    int expected1[] = {1, 3, 12, 0, 0};
    for (int i = 0; i < 5; i++) {
        assert(nums1[i] == expected1[i]);
    }
    printf("Test 1 Passed: [0, 1, 0, 3, 12] -> [1, 3, 12, 0, 0]\n");

    // Test 2: [0] -> [0]
    int nums2[] = {0};
    moveZeroes(nums2, 1);
    assert(nums2[0] == 0);
    printf("Test 2 Passed: [0] -> [0]\n");

    // Test 3: [1, 2, 3] -> [1, 2, 3]
    int nums3[] = {1, 2, 3};
    moveZeroes(nums3, 3);
    assert(nums3[0] == 1 && nums3[1] == 2 && nums3[2] == 3);
    printf("Test 3 Passed: [1, 2, 3] -> [1, 2, 3]\n");

    // Test 4: [0, 0, 0] -> [0, 0, 0]
    int nums4[] = {0, 0, 0};
    moveZeroes(nums4, 3);
    assert(nums4[0] == 0 && nums4[1] == 0 && nums4[2] == 0);
    printf("Test 4 Passed: [0, 0, 0] -> [0, 0, 0]\n");

    printf("All tests passed!\n");
    return 0;
}

/* =========================================================================
 * 🚩 【Albert 白板實戰複盤與陣列雙指標原地滾動深度解析】
 * =========================================================================
 * 
 * 🏆 LeetCode 提交結果：
 * - 狀態：Accepted (75/75 測試案例全數通過！)
 * - 執行時間：0 ms (Beats 100.00%)
 * - 記憶體：20.4 MB
 * - Submission ID: 2169088125
 * 
 * 💡 邏輯亮點（滿分白板表現）：
 * 1. 零提示下純白板秒殺！經典快慢指標（Two Pointers）原地覆寫（In-place Overwrite）行雲流水！
 * 2. 第一階段：fast 探路，將非零元素連續往前滾動填入 slow；
 * 3. 第二階段：slow 往後遍歷剩餘長度，批次補零！
 * 4. 時間複雜度 O(N)，額外空間 O(1)，完全沒有開任何額外陣列。
 * 
 * 🔬 面試官追問進階優化：能不能「單次遍歷（One Pass）」利用 Swap 解決？
 * -------------------------------------------------------------------------
 * 目前解法是「兩次遍歷（第一次移非零，第二次補零）」。
 * 面試官常問：「能不能只跑一次迴圈，不需要第二個補零的 while？」
 * 
 * 💡 單次遍歷 Swap 寫法：
 * void moveZeroesOnePass(int* nums, int numsSize) {
 *     int slow = 0;
 *     for (int fast = 0; fast < numsSize; fast++) {
 *         if (nums[fast] != 0) {
 *             if (slow != fast) { // 當 slow != fast 時才交換，避免自己跟自己換
 *                 int temp = nums[slow];
 *                 nums[slow] = nums[fast];
 *                 nums[fast] = temp;
 *             }
 *             slow++;
 *         }
 *     }
 * }
 * - 原理：只要 fast 指標找到非零數字，就直接與 slow 指標所指的 0 交換！
 * - 交換後，0 自動滾動到後面，非零元素自動來到前面，一次迴圈搞定！
 * ========================================================================= */
