#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/**
 * LeetCode #922: Sort Array By Parity II
 * 
 * 題目描述：
 * 給定一個非負整數陣列 nums，其中「恰好一半」的整數是奇數，另一半是偶數。
 * 請對陣列進行重新排序，使得：
 * - 當 nums[i] 為偶數時，索引 i 必須為偶數；
 * - 當 nums[i] 為奇數時，索引 i 必須為奇數。
 * 
 * 請返回滿足上述條件的任意一種重排結果陣列。
 * 
 * 限制條件：
 * - 2 <= nums.length <= 2 * 10^4
 * - nums.length 是偶數
 * - nums 中恰好有一半是偶數、一半是奇數
 * - 0 <= nums[i] <= 1000
 * - 進階挑戰（韌體面試精髓）：你能否做到「原地 (In-place) 操作」，額外空間複雜度 O(1)，杜絕額外 malloc？
 * 
 * 範例 1:
 *   輸入: nums = [4,2,5,7]
 *   輸出: [4,5,2,7] (或 [4,7,2,5], [2,5,4,7], [2,7,4,5])
 * 
 * 範例 2:
 *   輸入: nums = [2,3]
 *   輸出: [2,3]
 */
int* sortArrayByParityII(int* nums, int numsSize, int* returnSize) {
    // 【雙指標原地就地交換 (In-place Swap)】
    // i 負責巡檢偶數索引 (0, 2, 4...)
    // j 負責尋找放錯位置的奇數索引 (1, 3, 5...)
    int j = 1;

    for (int i = 0; i < numsSize; i += 2) {
        // 若偶數索引 i 放了奇數 (放錯位置)
        if ((nums[i] % 2) != 0) {
            // 向後推進 j，直到找到放了偶數的奇數索引
            while (j < numsSize && (nums[j] % 2) != 0) {
                j += 2;
            }
            // 兩者皆放錯，原地對調一次搞定兩個位置！
            if (j < numsSize) {
                int temp = nums[i];
                nums[i] = nums[j];
                nums[j] = temp;
            }
        }
    }

    *returnSize = numsSize;
    return nums;
}

/*
 * =====================================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * =====================================================================================
 * int* sortArrayByParityII(int* nums, int numsSize, int* returnSize) {
 *     int slow = 0;
 *     int fast = 1;
 *     while(fast <= numsSize)
 *     {
 *         if((nums[fast] % 2) != 1)
 *         {
 *             int temp = nums[slow];
 *             nums[slow] = nums[fast];
 *             nums[fast] = temp;
 *             slow += 2;
 *         }
 *         fast += 2;
 *     }
 *     *returnSize = numsSize;
 *     return nums;
 * }
 * =====================================================================================
 * 【面試盲點剖析 & 雙指標原地不變量精華】 (LeetCode #922: Sort Array By Parity II)
 * =====================================================================================
 * 
 * 1. 致命陣列越界 (Buffer Overflow / Off-by-One)：
 *    - 原始寫法：`while (fast <= numsSize)`
 *    - 陣列合法索引範圍是 0 ~ numsSize - 1。
 *    - 當 numsSize = 4 時，fast = 1, 3, 5，但在 fast = 3 時若再次累加 fast += 2，
 *      下一輪若條件為 <= numsSize，fast = 5 就會讀取 nums[5]，造成嚴重的未定義行為 (UB) 與記憶體踩踏！
 *    - 正確終止條件：`fast < numsSize`。
 * 
 * 2. 雙指標交換的不變量 (Loop Invariant) 遺失：
 *    - 原始程式碼只檢查了 `nums[fast]` 是否放錯（奇數索引放了偶數），
 *      但【完全沒有檢查 nums[slow] 當前是否也是放錯的】！
 *    - 假設輸入為 [4, 2, 5, 7]：
 *      * slow = 0 (nums[0] = 4，本身就是偶數，位置完全正確！)
 *      * fast = 1 (nums[1] = 2，是偶數，放錯在奇數索引上)
 *      * 原始代碼直接將 nums[0] 與 nums[1] 交換！
 *      * 結果：nums[0] 變成 2 (偶數)，nums[1] 變成 4 (依然是偶數，還是錯的！)。
 *    - 交換的核心原則：
 *      ★ 必須是「偶數索引放了奇數」與「奇數索引放了偶數」，兩者一對一互換，才能同時將兩個位置導正！
 *      若 slow 所在的位置本來就是偶數，slow 必須跳過，絕不可盲目對調！
 * 
 * 3. 極致 O(1) 空間與 O(N) 時間思維：
 *    - 讓 i 專門遍歷偶數索引 (0, 2, 4...)。
 *    - 只有當 nums[i] 放了奇數時，才觸發 j (1, 3, 5...) 往後找到第一個放了偶數的位置，兩者原地交換。
 *    - i 與 j 都各自只往前走不回頭，總迴圈步數不超過 N，時間 O(N)，額外空間 O(1)！
 * =====================================================================================
 */

// === 驗證輔助函式 ===
static bool verifyParity(const int* arr, int size) {
    for (int i = 0; i < size; i++) {
        if ((i % 2) != (arr[i] % 2)) {
            return false;
        }
    }
    return true;
}

int main(void) {
    printf("=== Test P2: Sort Array By Parity II ===\n");

    // Test 1: [4, 2, 5, 7]
    int t1[] = {4, 2, 5, 7};
    int s1 = 0;
    int* ans1 = sortArrayByParityII(t1, 4, &s1);
    assert(s1 == 4);
    assert(verifyParity(ans1, 4));
    printf("Test 1 Passed: [4, 2, 5, 7] reordered correctly\n");

    // Test 2: [2, 3]
    int t2[] = {2, 3};
    int s2 = 0;
    int* ans2 = sortArrayByParityII(t2, 2, &s2);
    assert(s2 == 2);
    assert(verifyParity(ans2, 2));
    printf("Test 2 Passed: [2, 3] reordered correctly\n");

    // Test 3: [3, 1, 4, 2] (所有元素都在錯誤位置)
    int t3[] = {3, 1, 4, 2};
    int s3 = 0;
    int* ans3 = sortArrayByParityII(t3, 4, &s3);
    assert(s3 == 4);
    assert(verifyParity(ans3, 4));
    printf("Test 3 Passed: [3, 1, 4, 2] reordered correctly\n");

    // Test 4: [1, 3, 5, 2, 4, 6]
    int t4[] = {1, 3, 5, 2, 4, 6};
    int s4 = 0;
    int* ans4 = sortArrayByParityII(t4, 6, &s4);
    assert(s4 == 6);
    assert(verifyParity(ans4, 6));
    printf("Test 4 Passed: [1, 3, 5, 2, 4, 6] reordered correctly\n");

    printf("All P2 Local Tests Passed!\n");
    return 0;
}
