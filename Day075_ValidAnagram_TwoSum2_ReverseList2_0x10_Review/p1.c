#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// 題目 1【有效的字母異位詞 (Valid Anagram - LeetCode #242)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：頻率統計陣列法 (Frequency Array) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

bool isAnagram(char* s, char* t)
{
    // 防呆處理
    if (s == NULL || t == NULL)
    {
        return false;
    }

    int len_s = strlen(s);
    int len_t = strlen(t);

    // 1. 若長度不同，必不為字母異位詞
    if (len_s != len_t)
    {
        return false;
    }

    // 2. 頻率統計陣列 (26 個英文字母)
    int count[26] = {0};

    // 3. 單次走訪：s 加頻率，t 減頻率
    for (int i = 0; i < len_s; i++)
    {
        count[s[i] - 'a']++;
        count[t[i] - 'a']--;
    }

    // 4. 檢查是否所有字母出現頻率均剛好抵消為 0
    for (int j = 0; j < 26; j++)
    {
        if (count[j] != 0)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    // 測試 1: "anagram" 與 "nagaram" -> true
    char s1[] = "anagram";
    char t1[] = "nagaram";
    assert(isAnagram(s1, t1) == true);
    printf("測試 1 通過: \"anagram\" 與 \"nagaram\" 是異位詞 (true)\n");

    // 測試 2: "rat" 與 "car" -> false
    char s2[] = "rat";
    char t2[] = "car";
    assert(isAnagram(s2, t2) == false);
    printf("測試 2 通過: \"rat\" 與 \"car\" 不是異位詞 (false)\n");

    // 測試 3: "a" 與 "ab" -> false (長度不同)
    char s3[] = "a";
    char t3[] = "ab";
    assert(isAnagram(s3, t3) == false);
    printf("測試 3 通過: \"a\" 與 \"ab\" 不是異位詞 (false)\n");

    printf("\n🎉 p1.c 有效的字母異位詞所有測試案例全數 100%% 通過！\n");
    return 0;
}
