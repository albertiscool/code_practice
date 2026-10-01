#include <stdio.h>
#include <stdlib.h>

/**
 * ============================================================================
 * 【Day 104 - 題目一】合併區間 (LeetCode #56 - Merge Intervals)
 * 【LeetCode 難度】Medium (中等)
 * ============================================================================
 * 【題目說明】
 * 以陣列 intervals 表示若干個區間的集合，其中單個區間為 intervals[i] = [start_i, end_i]。
 * 請合併所有重疊的區間，並回傳一個不重疊的區間陣列，該陣列需恰好覆蓋輸入中的所有區間。
 * 
 * 【範例 1】
 * 輸入: intervals = [[1,3],[2,6],[8,10],[15,18]]
 * 輸出: [[1,6],[8,10],[15,18]]
 * 解釋: 區間 [1,3] 和 [2,6] 重疊，將它們合併為 [1,6]。
 * 
 * 【範例 2】
 * 輸入: intervals = [[1,4],[4,5]]
 * 輸出: [[1,5]]
 * 解釋: 區間 [1,4] 和 [4,5] 被視為重疊區間。
 * 
 * 【限制條件】
 * 1. 1 <= intervals.length <= 10^4
 * 2. intervals[i].length == 2
 * 3. 0 <= start_i <= end_i <= 10^4
 * ============================================================================
 */

// qsort 比較函式：依照區間起點 (start) 由小到大排序
int cmp(const void* a, const void* b) {
    int* intervalA = *(int**)a;
    int* intervalB = *(int**)b;
    return intervalA[0] - intervalB[0];
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** merge(int** intervals, int intervalsSize, int* intervalsColSize, int* returnSize, int** returnColumnSizes) {
    if (intervalsSize <= 0) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    // 1. 關鍵步驟：必須先依照區間的 start 由小到大排序
    qsort(intervals, intervalsSize, sizeof(int*), cmp);

    // 2. 配置回傳結果空間與每個區間的大小陣列
    int** res = (int**)malloc(sizeof(int*) * intervalsSize);
    *returnColumnSizes = (int*)malloc(sizeof(int) * intervalsSize);

    // 先將第一個區間放進結果中
    int count = 0;
    res[count] = (int*)malloc(sizeof(int) * 2);
    res[count][0] = intervals[0][0];
    res[count][1] = intervals[0][1];
    (*returnColumnSizes)[count] = 2;
    count++;

    // 3. 遍歷後續所有區間進行合併
    for (int i = 1; i < intervalsSize; i++) {
        int curr_start = intervals[i][0];
        int curr_end = intervals[i][1];
        int last_end = res[count - 1][1];

        if (curr_start <= last_end) {
            // 重疊：更新最後一個區間的終點為兩者較大值
            if (curr_end > last_end) {
                res[count - 1][1] = curr_end;
            }
        } else {
            // 無重疊：新增一個獨立區間
            res[count] = (int*)malloc(sizeof(int) * 2);
            res[count][0] = curr_start;
            res[count][1] = curr_end;
            (*returnColumnSizes)[count] = 2;
            count++;
        }
    }

    *returnSize = count;
    return res;
}

/*
===============================================================================
【原始程式碼盲點與改進分析】
-------------------------------------------------------------------------------
1. 未進行區間排序（核心演算法致命傷）：
   - 原寫法假設輸入區間已經依起點排好，但 LeetCode 測資可能為亂序（例如 [[2,6],[1,3]]）。
   - 沒有先做 qsort 排序，相鄰比較 `intervals[i+1]` 會徹底漏掉跨區間重疊。

2. 區間包含（Covering）更新錯誤：
   - 原寫法：`intervals[i][1] = intervals[i+1][1];`
   - 若遇到 [1, 4] 與 [2, 3]：
     合併後應為 [1, 4]，但原寫法直接將終點蓋成 3，變成 [1, 3]，發生邏輯縮小錯誤！
     正確作法：終點應取兩者較大值 `max(last_end, curr_end)`。

3. C 語言指標優先權陷阱（面試天坑）：
   - 原寫法：`*returnSize--;`
   - 在 C 語言中，後置遞減 `--` 優先權高於指標解參考 `*`！
   - 所以 `*returnSize--` 等同於 `*(returnSize--)`，它修改的是「指標本身指向的位址」，
     而不是「指標所指向的整數數值」！必須寫成 `(*returnSize)--;`。

4. 二維動態記憶體與 Segfault：
   - 原寫法：`**returnColumnSizes = 2;`
   - `returnColumnSizes` 是二級指標，此時 `*returnColumnSizes` 還是 NULL，
     直接對它兩次解參考會觸發記憶體段錯誤（Segmentation Fault）直接死機。
   - 必須先為其 malloc 空間：`*returnColumnSizes = malloc(sizeof(int) * count);`。

5. 記憶體釋放與 Use-After-Free：
   - 原寫法：在原陣列上 `free(intervals[i+1])`，下一輪迴圈 `i` 遞增後，
     `intervals[i]` 便存取到了剛剛已被 free 掉的記憶體，造成未定義行為（Use-After-Free）。
   - 原寫法回傳 `return **intervals;`，其型態為 `int`，與函式要求的 `int**` 嚴重不合。
   - 正確規範：另行配置 `res` 陣列收集結果回傳。

-------------------------------------------------------------------------------
【原始程式碼存檔】
int** merge(int** intervals, int intervalsSize, int* intervalsColSize, int* returnSize, int** returnColumnSizes) {
    // 請在此處撰寫你的程式碼
    *returnSize = intervalsSize;
    **returnColumnSizes = 2;
    for(int i = 0; i < intervalsSize - 1; i++)
    {
        if(intervals[i][1] >= intervals[i+1][0])
        {
            intervals[i][1] = intervals[i+1][1];
            free(intervals[i+1]);
            *returnSize--;
        }
    }
    return **intervals;
}
===============================================================================
*/
