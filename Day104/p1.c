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

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** merge(int** intervals, int intervalsSize, int* intervalsColSize, int* returnSize, int** returnColumnSizes) {
    // 請在此處撰寫你的程式碼
    
}
