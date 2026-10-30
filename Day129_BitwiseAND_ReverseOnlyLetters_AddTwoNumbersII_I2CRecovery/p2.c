#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * LeetCode #917 - Reverse Only Letters
 *
 * Description:
 * Given a string s, reverse the string according to the following rules:
 * - All the characters that are not English letters remain in the same position.
 * - All the English letters (lowercase or uppercase) should be reversed.
 *
 * Return s after reversing it.
 *
 * Constraints:
 * 1 <= s.length <= 100
 * s consists of characters with ASCII values in the range [33, 122].
 * s does not contain '\"' or '\\'.
 */

char* reverseOnlyLetters(char* s) {
    int left = 0;
    int right = strlen(s) - 1;

    while (left < right) {
        // 若 left 不是英文字母，向右推進
        if (!isalpha((unsigned char)s[left])) {
            left++;
        }
        // 若 right 不是英文字母，向左推進
        else if (!isalpha((unsigned char)s[right])) {
            right--;
        }
        // 兩端皆為英文字母，進行對稱原地交換
        else {
            char temp = s[left];
            s[left] = s[right];
            s[right] = temp;
            left++;
            right--;
        }
    }
    return s;
}

/*
 * =========================================================================
 * 原始白板實作與盲點深度剖析 (Preserved User's Original Code & Blind Spots):
 * =========================================================================
 * 原始程式碼：
 * char* reverseOnlyLetters(char* s) {
 *     int left = 0;
 *     int right = 0;
 *     int len = strlen(s);
 *     while(right < len)
 *     {
 *         while(!((s[left] - 'a' >= 0 && s[left] - 'a' <= 26) || (s[left] - 'A' >= 0 && s[left] - 'A' <= 26)))
 *         {
 *             left++;
 *         }
 *         right = left;
 *         while(((s[right] - 'a' >= 0 && s[right] - 'a' <= 26) || (s[right] - 'A' >= 0 && s[right] - 'A' <= 26)))
 *         {
 *             right++;
 *         }
 *         int next = right;
 *         right--;
 *         while(left < right)
 *         {
 *             char temp = s[left];
 *             s[left] = s[right];
 *             s[right] = temp;
 *             left++;
 *             right--;
 *         }
 *         left = next;
 *         right = next;
 *     }
 *     return s;
 * }
 *
 * 盲點深度剖析：
 * 1. 題意理解偏差（局部單字翻轉 vs 全域字母首尾對稱翻轉）：
 *    原始實作是尋找「各個連續字母子區間」並在該子區間內原地翻轉（類似 LeetCode #557 Reverse Words III）。
 *    例如 "ab-cd"，原始實作將 "ab" 翻轉為 "ba"，"cd" 翻轉為 "dc"，輸出 "ba-dc"。
 *    但本題要求是「全字串所有字母進行全域反轉，非字母符號位置固定」，
 *    因此 "ab-cd" 的第一個字母 'a' 應該與最後一個字母 'd' 交換，
 *    第二個字母 'b' 與倒數第二個字母 'c' 交換，正確輸出應為 "dc-ba"。
 *
 * 2. 雙指標起始點設計：
 *    全域對稱反轉應將 left 設為 0，right 設為 strlen(s) - 1（首尾雙指標向中間靠攏）。
 *    若遇到非字母則跳過，雙方都是字母時才 swap。
 *
 * 3. 字元範圍判定與標準函式庫：
 *    英文字母只有 26 個，'z' - 'a' 為 25。原始寫法 <= 26 會多算一個 ASCII 字元 '{'。
 *    在 C 語言中可直接使用標準庫 <ctype.h> 的 isalpha()，或使用：
 *    ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))
 * =========================================================================
 */

int main(void) {
    // Test Case 1: "ab-cd" -> "dc-ba"
    char s1[] = "ab-cd";
    printf("Test 1: %s (Expected: dc-ba)\n", reverseOnlyLetters(s1));

    // Test Case 2: "a-bC-dEf-ghIj" -> "j-Ih-gfE-dCba"
    char s2[] = "a-bC-dEf-ghIj";
    printf("Test 2: %s (Expected: j-Ih-gfE-dCba)\n", reverseOnlyLetters(s2));

    // Test Case 3: "Test1ng-Leet=code-Q!" -> "Qedo1ct-eeLg=ntse-T!"
    char s3[] = "Test1ng-Leet=code-Q!";
    printf("Test 3: %s (Expected: Qedo1ct-eeLg=ntse-T!)\n", reverseOnlyLetters(s3));

    return 0;
}
