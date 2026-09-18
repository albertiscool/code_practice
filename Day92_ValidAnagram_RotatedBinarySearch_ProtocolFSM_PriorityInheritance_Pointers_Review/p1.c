#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// 題目 1【有效的字母異位詞 (Valid Anagram - LeetCode #242)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (科技廠/IC設計廠白板字串高頻基本題)
// 題目說明：給定兩個字串 s 和 t，編寫一個函式來判斷 t 是否是 s 的字母異位詞。
//           若 s 和 t 中每個字元出現的次數皆相同，只是排列順序不同，則為異位詞。
// 範例 1：
//    輸入：s = "anagram", t = "nagaram"
//    輸出：true
// 範例 2：
//    輸入：s = "rat", t = "car"
//    輸出：false
// 限制條件：
//    - 1 <= s.length, t.length <= 5 * 10^4
//    - s 和 t 僅包含小寫英文字母。
//    - 時間複雜度要求：O(N)
//    - 額外空間複雜度要求：O(1)（字元集大小固定為 26）
// ============================================================================

bool isAnagram(char* s, char* t) {
    // TODO: 請在純白板模式下完成實作 (請勿尋求提示)
    if(s == NULL || t == NULL)
    {
        return false;
    }
    if(strlen(s) != strlen(t))
    {
        return false;
    }
    int word[26] = {0};
    int len = strlen(s);
    for(int i = 0; i < len; i++)
    {
        word[s[i] - 'a']++;
        word[t[i] - 'a']--;
    }
    for(int j = 0; j < 26; j++)
    {
        if(word[j] != 0)
        {
            return false;
        }
    }
    return true;
}

int main(void) {
    /* 測試測資 1: "anagram" 與 "nagaram" -> true (1) */
    char s1[] = "anagram";
    char t1[] = "nagaram";
    bool ans1 = isAnagram(s1, t1);
    printf("測資 1 結果: %s (預期: true)\n", ans1 ? "true" : "false");

    /* 測試測資 2: "rat" 與 "car" -> false (0) */
    char s2[] = "rat";
    char t2[] = "car";
    bool ans2 = isAnagram(s2, t2);
    printf("測資 2 結果: %s (預期: false)\n", ans2 ? "true" : "false");

    /* 測試測資 3: 長度不同 "ab" 與 "a" -> false (0) */
    char s3[] = "ab";
    char t3[] = "a";
    bool ans3 = isAnagram(s3, t3);
    printf("測資 3 結果: %s (預期: false)\n", ans3 ? "true" : "false");

    return 0;
}
