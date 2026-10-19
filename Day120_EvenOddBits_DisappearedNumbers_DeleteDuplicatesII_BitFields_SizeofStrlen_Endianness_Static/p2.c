#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #448 - Find All Numbers Disappeared in an Array
 * 
 * 題目描述：
 * 給定一個包含 n 個整數的陣列 nums，其中所有整數都在範圍 [1, n] 內。
 * 陣列中某些元素出現了兩次，而有些元素只出現一次，導致某些 [1, n] 的數字沒有出現。
 * 請找出所有在 [1, n] 範圍內「沒有出現在 nums 中」的數字，並以陣列形式回傳。
 * 
 * 要求：
 * 請在 O(n) 時間複雜度且不使用額外空間（除了回傳陣列外，額外空間複雜度為 O(1)）的條件下完成。
 * 
 * 限制條件：
 * n == nums.length
 * 1 <= n <= 10^5
 * 1 <= nums[i] <= n
 */

/* =========================================================================
 * 【使用者原始白板程式碼 (原始未修改版本 - 一次 AC / 1 ms)】
 * -------------------------------------------------------------------------
 * int* findDisappearedNumbers(int* nums, int numsSize, int* returnSize) {
 *     int count = numsSize;
 *     for(int i = 0; i < numsSize; i++)
 *     {
 *         if(nums[abs(nums[i]) - 1] > 0)
 *         {
 *             nums[abs(nums[i]) - 1] = -nums[abs(nums[i]) - 1];
 *             count--;
 *         }
 *     }
 *     *returnSize = count;
 *     int* result = (int*) malloc(count*sizeof(int));
 *     int curr = 0;
 *     for(int j = 0; j < numsSize; j++)
 *     {
 *         if(nums[j] > 0)
 *         {
 *             result[curr] = j + 1;
 *             curr++;   
 *         }
 *     }
 *     return result;
 * }
 * -------------------------------------------------------------------------
 * 【設計亮點剖析】：
 * 1. 原地正負號標記 (In-Place Sign Negation)：
 *    - 將數字 x 映射至索引 idx = abs(x) - 1。
 *    - 若 nums[idx] > 0 則翻轉為負數，代表該數字已經出現過。
 * 2. 嚴格防止重複翻轉：
 *    - 透過 `nums[abs(nums[i]) - 1] > 0` 判斷，杜絕重複元素（Duplicate）二次翻轉變回正數的經典陷阱！
 * 3. 巧妙的兩趟掃描與精確動態配置：
 *    - 第一趟透過 `count--` 直接計算出「缺失數字的精確總數」，使得 `malloc` 一次精準到位（0 記憶體浪費）。
 *    - 第二趟透過 `nums[j] > 0` 快速撈出未被訪問的數字 `j + 1`。
 * 4. 遠端實測：35/35 全測資 Accepted，執行時間 1 ms (Beats 98%+)！
 * =========================================================================
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* findDisappearedNumbers(int* nums, int numsSize, int* returnSize) {
    int count = numsSize;
    for (int i = 0; i < numsSize; i++) {
        if (nums[abs(nums[i]) - 1] > 0) {
            nums[abs(nums[i]) - 1] = -nums[abs(nums[i]) - 1];
            count--;
        }
    }

    *returnSize = count;
    int* result = (int*)malloc(count * sizeof(int));
    int curr = 0;
    for (int j = 0; j < numsSize; j++) {
        if (nums[j] > 0) {
            result[curr] = j + 1;
            curr++;   
        }
    }
    return result;
}

int main() {
    int nums1[] = {4, 3, 2, 7, 8, 2, 3, 1};
    int size1 = 0;
    int* res1 = findDisappearedNumbers(nums1, 8, &size1);
    printf("Test 1: [");
    for (int i = 0; i < size1; i++) {
        printf("%d%s", res1[i], (i == size1 - 1) ? "" : ", ");
    }
    printf("] (Expected: [5, 6])\n");
    free(res1);

    int nums2[] = {1, 1};
    int size2 = 0;
    int* res2 = findDisappearedNumbers(nums2, 2, &size2);
    printf("Test 2: [");
    for (int i = 0; i < size2; i++) {
        printf("%d%s", res2[i], (i == size2 - 1) ? "" : ", ");
    }
    printf("] (Expected: [2])\n");
    free(res2);

    return 0;
}
