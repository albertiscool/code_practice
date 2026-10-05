#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 題目 1【合併兩個有序陣列 (Merge Sorted Array - LeetCode #88)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (一線 IC 設計廠/科技廠韌體白板必考題)
// 題目說明：給你兩個按「非遞減順序」排列的整數陣列 nums1 和 nums2，
//           以及兩個整數 m 和 n，分別表示 nums1 和 nums2 中的有效元素數目。
//           請你將 nums2 合併到 nums1 中，使合併後的陣列同樣按「非遞減順序」排列。
// 注意事項：
//    - 最終合併後的陣列不應由函式返回，而是就地 (in-place) 儲存在陣列 nums1 中。
//    - nums1 的空間大小為 m + n，其中前 m 個元素為應當合併的元素，後 n 個元素為 0，應予忽略。
// 範例 1：
//    輸入：nums1 = [1,2,3,0,0,0], m = 3, nums2 = [2,5,6], n = 3
//    輸出：nums1 = [1,2,2,3,5,6]
// 範例 2：
//    輸入：nums1 = [1], m = 1, nums2 = [], n = 0
//    輸出：nums1 = [1]
// 範例 3：
//    輸入：nums1 = [0], m = 0, nums2 = [1], n = 1
//    輸出：nums1 = [1]
// 限制條件：
//    - nums1.length == m + n
//    - nums2.length == n
//    - 0 <= m, n <= 200
//    - 1 <= m + n <= 200
//    - -10^9 <= nums1[i], nums2[j] <= 10^9
// ============================================================================

void merge(int* nums1, int nums1Size, int m, int* nums2, int nums2Size, int n) {
    /*
     * 正確解答 (從後往前雙指標 / 三指標技巧):
     * 1. p1 指向 nums1 的最後一個有效元素 (m - 1)
     * 2. p2 指向 nums2 的最後一個有效元素 (n - 1)
     * 3. p  指向 nums1 的最末尾填入位置 (m + n - 1)
     * 只要 nums2 還有元素 (p2 >= 0)，就比較兩者並由大到小從後方往前填入。
     * 若 nums2 全數填完，nums1 剩下的元素原本就已在其正確位置，無需額外搬移！
     */
    int p1 = m - 1;
    int p2 = n - 1;
    int p = m + n - 1;

    while (p2 >= 0) {
        if (p1 >= 0 && nums1[p1] > nums2[p2]) {
            nums1[p--] = nums1[p1--];
        } else {
            nums1[p--] = nums2[p2--];
        }
    }
}

/* ============================================================================
 * ❌ 原始實作盲點分析 (已保留供複習檢討):
 * ============================================================================
 * void merge_wrong(int* nums1, int nums1Size, int m, int* nums2, int nums2Size, int n) {
 *     if(nums1 == NULL) return;
 *     if(nums2 == NULL) return;
 * 
 *     // ⚠️ 盲點 1 (語法錯誤): 變數生命週期與 Scope 問題
 *     // curr_m 與 curr_n 被宣告在 if 與 else 的大括號內部，離開大括號後該變數便不存在！
 *     // 這會直接導致編譯失敗: error: 'curr_m' undeclared / error: 'curr_n' undeclared。
 *     if(m == 0)
 *     {
 *         int curr_m = 1;
 *     }
 *     else
 *     {
 *         int curr_m = m - 1;
 *         int curr_n = n - 1;
 *     }
 * 
 *     // ⚠️ 盲點 2 (邊界與指標耗盡條件):
 *     // 當陣列其中一方先被消耗完畢時 (例如 curr_m < 0 或 curr_n < 0)，
 *     // 直接做 nums1[curr_m] > nums2[curr_n] 會引發負數索引越界訪問 (Out-of-Bounds)！
 *     // 正確做法是將終止條件綁在 while (p2 >= 0)，並在取用 p1 前先檢查 p1 >= 0。
 *     for(int curr = m + n - 1; curr >= 0; curr--)
 *     {
 *         if(nums1[curr_m] > nums2[curr_n])
 *         {
 *             nums1[curr] = nums1[curr_m];
 *             curr_m--;
 *         }
 *         else
 *         {
 *             nums1[curr] = nums2[curr_n];
 *             curr_n--;
 *         }
 *     }
 * }
 * ============================================================================ */

int main(void) {
    /* 測試測資 1: [1,2,3,0,0,0] (m=3) 與 [2,5,6] (n=3) -> [1,2,2,3,5,6] */
    int nums1_1[6] = {1, 2, 3, 0, 0, 0};
    int nums2_1[3] = {2, 5, 6};
    merge(nums1_1, 6, 3, nums2_1, 3, 3);
    printf("測資 1 結果: ");
    for (int i = 0; i < 6; i++) {
        printf("%d ", nums1_1[i]);
    }
    printf("(預期: 1 2 2 3 5 6)\n");

    /* 測試測資 2: [1] (m=1) 與 [] (n=0) -> [1] */
    int nums1_2[1] = {1};
    int nums2_2[1] = {0};
    merge(nums1_2, 1, 1, nums2_2, 0, 0);
    printf("測資 2 結果: %d (預期: 1)\n", nums1_2[0]);

    /* 測試測資 3: [0] (m=0) 與 [1] (n=1) -> [1] */
    int nums1_3[1] = {0};
    int nums2_3[1] = {1};
    merge(nums1_3, 1, 0, nums2_3, 1, 1);
    printf("測資 3 結果: %d (預期: 1)\n", nums1_3[0]);

    return 0;
}
