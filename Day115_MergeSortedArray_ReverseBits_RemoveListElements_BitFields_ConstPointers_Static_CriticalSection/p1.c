#include <stdio.h>
#include <assert.h>

/**
 * LeetCode #88: Merge Sorted Array
 * 
 * 題目描述：
 * 給定兩個按非遞減順序（由小到大）排列的整數陣列 nums1 和 nums2，另有兩個整數 m 和 n，
 * 分別表示 nums1 和 nums2 中的有效元素數目。
 * 請將 nums2 合併到 nums1 中，使合併後的陣列同樣按非遞減順序排列。
 * 
 * 注意：
 * - 最終合併後的陣列不應由函式返回，而是直接儲存在陣列 nums1 中。
 * - nums1 的空間大小為 m + n，其中前 m 個元素表示應合併的有效元素，後 n 個元素初始為 0，應被覆蓋。
 * 
 * 限制條件：
 * - nums1Size == m + n
 * - nums2Size == n
 * - 0 <= m, n <= 200
 * - 1 <= m + n <= 200
 * - -10^9 <= nums1[i], nums2[j] <= 10^9
 * - 進階挑戰：你能否設計出時間複雜度為 O(m + n) 且空間複雜度為 O(1) 的原地合併演算法？
 * 
 * 範例 1:
 *   輸入: nums1 = [1,2,3,0,0,0], m = 3, nums2 = [2,5,6], n = 3
 *   輸出: [1,2,2,3,5,6]
 * 
 * 範例 2:
 *   輸入: nums1 = [1], m = 1, nums2 = [], n = 0
 *   輸出: [1]
 * 
 * 範例 3:
 *   輸入: nums1 = [0], m = 0, nums2 = [1], n = 1
 *   輸出: [1]
 */

void merge(int* nums1, int nums1Size, int m, int* nums2, int nums2Size, int n) {
    int curr1 = m - 1;
    int curr2 = n - 1;
    int tail = m + n - 1;

    while (curr1 >= 0 && curr2 >= 0) {
        if (nums1[curr1] >= nums2[curr2]) {
            nums1[tail] = nums1[curr1];
            curr1--;
        } else {
            nums1[tail] = nums2[curr2];
            curr2--;
        }
        tail--;
    }

    // 若 nums2 還有剩餘元素，全數填入 nums1 前端
    // （若 nums1 還有剩餘元素，其本身就已在 nums1 前部且維持排序，無須多做搬移）
    while (curr2 >= 0) {
        nums1[tail] = nums2[curr2];
        curr2--;
        tail--;
    }
}

/*
 * =====================================================================================
 * 【面試盲點剖析 & 邊界除錯精華】 (LeetCode #88: Merge Sorted Array)
 * =====================================================================================
 * 原始盲點剖析：
 * 1. 函式型別為 void，誤寫 `return nums1;` / `return nums2;`：
 *    - C 語言中 void 函式不可返回數值，會直接觸發編譯錯誤 (Wreturn-mismatch)。
 *    - 更重要的是，當 m == 0 時，若直接 `return nums2;`，nums1 本身完全沒有被寫入資料，
 *      nums1 依然維持空或未初始化狀態，造成答案錯誤！
 * 
 * 2. 終止條件誤用 `!= 0` 而非 `>= 0` (致命索引 0 遺失)：
 *    - 原始程式碼寫 `while (curr1 != 0 && curr2 != 0)` 與 `while (curr2 != 0)`。
 *    - 索引 0 是合法的有效元素位置！當 curr1 或 curr2 減到 0 時，迴圈會直接中斷跳出，
 *      導致 nums1[0] 與 nums2[0] 的數值完全沒有被比對或搬移！
 *    - 必須改為 `>= 0`，才能完整涵蓋第 0 個元素。
 * 
 * 3. 完美精簡邏輯 (無需特判 m == 0 或 n == 0)：
 *    - 只要將條件設為 `curr1 >= 0 && curr2 >= 0`：
 *      * 當 m == 0 時，curr1 初始為 -1，第一個迴圈自動不進，第二個迴圈自動把 nums2 全部灌入 nums1。
 *      * 當 n == 0 時，curr2 初始為 -1，兩個迴圈皆不進，nums1 保持原樣，完美通吃！
 * =====================================================================================
 */

int main(void) {
    printf("=== Test P1: Merge Sorted Array ===\n");

    // Test 1: [1,2,3,0,0,0], m = 3, [2,5,6], n = 3 -> [1,2,2,3,5,6]
    int nums1_1[] = {1, 2, 3, 0, 0, 0};
    int nums2_1[] = {2, 5, 6};
    int exp1[] = {1, 2, 2, 3, 5, 6};
    merge(nums1_1, 6, 3, nums2_1, 3, 3);
    for (int i = 0; i < 6; i++) {
        assert(nums1_1[i] == exp1[i]);
    }
    printf("Test 1 Passed: [1,2,3,0,0,0] + [2,5,6] -> [1,2,2,3,5,6]\n");

    // Test 2: [1], m = 1, [], n = 0 -> [1]
    int nums1_2[] = {1};
    int nums2_2[] = {};
    int exp2[] = {1};
    merge(nums1_2, 1, 1, nums2_2, 0, 0);
    assert(nums1_2[0] == exp2[0]);
    printf("Test 2 Passed: [1] + [] -> [1]\n");

    // Test 3: [0], m = 0, [1], n = 1 -> [1]
    int nums1_3[] = {0};
    int nums2_3[] = {1};
    int exp3[] = {1};
    merge(nums1_3, 1, 0, nums2_3, 1, 1);
    assert(nums1_3[0] == exp3[0]);
    printf("Test 3 Passed: [0] + [1] -> [1]\n");

    // Test 4: [4,5,6,0,0,0], m = 3, [1,2,3], n = 3 -> [1,2,3,4,5,6]
    int nums1_4[] = {4, 5, 6, 0, 0, 0};
    int nums2_4[] = {1, 2, 3};
    int exp4[] = {1, 2, 3, 4, 5, 6};
    merge(nums1_4, 6, 3, nums2_4, 3, 3);
    for (int i = 0; i < 6; i++) {
        assert(nums1_4[i] == exp4[i]);
    }
    printf("Test 4 Passed: [4,5,6,0,0,0] + [1,2,3] -> [1,2,3,4,5,6]\n");

    printf("All P1 Local Tests Passed!\n");
    return 0;
}
