#include <stdio.h>
#include <string.h>


/**
 * ============================================================================
 * 題目三：無重複字元的最長子字串 (LeetCode #3 - Longest Substring Without Repeating Characters)
 * ============================================================================
 * 【題目說明】
 * 給定一個字串 s，請找出其中「不含有重複字元」的最長子字串的長度。
 * 
 * 【範例 1】
 * 輸入: s = "abcabcbb"
 * 輸出: 3
 * 解釋: 因為無重複字元的最長子字串是 "abc"，其長度為 3。
 * 
 * 【範例 2】
 * 輸入: s = "bbbbb"
 * 輸出: 1
 * 解釋: 因為無重複字元的最長子字串是 "b"，其長度往後看最長無重複為 1。
 * 
 * 【範例 3】
 * 輸入: s = "pwwkew"
 * 輸出: 3
 * 解釋: 因為無重複字元的最長子字串是 "wke"，其長度為 3。
 *       請注意，你的答案必須是「子字串 (Substring)」的長度，
 *       "pwke" 是一個子序列 (Subsequence)，並不是子字串。
 * 
 * 【範例 4】
 * 輸入: s = ""
 * 輸出: 0
 * 
 * 【限制條件】
 * 1. 0 <= s.length <= 5 * 10^4
 * 2. s 由英文字母、數字、符號與空格組成。
 * ============================================================================
 */

int lengthOfLongestSubstring(char* s) {
    if (s == NULL || *s == '\0') {
        return 0;
    }

    // 盲點 1 修正：在 C 語言中，int arr[256] = {-1} 只有第 0 格是 -1，其餘 255 格都是 0！
    // 必須使用 memset 將記憶體全部填入 -1 (0xFF)
    int last_pos[256];
    memset(last_pos, -1, sizeof(last_pos));

    int slow = 0;
    int max_len = 0;
    int len = strlen(s);

    for (int fast = 0; fast < len; fast++) {
        // 使用 unsigned char 防止 char 為 signed 導致負數陣列越界
        unsigned char c = (unsigned char)s[fast];

        // 盲點 2 & 3 修正：
        // 只有當這個字元「之前出現過」且「位置在當前窗口內部 (>= slow)」時，slow 才需要跳躍！
        // 否則如果該字元在窗口左側 (如 "abba" 例子)，slow 絕對不能往回退！
        if (last_pos[c] >= slow) {
            slow = last_pos[c] + 1;
        }

        // 核心關鍵：無論有沒有重複，每一輪都要更新該字元「最後一次出現的位置」！
        last_pos[c] = fast;

        int curr_len = fast - slow + 1;
        if (curr_len > max_len) {
            max_len = curr_len;
        }
    }

    return max_len;
}

int main(void) {
    printf("=== Day 102 P3: Longest Substring Without Repeating Characters 驗證 ===\n");

    // 測試 1: "abcabcbb" -> 3 ("abc")
    char s1[] = "abcabcbb";
    printf("測試 1 (\"%s\"): %d (預期: 3)\n", s1, lengthOfLongestSubstring(s1));

    // 測試 2: "bbbbb" -> 1 ("b")
    char s2[] = "bbbbb";
    printf("測試 2 (\"%s\"): %d (預期: 1)\n", s2, lengthOfLongestSubstring(s2));

    // 測試 3: "pwwkew" -> 3 ("wke")
    char s3[] = "pwwkew";
    printf("測試 3 (\"%s\"): %d (預期: 3)\n", s3, lengthOfLongestSubstring(s3));

    // 測試 4: "" -> 0
    char s4[] = "";
    printf("測試 4 (\"%s\"): %d (預期: 0)\n", s4, lengthOfLongestSubstring(s4));

    // 測試 5: "abba" -> 2 (面試經典倒退陷阱測試)
    char s5[] = "abba";
    printf("測試 5 (\"%s\"): %d (預期: 2)\n", s5, lengthOfLongestSubstring(s5));

    // 測試 6: "tmmzuxt" -> 5 ("mzuxt")
    char s6[] = "tmmzuxt";
    printf("測試 6 (\"%s\"): %d (預期: 5)\n", s6, lengthOfLongestSubstring(s6));

    return 0;
}

/*
 * ============================================================================
 * 【原始白板盲點複盤 (Original Blind Spots)】
 * ============================================================================
 * 盲點 1：C 語言陣列初始化語法陷阱
 *   原始寫法：
 *     int last_pos[256] = {-1};
 *   問題：
 *     在 C 語言中，`int arr[N] = {val}` 只有第 0 個元素被初始化為 val，
 *     剩下的 N-1 個元素全被自動填入 0！
 *     這導致 last_pos[1..255] 全部是 0 而不是 -1，遇到非 '\0' 字元時誤以為在 index 0 出現過。
 *   正解：
 *     使用 `memset(last_pos, -1, sizeof(last_pos));`
 * 
 * 盲點 2：重複時未更新字元的最新位置
 *   原始寫法在 else 分支更新了 slow，但忘了執行 `last_pos[s[fast]] = fast;`。
 *   這導致字元如果出現第 3 次，依然讀取到很久以前的舊位置。
 *   正解：
 *     無論是否重複，每一輪都要更新 `last_pos[c] = fast;`。
 * 
 * 盲點 3：經典「abba 倒退陷阱」
 *   考慮字串 "abba"：
 *   - fast 走到第 2 個 'b' (index 2) 時，slow 跳到 index 2 (窗口為 "b")。
 *   - fast 走到第 2 個 'a' (index 3) 時，若直接 `slow = last_pos['a'] + 1`，
 *     slow 會從 2 倒退回 0 + 1 = 1！導致窗口變成 "bba"（出現重複 'b'）。
 *   正解：
 *     必須檢查 `if (last_pos[c] >= slow)`，只有當重複字元出現在「當前窗口內部」時才跳躍！
 * ============================================================================
 */
