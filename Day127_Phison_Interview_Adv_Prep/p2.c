#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>

/*
 * 【群聯面試 Q1/Q2 變形】區間缺失範圍摘要 (Summary Missing Ranges)
 *
 * 題目背景與情境：
 * 給定一個已排序且不含重複元素的整數陣列 nums，以及目標範圍的起點 lower 與終點 upper。
 * 請找出所有落在 [lower, upper] 區間內且未出現在 nums 中的遺失數字，
 * 並將連續的遺失數字以區間形式印出。
 *
 * 格式規範：
 * 1. 若遺失的數字只有單獨一個 x，格式為 "x"
 * 2. 若遺失的數字為連續區間 [l, r]，格式為 "l->r"
 * 3. 每個印出的區間以中括號包覆，例如：["2", "4->49", "51->74", "76->99"]
 *
 * 限制與要求：
 * 1. 時間複雜度必須為 O(N)，空間複雜度必須為 O(1)。
 * 2. 需妥善處理 lower 與 upper 的邊界溢位防護（如 INT_MAX、INT_MIN）。
 *
 * 範例 1：
 * nums = [0, 1, 3, 50, 75], lower = 0, upper = 99
 * 輸出：["2", "4->49", "51->74", "76->99"]
 *
 * 範例 2：
 * nums = [-1], lower = -1, upper = -1
 * 輸出：[]
 *
 * 範例 3：
 * nums = [], lower = 1, upper = 1
 * 輸出：["1"]
 */

/*
 * =============================================================================
 * 【版本一：使用者思路訂正版（完全不 malloc，直接以 printf 串流輸出）】
 * =============================================================================
 */
void printMissingRanges(const int *nums, int numsSize, long long lower, long long upper) {
    int first = 1;
    printf("[");

    // 特例防禦：若 nums 為空（或長度為 0），整個 [lower, upper] 就是遺失區間
    if (nums == NULL || numsSize == 0) {
        if (lower == upper) {
            printf("\"%lld\"", lower);
        } else {
            printf("\"%lld->%lld\"", lower, upper);
        }
    } else {
        // 第一段（沿用您的思維）：處理 lower 到 nums[0] 之間的缺失
        if ((long long)nums[0] > lower) {
            long long start = lower;
            long long end = (long long)nums[0] - 1;
            if (!first) printf(", ");
            if (start == end) {
                printf("\"%lld\"", start);
            } else {
                printf("\"%lld->%lld\"", start, end);
            }
            first = 0;
        }

        // 第二段（沿用您的思維）：處理 nums 陣列內部相鄰元素之間的缺失
        for (int i = 0; i < numsSize - 1; i++) {
            if ((long long)nums[i + 1] - nums[i] > 1) {
                long long start = (long long)nums[i] + 1;
                long long end = (long long)nums[i + 1] - 1;
                if (!first) printf(", ");
                if (start == end) {
                    printf("\"%lld\"", start);
                } else {
                    printf("\"%lld->%lld\"", start, end);
                }
                first = 0;
            }
        }

        // 第三段（沿用您的思維）：處理 nums[numsSize - 1] 到 upper 之間的缺失
        if ((long long)nums[numsSize - 1] < upper) {
            long long start = (long long)nums[numsSize - 1] + 1;
            long long end = upper;
            if (!first) printf(", ");
            if (start == end) {
                printf("\"%lld\"", start);
            } else {
                printf("\"%lld->%lld\"", start, end);
            }
            first = 0;
        }
    }

    printf("]\n");
}

/*
 * =============================================================================
 * 【版本二：極簡 O(1) 串流輸出解法（韌體最愛，完全不 malloc，邊比對邊 printf）】
 * =============================================================================
void printMissingRanges_Optimal(const int *nums, int numsSize, long long lower, long long upper) {
    // 【核心指針：點名計數器】心裡預期下一個應該出現的數字，從 lower 開始點名
    long long next = lower;
    int first = 1; // 控制逗號輸出的旗標（第一筆資料前不印逗號，之後每筆印 ", "）

    printf("["); // 印出外層左中括號

    for (int i = 0; i < numsSize; i++) {
        // 1. 【防禦過濾】：若陣列中的數字小於預期起點（例如負數小於 lower），直接略過
        if (nums[i] < next) continue;
        
        // 2. 【範圍剪枝】：若陣列中的數字已經大於 upper 範圍，提早退出迴圈
        if (nums[i] > upper) break;

        // 3. 【抓出缺席區間】：若來的數字大於心裡預期的 next，代表 [next, nums[i] - 1] 全部缺席！
        if (nums[i] > next) {
            long long start = next;          // 缺失區間起點
            long long end = nums[i] - 1;     // 缺失區間終點

            if (!first) printf(", ");        // 非第一筆時補印逗號分隔符
            if (start == end) {
                printf("\"%lld\"", start);   // 單一缺席數字，格式為 "x"
            } else {
                printf("\"%lld->%lld\"", start, end); // 連續區間缺席，格式為 "start->end"
            }
            first = 0; // 標記已輸出至少一筆資料
        }

        // 4. 【更新預期】：既然 nums[i] 已經出現，下一個預期要看見的數字就是 nums[i] + 1
        next = (long long)nums[i] + 1;
    }

    // 5. 【收尾檢查】：陣列全部點名完後，若預期號碼尚未達到 upper（例如 nums 結束在 75，但 upper 是 99）
    // 代表最後殘餘的 [next, upper] 全部缺席（亦完美相容 nums 為空陣列的情況）！
    if (next <= upper) {
        long long start = next;
        long long end = upper;

        if (!first) printf(", ");
        if (start == end) {
            printf("\"%lld\"", start);
        } else {
            printf("\"%lld->%lld\"", start, end);
        }
    }

    printf("]\n"); // 印出結尾右中括號並換行
}
 * =============================================================================
 */

/*
================================================================================
【原始白板代碼保留與盲點詳細剖析】
================================================================================
void printMissingRanges_Original(const int *nums, int numsSize, long long lower, long long upper) {
    // ❌ 盲點 1：函式原型為 void，不需回傳 char**。
    // 在嵌入式系統中，字串動態分配需特別注意空間大小與 free() 釋放。
    char** result = (char**) malloc((numsSize + 1) * sizeof(char*));
    int start_bit = 0;
    while(start_bit < numsSize && nums[start_bit] < lower)
    {
        start_bit++;
    }
    int curr = 0;
    // ❌ 盲點 2：當 nums 為空時，start_bit >= numsSize 成立，
    // 此處提早 return result 會導致空陣列時原本應輸出的 ["1"] 完全漏印。
    if(start_bit >= numsSize)
    {
        result[curr] = (char*) malloc(32 * sizeof(char));
        sprintf(result[curr], "");
        return result; 
    }
    long long start = lower;
    long long end = nums[start_bit] - 1;
    if(nums[start_bit] > lower)
    {
        result[curr] = (char*) malloc(32 * sizeof(char));
        if(start == end)
        {
            sprintf(result[curr], "%d", start);
        }
        else
        {
            sprintf(result[curr], "%d->%d", start, end);
        }
        curr++;
    }
    while(start_bit < numsSize - 1)
    {
        if(nums[start_bit + 1] - nums[start_bit] == 1)
        {
            start_bit++;
        }
        else
        {
            start = nums[start_bit] + 1;
            end = nums[start_bit + 1] - 1;
            if(start == end)
            {
                sprintf(result[curr], "%d", start);
            }
            else
            {
                sprintf(result[curr], "%d->%d", start, end);
            }
            curr++;
            start_bit++;
        }
    }
    if(nums[start_bit] < upper)
    {
        start = nums[start_bit] + 1;
        end = upper;
        if(start == end)
        {
            sprintf(result[curr], "%d", start);
        }
        else
        {
            sprintf(result[curr], "%d->%d", start, end);
        }
        // ❌ 盲點 3：第三段若有放進 result，漏了 curr++！
    }

    for(int i = 0; i < curr; i++)
    {
        //print結果  <-- 此處未完成
    }
}
================================================================================
*/

int main() {
    printf("=== Test 1 ===\n");
    int nums1[] = {0, 1, 3, 50, 75};
    printf("Actual:   ");
    printMissingRanges(nums1, 5, 0, 99);
    printf("Expected: [\"2\", \"4->49\", \"51->74\", \"76->99\"]\n");

    printf("\n=== Test 2 ===\n");
    int nums2[] = {-1};
    printf("Actual:   ");
    printMissingRanges(nums2, 1, -1, -1);
    printf("Expected: []\n");

    printf("\n=== Test 3 ===\n");
    printf("Actual:   ");
    printMissingRanges(NULL, 0, 1, 1);
    printf("Expected: [\"1\"]\n");

    printf("\n=== Test 4 ===\n");
    int nums4[] = {10, 20};
    printf("Actual:   ");
    printMissingRanges(nums4, 2, 0, 30);
    printf("Expected: [\"0->9\", \"11->19\", \"21->30\"]\n");

    return 0;
}
