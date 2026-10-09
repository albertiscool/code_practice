#include <stdio.h>
#include <assert.h>

/**
 * LeetCode #136: Single Number
 * 
 * 題目描述：
 * 給定一個非空的整數陣列 nums，除了某個元素只出現一次以外，其餘每個元素均出現兩次。
 * 請找出那個只出現了一次的元素。
 * 
 * 限制條件：
 * - 你必須設計並實現一個線性時間複雜度 O(N) 的演算法。
 * - 且只能使用常數空間 O(1) 的額外空間。
 * - 1 <= numsSize <= 3 * 10^4
 * - -3 * 10^4 <= nums[i] <= 3 * 10^4
 * 
 * 範例 1:
 *   輸入: nums = [2,2,1]
 *   輸出: 1
 * 
 * 範例 2:
 *   輸入: nums = [4,1,2,1,2]
 *   輸出: 4
 * 
 * 範例 3:
 *   輸入: nums = [1]
 *   輸出: 1
 * 
 * 面試深意：
 * IC 設計廠（群聯 Phison、瑞昱 Realtek）高頻白板題，考驗在嚴格 O(1) 空間限制下的解法。
 */

int singleNumber(int* nums, int numsSize) {
    // 請在此處實現純白板程式碼
    if(numsSize == 1)
    {
        return nums[0];
    }

    int result = nums[0];
    for(int i = 1; i < numsSize; i++)
    {
        result = result ^ nums[i];
    }

    return result;
}

int main(void) {
    printf("=== Test P1: Single Number ===\n");
    // Test 1: [2, 2, 1] -> 1
    int nums1[] = {2, 2, 1};
    assert(singleNumber(nums1, 3) == 1);
    printf("Test 1 Passed: [2, 2, 1] -> 1\n");

    // Test 2: [4, 1, 2, 1, 2] -> 4
    int nums2[] = {4, 1, 2, 1, 2};
    assert(singleNumber(nums2, 5) == 4);
    printf("Test 2 Passed: [4, 1, 2, 1, 2] -> 4\n");

    // Test 3: [1] -> 1
    int nums3[] = {1};
    assert(singleNumber(nums3, 1) == 1);
    printf("Test 3 Passed: [1] -> 1\n");

    // Test 4: negative numbers [-2, -2, -5] -> -5
    int nums4[] = {-2, -2, -5};
    assert(singleNumber(nums4, 3) == -5);
    printf("Test 4 Passed: [-2, -2, -5] -> -5\n");

    printf("All tests passed!\n");
    return 0;
}

/* =========================================================================
 * 🚩 【Albert 白板實戰複盤與 IC 設計廠位元運算深入解析】
 * =========================================================================
 * 
 * 🏆 LeetCode 提交結果：
 * - 狀態：Accepted (61/61 測試案例全數通過！)
 * - 執行時間：0 ms (Beats 100.00%)
 * - 記憶體：10.2 MB
 * - Submission ID: 2167254428
 * 
 * 💡 邏輯亮點（滿分白板表現）：
 * 1. 在零提示條件下，一眼辨識出 XOR 互斥或的「對偶消除律」，寫出精準的 O(N) 時間、O(1) 空間最優解！
 * 2. 善用邊界保護，程式碼乾淨俐落無贅餘。
 * 
 * 🔬 數學與硬體位元原理深度解析：
 * -------------------------------------------------------------------------
 * XOR (^) 運算的三大核心性質：
 * 1. 自反性（自我消除）：a ^ a = 0
 * 2. 單位元素性：a ^ 0 = a
 * 3. 交換律與結合律：a ^ b ^ a = (a ^ a) ^ b = 0 ^ b = b
 * 
 * 在本題中，陣列中所有成對出現的數字進行連續 XOR 後，無論它們出現的順序為何，
 * 成對的二進位 bit 必兩兩相消抵銷為 0，最終殘留的數值必然是那唯一孤立的單一數字！
 * 
 * 💡 程式碼極簡化寫法（面試加分點）：
 * -------------------------------------------------------------------------
 * 若將 result 初始化為 0，從 index 0 開始遍歷：
 * int singleNumber(int* nums, int numsSize) {
 *     int result = 0;
 *     for (int i = 0; i < numsSize; i++) {
 *         result ^= nums[i];
 *     }
 *     return result;
 * }
 * 因為 `0 ^ nums[0] = nums[0]`，即使 numsSize == 1 也能天然完美相容，省去額外的 if 判斷！
 * ========================================================================= */
