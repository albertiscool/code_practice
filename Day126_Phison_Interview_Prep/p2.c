#include <stdio.h>
#include <stdlib.h>

/*
 * 【群聯面試經典題之二】已排序陣列之區間過濾輸出 (Phison Q2)
 *
 * 題目描述：
 * 給定一個已經排序且元素可能含有重複值的整數陣列 a（大小為 size）。
 * 請實作函式 printRangeExceptA(a, size, b)，依照參數 b (b 範圍為 0 ~ 4)，
 * 印出該區間內所有的整數，且不包含出現在陣列 a 內的元素。
 *
 * 區間定義：
 * - b = 0: 印出 0 ~ 99
 * - b = 1: 印出 100 ~ 199
 * - b = 2: 印出 200 ~ 299
 * - b = 3: 印出 300 ~ 399
 * - b = 4: 印出 400 ~ 499
 *
 * 限制與要求：
 * 1. 0 <= b <= 4
 * 2. 必須以最佳時間複雜度 O(區間長度) 與空間複雜度 O(1) 完成。
 * 3. 需妥善處理指標越界防禦、陣列內重複元素、以及陣列元素小於/大於該區間邊界之情況。
 * 4. 輸出格式：每個印出的數字後方帶一個空格，印完該區間後換行。
 */

void printRangeExceptA(const int *a, int size, int b) {
    int start = b * 100;
    int end = start + 100;

    // 1. 入參安全防護：如果 a 是 NULL 或長度為 0，直接印出全部區間數字
    if (a == NULL || size <= 0) {
        for (int i = start; i < end; i++) {
            printf("%d ", i);
        }
        printf("\n");
        return;
    }

    // 2. 跳過小於當前區間起點的數值
    int curr = 0;
    while (curr < size && a[curr] < start) {
        curr++;
    }

    // 3. 遍歷該區間所有數字
    for (int i = start; i < end; i++) {
        if (curr < size && a[curr] == i) {
            // 跳過所有重複的元素
            while (curr < size && a[curr] == i) {
                curr++;
            }
        } else {
            printf("%d ", i);
        }
    }
    printf("\n");
}

/*
================================================================================
【原始白板代碼保留與盲點詳細剖析】
================================================================================
void printRangeExceptA(const int *a, int size, int b) {
    int curr = 0;
    while(a[curr] < b * 100 && curr < size) curr++; 
        // ❌ 盲點 1 & 2:
        // 1. 執行順序在 for 與 if 之前，當 a == NULL 時直接存取 a[curr] 導致 Segfault 崩潰。
        // 2. 且短路求值 (&&) 順序寫反：curr < size 寫在右側，導致先讀取非法陣列越界位址後才檢查長度。

    for(int i = b * 100; i < b * 100 + 100; i++)
    {
        if(a != NULL && curr < size && i == a[curr])
        {
            while(i == a[curr] && curr < size) curr++;
                // ❌ 盲點 3:
                // 短路求值順序寫反：當比到最後一個元素 curr++ 越界後，
                // 下一輪會先對 a[curr] 解引用（越界讀取 Out-of-Bounds），才檢查 curr < size！
        }
        else
        {
            printf("%d ",i);
        }
    }
    printf("\n");
}
================================================================================
*/

int main() {
    printf("=== Test 1: b = 0, a contains some numbers in 0~99 ===\n");
    int a1[] = {5, 10, 15, 20, 20, 25, 120};
    printf("Actual output:   ");
    printRangeExceptA(a1, 7, 0);

    printf("\n=== Test 2: b = 1, a contains boundary numbers ===\n");
    int a2[] = {50, 100, 101, 150, 199, 200};
    printf("Actual output:   ");
    printRangeExceptA(a2, 6, 1);

    printf("\n=== Test 3: b = 2, a is empty (size = 0) ===\n");
    printf("Actual output:   ");
    printRangeExceptA(NULL, 0, 2);

    return 0;
}
