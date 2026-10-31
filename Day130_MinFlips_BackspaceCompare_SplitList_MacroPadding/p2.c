#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*
 * LeetCode #844 - Backspace String Compare
 *
 * Description:
 * Given two strings s and t, return true if they are equal when both are typed into
 * empty text editors. '#' means a backspace character.
 *
 * Note that after backspacing an empty text, the text will continue empty.
 *
 * Constraints:
 * 1 <= s.length, t.length <= 200
 * s and t only contain lowercase letters and '#' characters.
 *
 * Follow-up:
 * Can you solve it in O(n) time and O(1) space?
 */

bool backspaceCompare(char* s, char* t) {
    int curr_s = strlen(s) - 1;
    int curr_t = strlen(t) - 1;
    int count_s = 0;
    int count_t = 0;

    while (curr_s >= 0 || curr_t >= 0) {
        // 尋找 s 的下一個存活字元
        while (curr_s >= 0) {
            if (s[curr_s] == '#') {
                count_s++;
                curr_s--;
            } else if (count_s > 0) {
                curr_s--;
                count_s--;
            } else {
                break;
            }
        }

        // 尋找 t 的下一個存活字元
        while (curr_t >= 0) {
            if (t[curr_t] == '#') {
                count_t++;
                curr_t--;
            } else if (count_t > 0) {
                curr_t--;
                count_t--;
            } else {
                break;
            }
        }

        // 比對兩端存活字元
        if (curr_s >= 0 && curr_t >= 0) {
            if (s[curr_s] != t[curr_t]) {
                return false;
            }
        } else if ((curr_s >= 0) != (curr_t >= 0)) {
            // 一邊還有存活字元，另一邊已為空字串
            return false;
        }

        // 本輪字元比對成功（或雙方皆已為負），兩指標同時往前推進
        curr_s--;
        curr_t--;
    }
    return true;
}

/*
 * =========================================================================
 * 原始白板實作與盲點深度剖析 (Preserved User's Original Code & Blind Spots):
 * =========================================================================
 * 原始程式碼：
 * bool backspaceCompare(char* s, char* t) {
 *     int curr_s = strlen(s) - 1;
 *     int curr_t = strlen(t) - 1;
 *     int count_s = 0;
 *     int count_t = 0;
 *     while(curr_s > 0 || curr_t > 0)          // 盲點 1: > 0 漏掉索引 0
 *     {
 *         while(curr_s > 0)                   // 盲點 1: > 0 漏掉索引 0
 *         {
 *             if(s[curr_s] == '#') { count_s++; curr_s--; }
 *             else if(count_s > 0) { curr_s--; count_s--; }
 *             else { break; }
 *         }
 *         while(curr_t > 0)                   // 盲點 1: > 0 漏掉索引 0
 *         {
 *             if(t[curr_t] == '#') { count_t++; curr_t--; }
 *             else if(count_t > 0) { curr_t--; count_t--; }
 *             else { break; }
 *         }
 * 
 *         if((curr_s >= 0) && (curr_t >= 0))
 *         {
 *             if(s[curr_s] != t[curr_t]) return false;
 *             // 盲點 2: 當 s[curr_s] == t[curr_t] 時未推進指針，導致死迴圈！
 *         }
 *         else if((curr_s >= 0) != (curr_t >= 0))
 *         {
 *             return false;
 *         }
 *         else
 *         {
 *             curr_s--;
 *             curr_t--;
 *         }
 *     }
 *     return true;
 * }
 *
 * 盲點深度剖析：
 * 1. 指標推進與分支結構陷阱 (Deadlock / Infinite Loop)：
 *    在原本的結構中：
 *    if ((curr_s >= 0) && (curr_t >= 0)) {
 *        if (s[curr_s] != t[curr_t]) return false;
 *    } else if (...) { ... }
 *    else { curr_s--; curr_t--; }
 *    當比對成功 (s[curr_s] == t[curr_t]) 時，程式不會進入 return false，
 *    但因為它命中了外層 if，所以會跳過最後的 else (curr_s--; curr_t--;)！
 *    結果：curr_s 與 curr_t 完全沒有變動，下一輪迴圈又停留在同一個位置比對，
 *    直接陷入無窮死迴圈 (Infinite Loop)！
 *    正解：比對成功後，curr_s--; curr_t--; 必須在 if-else 之外無條件執行。
 *
 * 2. 邊界條件之 Off-by-one 漏判索引 0：
 *    條件寫成 curr_s > 0 會導致當指標走到字串的第一個字元 (index 0) 時提前結束迴圈，
 *    完全沒有檢查到 index 0 的字元是否為 '#' 或是存活字元。
 *    例如 "a" 與 "a"，當 index 為 0 時迴圈停止，造成嚴重漏判。
 *    陣列與字串下標從 0 開始，因此條件必須是 curr_s >= 0 與 curr_t >= 0。
 * =========================================================================
 */

int main(void) {
    // Test Case 1: s = "ab#c", t = "ad#c" -> Both become "ac" -> true
    char s1[] = "ab#c";
    char t1[] = "ad#c";
    printf("Test 1: %s (Expected: true)\n", backspaceCompare(s1, t1) ? "true" : "false");

    // Test Case 2: s = "ab##", t = "c#d#" -> Both become "" -> true
    char s2[] = "ab##";
    char t2[] = "c#d#";
    printf("Test 2: %s (Expected: true)\n", backspaceCompare(s2, t2) ? "true" : "false");

    // Test Case 3: s = "a#c", t = "b" -> s becomes "c", t becomes "b" -> false
    char s3[] = "a#c";
    char t3[] = "b";
    printf("Test 3: %s (Expected: false)\n", backspaceCompare(s3, t3) ? "true" : "false");

    // Test Case 4: s = "a##c", t = "#a#c" -> Both become "c" -> true
    char s4[] = "a##c";
    char t4[] = "#a#c";
    printf("Test 4: %s (Expected: true)\n", backspaceCompare(s4, t4) ? "true" : "false");

    return 0;
}
