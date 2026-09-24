#include <stdio.h>

/* 
 * 題目 3: 尋找旋轉排序陣列中的最小值 (LeetCode #153 - Find Minimum in Rotated Sorted Array)
 * 難度: 🟡 一線 IC 設計廠 (聯發科、聯詠、瑞昱) 二分搜尋經典高頻題
 * 
 * 題目說明:
 * 已知一個長度為 n 的升序陣列在某個未知的軸點 (Pivot) 上進行了 1 到 n 次旋轉。
 * 例如：原陣列 [0,1,2,4,5,6,7] 旋轉 4 次後可能變為 [4,5,6,7,0,1,2]。
 * 假設陣列中所有元素皆互不相同 (Unique Elements)。
 * 請找出並回傳該陣列中的「最小值」。
 * 
 * 限制條件:
 * - 時間複雜度必須為: O(log N) (嚴禁使用 O(N) 遍歷陣列)
 * - 空間複雜度: O(1)
 */
int findMin(int* nums, int numsSize) {
    int left = 0;
    int right = numsSize - 1;

    // 終止條件：當 left == right 時，區間收斂到單一元素，即為全陣列最小值
    while (left < right) {
        int mid = left + (right - left) / 2;

        // 若 mid 大於 right，代表扭轉斷層 (最小值) 一定落在右半部 (mid, right]
        if (nums[mid] > nums[right]) {
            left = mid + 1;
        }
        // 若 mid 小於等於 right，代表右半部 [mid, right] 是單調遞增的，
        // 最小值可能落在左半部，或者 nums[mid] 本身就是最小值！
        else {
            right = mid; // 關鍵：不能寫 mid - 1，因為 nums[mid] 可能是最小值
        }
    }

    return nums[left];
}

/*
================================================================================
【原始盲點剖析與使用者程式碼存檔】
================================================================================
使用者原始實作：
int findMin(int* nums, int numsSize) {
    int left = 0;
    int right = numsSize - 1;
    while(left <= right)
    {
        int mid = left + (right - left) / 2;
        if(nums[left] < nums[mid])
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return nums[mid];
}

盲點 1：變數生命週期與 Scope（引發 MSVC 編譯錯誤 C2065）
- `int mid` 宣告在 `while` 迴圈大括號區塊內部，離開迴圈後在第 33 行執行 `return nums[mid];`，
  導致編譯器報錯：`error C2065: 'mid': undeclared identifier`。

盲點 2：二分基準錯誤（不能與 nums[left] 比較，必須與 nums[right] 比較）
- 當 `nums[left] < nums[mid]` 時，無法確定最小值在左側還是右側：
  - 反例：完全未旋轉的已排序陣列 `[11, 13, 15, 17]`，`nums[left]=11 < nums[mid]=13` 成立，
    但最小值就在左側 index 0（11）！若執行 `left = mid + 1`，會把真正的最小值拋棄。
  - 正確做法：以「右邊界 nums[right]」為基準！
    - 若 `nums[mid] > nums[right]`：說明斷層（扭轉點/最小值）必然在右半部 (mid, right]，因此 `left = mid + 1`。
    - 若 `nums[mid] <= nums[right]`：說明 [mid, right] 區間單調遞增，最小值必在左側或 mid 本身，因此 `right = mid`。

盲點 3：邊界收斂與終止條件（while (left < right)）
- 由於當 `nums[mid] <= nums[right]` 時我們設定 `right = mid`（保留 mid），
  若迴圈條件使用 `left <= right`，在剩下 1 個元素時可能陷入死循環。
- 採用 `while (left < right)`，當兩者相遇時自然退出迴圈，此時 `nums[left]` 即為唯一最小值。
================================================================================
*/

int main(void) {
    printf("=== Day 98 - 白板題 3: 尋找旋轉排序陣列中的最小值 (LeetCode #153) ===\n\n");

    // 測試案例 1
    int t1[] = {3, 4, 5, 1, 2};
    printf("Test 1: [3, 4, 5, 1, 2]\n");
    printf("Result:   %d\n", findMin(t1, 5));
    printf("Expected: 1\n\n");

    // 測試案例 2
    int t2[] = {4, 5, 6, 7, 0, 1, 2};
    printf("Test 2: [4, 5, 6, 7, 0, 1, 2]\n");
    printf("Result:   %d\n", findMin(t2, 7));
    printf("Expected: 0\n\n");

    // 測試案例 3: 原本就完全升序 (旋轉 n 次或 0 次)
    int t3[] = {11, 13, 15, 17};
    printf("Test 3: [11, 13, 15, 17]\n");
    printf("Result:   %d\n", findMin(t3, 4));
    printf("Expected: 11\n\n");

    // 測試案例 4: 雙元素旋轉
    int t4[] = {2, 1};
    printf("Test 4: [2, 1]\n");
    printf("Result:   %d\n", findMin(t4, 2));
    printf("Expected: 1\n\n");

    // 測試案例 5: 單元素
    int t5[] = {1};
    printf("Test 5: [1]\n");
    printf("Result:   %d\n", findMin(t5, 1));
    printf("Expected: 1\n");

    return 0;
}
