#include <stdio.h>

/* 
 * 題目 3: 尋找峰值 (LeetCode #162 - Find Peak Element)
 * 難度: 🟡 一線 IC 設計廠 (聯發科、聯詠、瑞昱) 二分搜尋經典高頻題
 * 
 * 題目說明:
 * 峰值元素是指其值嚴格大於左右相鄰值的元素。
 * 給定一個整數陣列 nums，請找到任意一個峰值元素並回傳其「索引 (Index)」。
 * 陣列可能包含多個峰值，在這種情況下，回傳任何一個峰值所在的索引即可。
 * 題目保證：nums[-1] = nums[n] = -∞，且相鄰元素 nums[i] != nums[i + 1]。
 * 
 * 限制條件:
 * - 時間複雜度必須為: O(log N) (嚴禁使用 O(N) 線性遍歷找最大值)
 * - 空間複雜度: O(1)
 */
int findPeakElement(int* nums, int numsSize) {
    if (nums == NULL || numsSize <= 0) {
        return -1;
    }
    int left = 0;
    int right = numsSize - 1;

    // 終止條件：當 left == right 時，搜尋區間收斂到單一元素，該元素即為峰值
    while (left < right) {
        int mid = left + (right - left) / 2;

        // 若 mid 小於 mid + 1，代表正處於「上坡」，右半部必然存在至少一個峰值 (排除左半部)
        if (nums[mid] < nums[mid + 1]) {
            left = mid + 1;
        }
        // 若 mid 大於 mid + 1，代表正處於「下坡」，左半部必然存在峰值，或 mid 本身就是峰頂 (保留 mid)
        else {
            right = mid;
        }
    }

    return left;
}

/*
================================================================================
【原始盲點剖析與使用者程式碼存檔】
================================================================================
使用者原始實作：
int findPeakElement(int* nums, int numsSize) {
    if(nums == NULL)
    {
        return -1;
    }
    int left = 0;
    int right = numsSize - 1;
    while(left < right)
    {
        int mid = left + (right - left) / 2;
        if(nums[mid] > nums[mid+1])
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }
    return right;
}

盲點 1：坡度方向與二分收斂方向顛倒（誤將尋找峰頂寫成尋找谷底）
- 原始代碼中判斷條件寫成了：
  `if (nums[mid] > nums[mid + 1]) { left = mid + 1; }`
- 物理意義剖析：
  - 當 `nums[mid] > nums[mid + 1]` 時，右側高度比目前更低，這代表朝右邊走是「下坡 (Downhill)」！
  - 如果此時執行 `left = mid + 1`，相當於把左側高處與 mid 拋棄，轉而往右側低窪處走，
    結果找出來的反而是「局部谷底 (Local Minimum)」而非「山峰 (Local Maximum)」！
  - 測試實測：在 [1, 2, 3, 1] 中，這段邏輯一路走向了谷底 index 0 (數值 1)，導致誤判。
- 正確爬山範式：
  - 若 `nums[mid] < nums[mid + 1]`：正處於「上坡」，往右爬必有峰頂 -> `left = mid + 1;`
  - 若 `nums[mid] > nums[mid + 1]`：正處於「下坡」，左側必有峰頂或 mid 本身即是頂點 -> `right = mid;`
================================================================================
*/

int main(void) {
    printf("=== Day 99 - 白板題 3: 尋找峰值 (LeetCode #162) ===\n\n");

    // 測試案例 1: 單峰 [1, 2, 3, 1] -> index 2 (數值 3)
    int t1[] = {1, 2, 3, 1};
    int r1 = findPeakElement(t1, 4);
    printf("Test 1: [1, 2, 3, 1]\n");
    printf("Result Index:   %d (數值 %d)\n", r1, t1[r1]);
    printf("Expected Index: 2 (數值 3)\n\n");

    // 測試案例 2: 多峰 [1, 2, 1, 3, 5, 6, 4] -> index 1 (數值 2) 或 index 5 (數值 6) 皆可
    int t2[] = {1, 2, 1, 3, 5, 6, 4};
    int r2 = findPeakElement(t2, 7);
    printf("Test 2: [1, 2, 1, 3, 5, 6, 4]\n");
    printf("Result Index:   %d (數值 %d)\n", r2, t2[r2]);
    printf("Expected:       Index 1 (2) 或 Index 5 (6) 皆合法\n\n");

    // 測試案例 3: 單調遞增 [1, 2, 3] -> 峰值在最右端 index 2 (數值 3)
    int t3[] = {1, 2, 3};
    int r3 = findPeakElement(t3, 3);
    printf("Test 3: [1, 2, 3]\n");
    printf("Result Index:   %d (數值 %d)\n", r3, t3[r3]);
    printf("Expected Index: 2 (數值 3)\n\n");

    // 測試案例 4: 單元素 [1] -> 峰值在 index 0
    int t4[] = {1};
    int r4 = findPeakElement(t4, 1);
    printf("Test 4: [1]\n");
    printf("Result Index:   %d (數值 %d)\n", r4, t4[r4]);
    printf("Expected Index: 0 (數值 1)\n");

    return 0;
}
