#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * LeetCode #349: Intersection of Two Arrays
 * 
 * 題目描述：
 * 給定兩個整數陣列 nums1 與 nums2，回傳它們的交集陣列。
 * 交集陣列中的每個元素必須是唯一的（Unique，不重複），輸出順序不限。
 * 
 * 限制條件：
 * - 1 <= nums1Size, nums2Size <= 1000
 * - 0 <= nums1[i], nums2[i] <= 1000
 * 
 * 範例 1:
 *   輸入: nums1 = [1,2,2,1], nums2 = [2,2]
 *   輸出: [2]
 * 
 * 範例 2:
 *   輸入: nums1 = [4,9,5], nums2 = [9,4,9,8,4]
 *   輸出: [9,4] 或 [4,9]
 * 
 * 注意：回傳之陣列必須動態配置 (malloc)，並透過 returnSize 回傳有效元素個數。
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* intersection(int* nums1, int nums1Size, int* nums2, int nums2Size, int* returnSize) {
    int seen[1001] = {0};
    
    // 1. 標記 nums1 中所有出現過的元素
    for (int i = 0; i < nums1Size; i++) {
        seen[nums1[i]] = 1;
    }
    
    // 2. 交集最大可能長度為兩陣列較小者
    int size = (nums1Size < nums2Size) ? nums1Size : nums2Size;
    int* result = (int*)malloc(size * sizeof(int));
    int curr = 0;
    
    // 3. 掃描 nums2，若在 seen 中有記錄則加入 result 並重設為 0 去重
    for (int j = 0; j < nums2Size; j++) {
        if (seen[nums2[j]] == 1) {
            seen[nums2[j]] = 0; // 自動去重
            result[curr] = nums2[j];
            curr++;
        }
    }

    *returnSize = curr;
    return result;
}

int main(void) {
    printf("=== Test P2: Intersection of Two Arrays ===\n");
    int nums1_a[] = {1, 2, 2, 1};
    int nums2_a[] = {2, 2};
    int retSize_a = 0;
    int* res_a = intersection(nums1_a, 4, nums2_a, 2, &retSize_a);
    printf("Test 1 Result size: %d, elements: ", retSize_a);
    for (int i = 0; i < retSize_a; i++) printf("%d ", res_a[i]);
    printf("\n");
    assert(retSize_a == 1);
    assert(res_a[0] == 2);
    free(res_a);

    int nums1_b[] = {4, 9, 5};
    int nums2_b[] = {9, 4, 9, 8, 4};
    int retSize_b = 0;
    int* res_b = intersection(nums1_b, 3, nums2_b, 5, &retSize_b);
    printf("Test 2 Result size: %d, elements: ", retSize_b);
    for (int i = 0; i < retSize_b; i++) printf("%d ", res_b[i]);
    printf("\n");
    assert(retSize_b == 2);
    free(res_b);

    printf("All tests passed!\n");
    return 0;
}

/* =========================================================================
 * 🚩 【Albert 原始嘗試盲點剖析與韌體架構深入複盤】
 * =========================================================================
 * 
 * 原始撰寫程式碼：
 * -------------------------------------------------------------------------
 * int* intersection(int* nums1, int nums1Size, int* nums2, int nums2Size, int* returnSize) {
 *     int seen[1001] = {0};
 *     for(int i = 0; i < nums1Size; i++)
 *     {
 *         if(seen[nums1[i]] == 0)
 *         {
 *             seen[nums1[i]] = 1;
 *         }
 *     }
 *     int curr = 0;
 *     int size = (nums1Size < nums2Size) : nums1Size ? nums2Size; // ⚠️ 語法筆誤
 *     int* result = (int*) malloc(size*sizeof(int));
 *     int curr = 0;                                               // ⚠️ 重複宣告
 *     for(int j = 0; j < nums2Size; j++)
 *     {
 *         if(seen[nums2[j]] == 1)
 *         {
 *             seen[nums2[j]] = 0;
 *             result[curr] = nums2[j];
 *             curr++;
 *         }
 *     }
 * 
 *     *returnSize = curr;
 *     return result;
 * }
 * -------------------------------------------------------------------------
 * 
 * 💡 邏輯點評：滿分演算法思維！
 * 1. 成功應用 O(1) 空間的布林查表陣列 seen[1001]，達成 O(M + N) 極速線性搜尋。
 * 2. 在 nums2 掃描到符合項時，精準執行 `seen[nums2[j]] = 0;` 原地去重，乾淨俐落！
 * 
 * ⚠️ 小瑕疵（編譯語法筆誤）：
 * 1. 三元運算子寫反：C 語言語法為 `條件 ? 真值 : 偽值`，原代碼誤寫為 `... : nums1Size ? nums2Size;`。
 * 2. 變數重複宣告：在第 40 行與第 43 行宣告了兩次 `int curr = 0;`，會觸發 C 語言編譯器 `redefinition of 'curr'` 錯誤。
 * ========================================================================= */
