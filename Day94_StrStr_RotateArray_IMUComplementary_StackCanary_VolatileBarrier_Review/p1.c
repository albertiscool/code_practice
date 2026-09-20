#include <stdio.h>
#include <string.h>

// ============================================================================
// 題目 1【找出字串中第一個匹配項的下標 (Find the Index of the First Occurrence in a String - LeetCode #28)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (C 語言標準函式庫 strstr 底層手寫經典考題)
// 題目說明：給你兩個字串 haystack 和 needle，請你在 haystack 字串中找出 needle 
//           字串的第一個匹配項的下標（下標從 0 開始）。如果 needle 不是 haystack 的一部分，則返回 -1。
// 範例 1：
//    輸入：haystack = "sadbutsad", needle = "sad"
//    輸出：0
//    解釋："sad" 在下標 0 和 6 處匹配。第一個匹配項的下標是 0，所以返回 0。
// 範例 2：
//    輸入：haystack = "leetcode", needle = "leeto"
//    輸出：-1
//    解釋："leeto" 沒有在 "leetcode" 中出現，所以返回 -1。
// 限制條件：
//    - 1 <= haystack.length, needle.length <= 10^4
//    - haystack 和 needle 僅由小寫英文字母組成。
//    - ⚠️ 嚴禁使用 C 語言庫函式 strstr()！
// ============================================================================

int strStr(char* haystack, char* needle) {
    /*
     * 正確解答 (滑動窗口 / 暴力雙指標字串匹配 - 時間 O((N - M + 1) * M)、空間 O(1)):
     * 1. 外層迴圈 i 枚舉 haystack 中所有可能的起點 (只需檢查到 len_hay - len_needle)。
     * 2. 內層比對以 i 為起點的長度 M 子字串是否與 needle 完全吻合。
     * 3. 只要一比對完成 (j == len_needle)，立刻回傳起始下標 i (保證為第一個匹配項)。
     */
    if (haystack == NULL || needle == NULL) {
        return -1;
    }
    int len_hay = strlen(haystack);
    int len_needle = strlen(needle);

    if (len_needle == 0) {
        return 0;
    }
    if (len_hay < len_needle) {
        return -1;
    }

    for (int i = 0; i <= len_hay - len_needle; i++) {
        int j = 0;
        while (j < len_needle && haystack[i + j] == needle[j]) {
            j++;
        }
        if (j == len_needle) {
            return i; // 找到第一個完整匹配項，立即返回
        }
    }

    return -1;
}

/* ============================================================================
 * ❌ 原始實作盲點分析 (已保留供複習檢討):
 * ============================================================================
 * int strStr_wrong(char* haystack, char* needle) {
 *     if(haystack == NULL || needle == NULL) return -1;
 *     int len_hay = strlen(haystack);
 *     int len_needle = strlen(needle);
 *     int start = -1;
 *     int curr_n = 0;
 *     for(int i = 0; i < len_hay; i++)
 *     {
 *         char h = haystack[i];
 *         if(needle[curr_n] == h && start == -1)
 *         {
 *             curr_n++;
 *             start = i;
 *         }
 *         else if(needle[curr_n] == h && start != -1)
 *         {
 *             curr_n++;
 *         }
 *         else
 *         {
 *             curr_n = 0;
 *             start = -1;
 *         }
 *     }
 *     // ⚠️ 盲點 1: 缺少「完全匹配成功時立即返回」的判定！
 *     //           當 curr_n 達到 len_needle 時，迴圈沒有 break/return，而是繼續往後跑。
 *     //           這會導致:
 *     //           (1) 後面讀到 needle[len_needle] 的 '\0' 觸發 else，將剛才配對成功的 start 誤洗成 -1。
 *     //           (2) 若字串後方再次出現相同的 needle (如 "sadbutsad")，原本第 0 格的匹配會被後面
 *     //               第 6 格的匹配覆蓋，導致回傳最後一個匹配 (6) 而非題目要求的第一個匹配 (0)。
 *     //
 *     // ⚠️ 盲點 2: 單迴圈缺乏「失配回溯 (Backtracking)」機制！
 *     //           例如 "mississippi" 搜尋 "issip"：
 *     //           在比對到 "mississi" 時發生不匹配，此時 i 已經走到 5，直接 curr_n=0 且 i 繼續往前，
 *     //           會漏掉從 index 4 重新開始的正確匹配 "issip"！
 *     //           字串比對標準作法為雙層迴圈：外層枚舉起點 i，內層比對字元。
 *     return start;
 * }
 * ============================================================================ */

int main(void) {
    /* 測試測資 1: "sadbutsad" 搜尋 "sad" -> 0 */
    char h1[] = "sadbutsad";
    char n1[] = "sad";
    int ans1 = strStr(h1, n1);
    printf("測資 1 結果: %d (預期: 0)\n", ans1);

    /* 測試測資 2: "leetcode" 搜尋 "leeto" -> -1 */
    char h2[] = "leetcode";
    char n2[] = "leeto";
    int ans2 = strStr(h2, n2);
    printf("測資 2 結果: %d (預期: -1)\n", ans2);

    /* 測試測資 3: "mississippi" 搜尋 "issip" -> 4 */
    char h3[] = "mississippi";
    char n3[] = "issip";
    int ans3 = strStr(h3, n3);
    printf("測資 3 結果: %d (預期: 4)\n", ans3);

    /* 測試測資 4: "a" 搜尋 "a" -> 0 */
    char h4[] = "a";
    char n4[] = "a";
    int ans4 = strStr(h4, n4);
    printf("測資 4 結果: %d (預期: 0)\n", ans4);

    return 0;
}
