#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 題目 2【兩數之和 II - 輸入有序陣列 (Two Sum II - LeetCode #167)】
// 難度：🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠雙指針高頻經典題)
// 題目說明：給定一個下標從 1 開始的「已按升序排列」的整數陣列 numbers，
//           請從陣列中找出兩個數滿足其和等於目標數 target。
//           假設每個輸入剛好只有一個有效答案，而且不可重複使用相同的元素。
//           你必須使用 O(1) 的額外空間解決問題。
//           回傳的結果為動態配置的陣列，長度為 2，內容為兩數的 1-indexed 下標 [index1, index2]。
// 範例 1：
//    輸入：numbers = [2,7,11,15], target = 9
//    輸出：[1,2] (因為 2 + 7 == 9)
// 範例 2：
//    輸入：numbers = [2,3,4], target = 6
//    輸出：[1,3] (因為 2 + 4 == 6)
// 範例 3：
//    輸入：numbers = [-1,0], target = -1
//    輸出：[1,2] (因為 -1 + 0 == -1)
// 限制條件：
//    - 2 <= numbers.length <= 3 * 10^4
//    - -1000 <= numbers[i] <= 1000
//    - numbers 按非遞減順序排列
//    - -1000 <= target <= 1000
//    - 僅存在一個有效答案
// ============================================================================

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* numbers, int numbersSize, int target, int* returnSize) {
    if (numbers == NULL || numbersSize < 2 || returnSize == NULL) {
        if (returnSize) *returnSize = 0;
        return NULL;
    }

    int head = 0;
    int tail = numbersSize - 1;

    while (head < tail) {
        int sum = numbers[head] + numbers[tail];
        if (sum == target) {
            // 題目要求：下標從 1 開始計算 (1-indexed)！因此必須 + 1
            int *result = (int*)malloc(2 * sizeof(int));
            result[0] = head + 1;
            result[1] = tail + 1;
            *returnSize = 2;
            return result;
        } else if (sum > target) {
            tail--; // 和太大，右指針左移減小總和
        } else {
            head++; // 和太小，左指針右移增加總和
        }
    }

    *returnSize = 0;
    return NULL;
}

/*
 * ============================================================================
 * 【原始錯誤實作紀錄與盲點分析】
 * 錯誤原因：
 * 1. 忽略 1-Indexed 要求 (題目明訂下標從 1 開始)：
 *    程式中直接填入 `result[0] = head; result[1] = tail;`，
 *    傳回了從 0 開始的索引值 (例如測資 1 回傳了 [0, 1] 而非預期的 [1, 2])！
 *    正確寫法必須加 1：`result[0] = head + 1; result[1] = tail + 1;`。
 * 2. 迴圈條件防禦性較弱：
 *    `while(numbers[head] + numbers[tail] != target)`
 *    若極端情況下陣列中沒有解，指針會交錯越界引發 Segfault。
 *    標準寫法應為 `while (head < tail)`。
 * ============================================================================
int* twoSum_wrong(int* numbers, int numbersSize, int target, int* returnSize) {
    if(numbers == NULL)
    {
        return NULL;
    }
    int head = 0;
    int tail = numbersSize - 1;
    while(numbers[head] + numbers[tail] != target)
    {
        if(numbers[head] + numbers[tail] > target)
        {
            tail--;
        } 
        else
        {
            head++;
        }
    }
    *returnSize = 2;
    int *result = (int* )malloc(2 * sizeof(int));
    result[0] = head; // ❌ 盲點：題目要求 1-indexed，應填入 head + 1 與 tail + 1
    result[1] = tail;
    return result;
}
*/

int main(void) {
    /* 測試測資 1: [2, 7, 11, 15], target = 9 */
    int nums1[] = {2, 7, 11, 15};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    int retSize1 = 0;
    int* ans1 = twoSum(nums1, n1, 9, &retSize1);
    if (ans1) {
        printf("測資 1 結果: [%d, %d] (預期: [1, 2])\n", ans1[0], ans1[1]);
        free(ans1);
    } else {
        printf("測資 1 結果: NULL\n");
    }

    /* 測試測資 2: [2, 3, 4], target = 6 */
    int nums2[] = {2, 3, 4};
    int n2 = sizeof(nums2) / sizeof(nums2[0]);
    int retSize2 = 0;
    int* ans2 = twoSum(nums2, n2, 6, &retSize2);
    if (ans2) {
        printf("測資 2 結果: [%d, %d] (預期: [1, 3])\n", ans2[0], ans2[1]);
        free(ans2);
    } else {
        printf("測資 2 結果: NULL\n");
    }

    return 0;
}
