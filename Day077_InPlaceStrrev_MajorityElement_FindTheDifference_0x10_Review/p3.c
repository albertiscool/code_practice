#include <stdio.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// 題目 3【找出字串的不同字元 (Find the Difference - LeetCode #389)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (IC 廠經典異或消除題)
// 演算法：全域 XOR 異或消去法 ($a \oplus a = 0$) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

char findTheDifference(char* s, char* t)
{
    // 防呆處理
    if (t == NULL)
    {
        return '\0';
    }

    char result = 0;

    // 1. 異或字串 s 的所有字元
    if (s != NULL)
    {
        for (int i = 0; s[i] != '\0'; i++)
        {
            result ^= s[i];
        }
    }

    // 2. 異或字串 t 的所有字元
    for (int j = 0; t[j] != '\0'; j++)
    {
        result ^= t[j];
    }

    // 3. 所有成對字元全部兩兩抵消為 0，最後留下的就是多出來的字元！
    return result;
}

int main()
{
    // 測試 1: s = "abcd", t = "abcde" -> 'e'
    char s1[] = "abcd";
    char t1[] = "abcde";
    char ans1 = findTheDifference(s1, t1);
    printf("測試 1 通過: 被添加的字元是: '%c' (預期: 'e')\n", ans1);
    assert(ans1 == 'e');

    // 測試 2: s = "", t = "y" -> 'y'
    char s2[] = "";
    char t2[] = "y";
    char ans2 = findTheDifference(s2, t2);
    printf("測試 2 通過: 被添加的字元是: '%c' (預期: 'y')\n", ans2);
    assert(ans2 == 'y');

    // 測試 3: s = "a", t = "aa" -> 'a'
    char s3[] = "a";
    char t3[] = "aa";
    char ans3 = findTheDifference(s3, t3);
    printf("測試 3 通過: 被添加的字元是: '%c' (預期: 'a')\n", ans3);
    assert(ans3 == 'a');

    printf("\n🎉 p3.c 找出不同字元 (XOR 異或消去法) 所有測試案例全數 100%% 通過！\n");
    return 0;
}
