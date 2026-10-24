#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #34 - Find First and Last Position of Element in Sorted Array
 * 
 * 題目描述：
 * 給定一個按照非遞減順序排列的整數陣列 nums，以及一個目標值 target。
 * 請找出給定目標值在陣列中的開始位置和結束位置。
 * 
 * 如果陣列中不存在目標值 target，回傳 [-1, -1]。
 * 你必須設計並實作時間複雜度為 O(log n) 的演算法。
 * 
 * 範例 1：
 * 輸入：nums = [5, 7, 7, 8, 8, 10], target = 8
 * 輸出：[3, 4]
 * 
 * 範例 2：
 * 輸入：nums = [5, 7, 7, 8, 8, 10], target = 6
 * 輸出：[-1, -1]
 * 
 * 範例 3：
 * 輸入：nums = [], target = 0
 * 輸出：[-1, -1]
 * 
 * 限制條件：
 * 0 <= nums.length <= 10^5
 * -10^9 <= nums[i] <= 10^9
 * nums 呈現非遞減（由小到大排序）
 * -10^9 <= target <= 10^9
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
/*
 * =========================================================================
 * 【LeetCode 官方提交成績】：
 * - 狀態：Accepted (通過 88/88 測試案例)
 * - 執行時間：0 ms (擊敗 100.00% C 語言提交)
 * - 記憶體：10.4 MB
 * - Submission ID：2179845439
 * -------------------------------------------------------------------------
 * 【使用者原始白板程式碼】：
 * int* searchRange(int* nums, int numsSize, int target, int* returnSize) {
 *     int left = 0;
 *     int right = numsSize - 1;
 *     int mid1 = -1;
 *     int mid2 = -1;
 *     int mid = 0;
 *     while(left <= right)
 *     {
 *         mid = left + (right - left) / 2;
 *         if(nums[mid] == target) { right = mid - 1; }
 *         else if(nums[mid] < target) { left = mid + 1; }
 *         else { right = mid - 1; }
 *     }
 *     if(nums[mid] == target) mid1 = mid; // 盲點 1：迴圈最後一步必定讓 nums[mid] != target
 *     left = 0;
 *     right = numsSize - 1;
 *     while(left <= right)
 *     {
 *         mid = left + (right - left) / 2;
 *         if(nums[mid] == target) { left = mid + 1; }
 *         else if(nums[mid] < target) { left = mid + 1; }
 *         else { right = mid - 1; }
 *     }
 *     if(nums[mid] == target) mid2 = mid; // 盲點 1：同上，導致 mid2 永遠是 -1
 *     *returnSize = 2;
 *     int* result = (int*) malloc(2*sizeof(int));
 *     result[0] = mid1;
 *     result[1] = mid2;
 *     return result;
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點剖析】：
 * 1. 候選解記錄時機錯誤（致命邏輯缺陷）：
 *    - 在二分搜尋中，當你找到 nums[mid] == target 時，為了尋找「更左」或「更右」的邊界，
 *      你分別做了 right = mid - 1 或 left = mid + 1。
 *    - 迴圈會繼續往下跑，直到 left > right 終止。
 *    - 在迴圈終止的那最後一輪，mid 的位置必定落在 nums[mid] != target 的元素上！
 *    - 離開迴圈後才做 if (nums[mid] == target) 永遠不會成立，導致 mid1/mid2 被漏記（變成 -1）。
 *    - 正確做法：必須在迴圈內部「命中 target 的當下立即更新候選值」：
 *      if (nums[mid] == target) { mid1 = mid; right = mid - 1; }
 * 2. 空陣列邊界防禦（防 Segmentation fault）：
 *    - 若 numsSize == 0，nums[mid] 會直接存取空指標或越界當機。開頭需加上 numsSize == 0 的守衛條件。
 * 3. 提早剪枝小亮點（Early Return）：
 *    - 若第一次二分搜尋連左邊界都沒找到（mid1 == -1），表示 target 根本不存在於陣列中，
 *      第二輪二分搜尋完全不需要做，直接回傳 [-1, -1] 即可！
 * =========================================================================
 */

int* searchRange(int* nums, int numsSize, int target, int* returnSize) {
    *returnSize = 2;
    int* result = (int*)malloc(2 * sizeof(int));
    result[0] = -1;
    result[1] = -1;

    // 空陣列邊界防禦
    if (numsSize == 0) {
        return result;
    }

    // 第一輪二分搜尋：尋找最左邊界（First Occurrence）
    int left = 0;
    int right = numsSize - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) {
            result[0] = mid;     // 命中時立即更新候選解！
            right = mid - 1;    // 繼續往左側區間擠壓收斂
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    // 若目標值根本不存在，直接返回 [-1, -1]，省下第二輪二分搜尋
    if (result[0] == -1) {
        return result;
    }

    // 第二輪二分搜尋：尋找最右邊界（Last Occurrence）
    left = 0;
    right = numsSize - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) {
            result[1] = mid;     // 命中時立即更新候選解！
            left = mid + 1;     // 繼續往右側區間擠壓收斂
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}

int main() {
    int nums1[] = {5, 7, 7, 8, 8, 10};
    int retSize1 = 0;
    int* res1 = searchRange(nums1, 6, 8, &retSize1);
    printf("Test 1: [%d, %d] (Expected: [3, 4])\n", res1 ? res1[0] : -99, res1 ? res1[1] : -99);
    free(res1);

    int nums2[] = {5, 7, 7, 8, 8, 10};
    int retSize2 = 0;
    int* res2 = searchRange(nums2, 6, 6, &retSize2);
    printf("Test 2: [%d, %d] (Expected: [-1, -1])\n", res2 ? res2[0] : -99, res2 ? res2[1] : -99);
    free(res2);

    int retSize3 = 0;
    int* res3 = searchRange(NULL, 0, 0, &retSize3);
    printf("Test 3: [%d, %d] (Expected: [-1, -1])\n", res3 ? res3[0] : -99, res3 ? res3[1] : -99);
    free(res3);

    return 0;
}
