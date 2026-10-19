#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #1539 - Kth Missing Positive Number
 * 
 * 考點對照 (HackMD 群聯面試題 & 筆試「群聯三題」)：
 * 1. 筆記「群聯三題」第一題與第二題：給定已排序陣列，找出缺失的數字（Missing Numbers）。
 * 2. 筆記考古第 9 題：「給一段 binary search code 解釋」。
 * 
 * 題目描述：
 * 給定一個嚴格遞增排序的正整數陣列 arr，以及一個整數 k。
 * 請找出該陣列中「第 k 個缺失的正整數」。
 * 正整數由 1 開始起算：1, 2, 3, 4, 5, ...
 * 
 * 限制條件：
 * 1 <= arr.length <= 1000
 * 1 <= arr[i] <= 1000
 * 1 <= k <= 1000
 * arr[i] 為嚴格遞增排序
 */

/* =========================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * -------------------------------------------------------------------------
 * int findKthPositive(int* arr, int arrSize, int k) {
 *     // 請於此處實作純白板解法
 *     int curr = 0;
 *     int count = 1;
 *     while(curr < arrSize)
 *     {
 *         if(arr[curr] != count)
 *         {
 *             k--;
 *             count++;
 *         }
 *         else
 *         {
 *             count++
 *             curr++;
 *         }
 * 
 *         if(k == 0)
 *         {
 *             return count - 1;
 *         }
 *     }
 * 
 *     return count + k - 1;
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點剖析】：
 * 1. 語法錯誤：
 *    - 第 53 行 `count++` 結尾漏掉了分號 `;`，導致編譯失敗：`error: expected ';' before 'curr'`。
 * 2. 演算法邏輯評定：
 *    - 邏輯完全正確！這正是「群聯三題」第一題與第二題最經典的雙指標/模擬走訪法。
 *    - 時間複雜度為 O(N + k)，空間複雜度為 O(1)。
 * 3. 面試進階優化 (O(log N) 二分搜尋法)：
 *    - 由於 arr 嚴格遞增，若無缺失數字，arr[i] 應為 i + 1。
 *    - 因此在索引 mid 之前缺失的數字個數為：`arr[mid] - (mid + 1)`。
 *    - 若缺失數 < k，表示第 k 個缺失數在右半段；否則在左半段。
 *    - 二分搜尋結束時，答案可直接化簡為 `left + k`，時間縮短至 O(log N)！
 * =========================================================================
 */

int findKthPositive(int* arr, int arrSize, int k) {
    // 方案 A：使用者邏輯修復版 (O(N + k) 直覺模擬法，群聯經典解法)
    int curr = 0;
    int count = 1;
    while (curr < arrSize) {
        if (arr[curr] != count) {
            k--;
            count++;
        } else {
            count++; // 補充分號
            curr++;
        }

        if (k == 0) {
            return count - 1;
        }
    }

    return count + k - 1;
}

// 方案 B：面試加分項 O(log N) 二分搜尋法 (對應 HackMD 考古「解釋 Binary Search」)
int findKthPositive_BinarySearch(int* arr, int arrSize, int k) {
    int left = 0, right = arrSize - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        // 在索引 mid 之前缺失的數字個數
        int missing = arr[mid] - (mid + 1);
        if (missing < k) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    // 最終 left 為分界點，答案即為 left + k
    return left + k;
}

int main() {
    int arr1[] = {2, 3, 4, 7, 11};
    printf("Test 1: [2,3,4,7,11], k=5 -> %d (Expected: 9)\n", findKthPositive(arr1, 5, 5));

    int arr2[] = {1, 2, 3, 4};
    printf("Test 2: [1,2,3,4], k=2 -> %d (Expected: 6)\n", findKthPositive(arr2, 4, 2));

    return 0;
}
