#include <stdio.h>
#include <assert.h>

/**
 * LeetCode #268: Missing Number
 * 
 * 題目描述：
 * 給定一個包含 [0, n] 中 n 個數的陣列 nums，請找出 [0, n] 這個範圍內沒有出現在陣列中的那個整數。
 * 
 * 限制條件：
 * - n == numsSize
 * - 1 <= n <= 10^4
 * - 0 <= nums[i] <= n
 * - nums 中的所有數字都「互不相同」
 * - 進階挑戰：你能否設計一個線性時間複雜度 O(N) 且使用常數 O(1) 額外空間的演算法？
 * 
 * 範例 1:
 *   輸入: nums = [3,0,1]
 *   輸出: 2
 *   解釋: n = 3，範圍是 [0,3]，其中 2 沒有出現在陣列中。
 * 
 * 範例 2:
 *   輸入: nums = [0,1]
 *   輸出: 2
 *   解釋: n = 2，範圍是 [0,2]，其中 2 沒有出現在陣列中。
 * 
 * 範例 3:
 *   輸入: nums = [9,6,4,2,3,5,7,0,1]
 *   輸出: 8
 *   解釋: n = 9，範圍是 [0,9]，其中 8 沒有出現在陣列中。
 */

int missingNumber(int* nums, int numsSize) {
    // 請在此處實現純白板程式碼
    int sum = (0 + numsSize) * (numsSize + 1) / 2;
    for(int i = 0; i < numsSize; i++)
    {
        sum -= nums[i];
    }
    return sum;
}

int main(void) {
    printf("=== Test P1: Missing Number ===\n");
    // Test 1: [3, 0, 1] -> 2
    int nums1[] = {3, 0, 1};
    assert(missingNumber(nums1, 3) == 2);
    printf("Test 1 Passed: [3, 0, 1] -> 2\n");

    // Test 2: [0, 1] -> 2
    int nums2[] = {0, 1};
    assert(missingNumber(nums2, 2) == 2);
    printf("Test 2 Passed: [0, 1] -> 2\n");

    // Test 3: [9,6,4,2,3,5,7,0,1] -> 8
    int nums3[] = {9, 6, 4, 2, 3, 5, 7, 0, 1};
    assert(missingNumber(nums3, 9) == 8);
    printf("Test 3 Passed: [9,6,4,2,3,5,7,0,1] -> 8\n");

    // Test 4: [0] -> 1
    int nums4[] = {0};
    assert(missingNumber(nums4, 1) == 1);
    printf("Test 4 Passed: [0] -> 1\n");

    // Test 5: [1] -> 0
    int nums5[] = {1};
    assert(missingNumber(nums5, 1) == 0);
    printf("Test 5 Passed: [1] -> 0\n");

    printf("All tests passed!\n");
    return 0;
}

/* =========================================================================
 * 🚩 【Albert 白板實戰複盤與 IC 設計廠數值/位元運算深入解析】
 * =========================================================================
 * 
 * 🏆 LeetCode 提交結果：
 * - 狀態：Accepted (122/122 測試案例全數通過！)
 * - 執行時間：0 ms (Beats 100.00%)
 * - 記憶體：9.6 MB
 * - Submission ID: 2169054152
 * 
 * 💡 邏輯亮點（滿分白板表現）：
 * 1. 零提示下直接給出高斯等差數列求和公式 `sum = n * (n + 1) / 2`，數學直覺敏銳！
 * 2. 時間複雜度嚴格 O(N)，額外空間 O(1)，程式碼簡潔高效。
 * 
 * 🔬 面試官高頻追問：整數溢位（Integer Overflow）與 XOR 位元運算解
 * -------------------------------------------------------------------------
 * 面試官必問：「高斯求和很漂亮，但若 n 高達 10^5 或 10^6，`n * (n + 1)` 會超過 32-bit int
 * 的上限 (2.14 * 10^9) 導致溢位！在嚴格的韌體安全標準下，你能否用【位元運算】解決這個問題？」
 * 
 * 💡 XOR 位元消去解法（零溢位風險）：
 * 利用 `a ^ a = 0` 與 `a ^ 0 = a`：
 * 我們將 [0, n] 的所有數字與陣列中的所有元素全部進行 XOR：
 * int missingNumberXOR(int* nums, int numsSize) {
 *     int xor_sum = numsSize; // 先放入 n
 *     for (int i = 0; i < numsSize; i++) {
 *         xor_sum ^= i ^ nums[i]; // i 與 nums[i] 成對消去
 *     }
 *     return xor_sum;
 * }
 * 出現在陣列中的數字會與索引 i 剛好兩兩相消，最後留下的就是未成對的「缺失數字」！
 * 此寫法為純位元運算，完全不會有加法或乘法溢位的任何風險！
 * ========================================================================= */
