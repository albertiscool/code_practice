#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * LeetCode #557 - Reverse Words in a String III
 * 
 * 題目描述：
 * 給定一個字串 s，請反轉字串中每個單詞內的字元順序，
 * 同時仍保留空格以及所有單詞的原始先後順序。
 * 
 * 範例 1：
 * 輸入：s = "Let's take LeetCode contest"
 * 輸出："s'teL ekat edoCteeL tsetnoc"
 * 
 * 範例 2：
 * 輸入：s = "Mr Ding"
 * 輸出："rM gniD"
 * 
 * 限制條件：
 * 1 <= s.length <= 5 * 10^4
 * s 包含可列印的 ASCII 字元
 * s 不包含任何前導或尾隨空格
 * s 中的所有單詞都由單一空格分隔
 */

/* =========================================================================
 * 【使用者原始白板程式碼 (原始未修改版本 - 一次 AC / 0 ms)】
 * -------------------------------------------------------------------------
 * char* reverseWords(char* s) {
 *     int slow = 0;
 *     int fast = 0;
 *     while(s[fast] != '\0')
 *     {
 *         if(s[fast] == ' ')
 *         {
 *             int fast_next = fast + 1;
 *             fast = fast - 1;
 *             while(slow < fast)
 *             {
 *                 char temp = s[slow];
 *                 s[slow] = s[fast];
 *                 s[fast] = temp;
 *                 slow++;
 *                 fast--;
 *             }
 *             slow = fast_next;
 *             fast = fast_next;
 *         }
 *         else
 *         {
 *             fast++;
 *         }
 *     }
 *     fast = fast - 1;
 *     while(slow < fast)
 *     {
 *         char temp = s[slow];
 *         s[slow] = s[fast];
 *         s[fast] = temp;
 *         slow++;
 *         fast--;
 *     }
 *     return s;
 * }
 * -------------------------------------------------------------------------
 * 【設計亮點剖析】：
 * 1. 雙指針滑動視窗 (Sliding Window Two Pointers)：
 *    - `slow` 標記當前單詞的起始字元，`fast` 向前探測單詞結尾邊界（' ' 或 '\0'）。
 * 2. 極致原地翻轉 (In-Place Inversion)：
 *    - 在原始字串緩衝區上直接執行首尾對調，不額外宣告任何輔助陣列，輔助空間複雜度達 O(1)！
 * 3. 完美的末端邊界處理：
 *    - 離開主迴圈後，嚴謹補上最後一個單詞的翻轉，杜絕經典的末端單詞漏反轉 Bug！
 * 4. 遠端實測：29/29 全測資 Accepted，執行時間 0 ms (Beats 100%)！
 * =========================================================================
 */

char* reverseWords(char* s) {
    int slow = 0;
    int fast = 0;
    while (s[fast] != '\0') {
        if (s[fast] == ' ') {
            int fast_next = fast + 1;
            fast = fast - 1;
            while (slow < fast) {
                char temp = s[slow];
                s[slow] = s[fast];
                s[fast] = temp;
                slow++;
                fast--;
            }
            slow = fast_next;
            fast = fast_next;
        } else {
            fast++;
        }
    }
    // 末端單詞收尾翻轉
    fast = fast - 1;
    while (slow < fast) {
        char temp = s[slow];
        s[slow] = s[fast];
        s[fast] = temp;
        slow++;
        fast--;
    }
    return s;
}

int main() {
    char s1[] = "Let's take LeetCode contest";
    printf("Test 1: %s\n", reverseWords(s1));
    printf("Expected: s'teL ekat edoCteeL tsetnoc\n\n");

    char s2[] = "Mr Ding";
    printf("Test 2: %s\n", reverseWords(s2));
    printf("Expected: rM gniD\n");

    return 0;
}
