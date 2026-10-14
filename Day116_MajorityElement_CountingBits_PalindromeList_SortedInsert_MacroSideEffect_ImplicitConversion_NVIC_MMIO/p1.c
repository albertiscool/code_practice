#include <stdio.h>
#include <assert.h>

/**
 * LeetCode #169: Majority Element
 * 
 * 題目描述：
 * 給定一個大小為 n 的整數陣列 nums，請找出其中的「多數元素 (Majority Element)」。
 * 多數元素是指在陣列中出現次數「大於 ⌊ n / 2 ⌋」的元素。
 * 
 * 假設：
 * - 陣列為非空，且給定的陣列保證一定存在多數元素。
 * 
 * 限制條件：
 * - n == numsSize
 * - 1 <= n <= 5 * 10^4
 * - -10^9 <= nums[i] <= 10^9
 * - 進階挑戰：你能否設計出時間複雜度為 O(N) 且額外空間複雜度為 O(1) 的演算法？
 * 
 * 範例 1:
 *   輸入: nums = [3,2,3]
 *   輸出: 3
 * 
 * 範例 2:
 *   輸入: nums = [2,2,1,1,1,2,2]
 *   輸出: 2
 */

int majorityElement(int* nums, int numsSize) {
    // 請在此處實現純白板程式碼
    int curr = nums[0];
    int count = 1;
    for(int i = 1; i < numsSize; i++)
    {
        if(nums[i] == curr)
        {
            count++;
        }
        else
        {
            count--;
            if(count == 0)
            {
                curr = nums[i];
                count = 1;
            }
        }
    }
    return curr;
}

/*
 * =====================================================================================
 * 【面試亮點剖析 & 演算法極限思維】 (LeetCode #169: Majority Element)
 * =====================================================================================
 * 滿分解法亮點 (Boyer-Moore 摩爾投票演算法)：
 * 1. 極致的 O(1) 空間複雜度 (淘汰 Hash Table 與排序)：
 *    絕大多數沒受過演算法訓練的候選人，看到這題會直接宣告 HashMap 或使用 qsort (O(N log N))。
 *    你在白板上直接端出 O(N) 時間、O(1) 空間的投票法，展現出高階資深工程師的深度！
 * 
 * 2. 核心數學本質 (極限同歸於盡 / 抵消原理)：
 *    - 核心前提：多數元素出現次數「嚴格大於」總長度的一半 (> n / 2)。
 *    - 演算法思維：想像成大亂鬥，每個多數元素與任何一個「不同」的非多數元素一對一互換同歸於盡。
 *      因為多數元素的數量比所有其他所有數字加起來還要多，無論其他數字怎麼聯合抵消，
 *      最後存活下來的必然是多數元素！
 * =====================================================================================
 */

int main(void) {
    printf("=== Test P1: Majority Element ===\n");

    // Test 1: [3, 2, 3] -> 3
    int nums1[] = {3, 2, 3};
    assert(majorityElement(nums1, 3) == 3);
    printf("Test 1 Passed: [3, 2, 3] -> 3\n");

    // Test 2: [2, 2, 1, 1, 1, 2, 2] -> 2
    int nums2[] = {2, 2, 1, 1, 1, 2, 2};
    assert(majorityElement(nums2, 7) == 2);
    printf("Test 2 Passed: [2, 2, 1, 1, 1, 2, 2] -> 2\n");

    // Test 3: [1] -> 1
    int nums3[] = {1};
    assert(majorityElement(nums3, 1) == 1);
    printf("Test 3 Passed: [1] -> 1\n");

    // Test 4: [6, 5, 5] -> 5
    int nums4[] = {6, 5, 5};
    assert(majorityElement(nums4, 3) == 5);
    printf("Test 4 Passed: [6, 5, 5] -> 5\n");

    printf("All P1 Local Tests Passed!\n");
    return 0;
}
