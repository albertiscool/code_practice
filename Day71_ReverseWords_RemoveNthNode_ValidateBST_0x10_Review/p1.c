#include <stdio.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// 題目 1【反轉字串中的單字 (Reverse Words in a String - LeetCode #151 核心版)】
// 難度：🟢 Easy / 🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：兩次原地反轉法 (Two-Pass In-Place Reversal) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

// 輔助函式：原地反轉字串的 [start, end] 區間
void reverse(char* s, int start, int end)
{
    while (start < end)
    {
        char temp = s[start];
        s[start] = s[end];
        s[end] = temp;
        start++;
        end--;
    }
}

void reverseWords(char* s)
{
    // 防呆處理
    if (s == NULL) return;
    
    int len = strlen(s);
    if (len <= 1) return;

    // ------------------------------------------------------------------------
    // 步驟 1：先將「整個字串」從頭到尾整體反轉！(呼叫 1 次 reverse 即可)
    // "the sky is blue" ➔ "eulb si yks eht"
    // ------------------------------------------------------------------------
    reverse(s, 0, len - 1);

    // ------------------------------------------------------------------------
    // 步驟 2：逐一走訪每個單字，將各單字內部再次反轉！
    // "eulb" ➔ "blue", "si" ➔ "is", "yks" ➔ "sky", "eht" ➔ "the"
    // ------------------------------------------------------------------------
    int start = 0;
    for (int end = 0; end <= len; end++)
    {
        // 當遇到空格 ' ' 或是字串結尾 '\0' 時，代表一個單字結束了！
        if (s[end] == ' ' || s[end] == '\0')
        {
            reverse(s, start, end - 1); // 將目前單字區間 [start, end - 1] 原地反轉
            start = end + 1;            // 下一個單字的起始位置
        }
    }
}

int main()
{
    // 測試 1: "the sky is blue" -> "blue is sky the"
    char s1[] = "the sky is blue";
    reverseWords(s1);
    printf("測試 1 反轉結果: \"%s\" (預期: \"blue is sky the\")\n", s1);
    assert(strcmp(s1, "blue is sky the") == 0);

    // 測試 2: "hello world" -> "world hello"
    char s2[] = "hello world";
    reverseWords(s2);
    printf("測試 2 反轉結果: \"%s\" (預期: \"world hello\")\n", s2);
    assert(strcmp(s2, "world hello") == 0);

    // 測試 3: "embedded system" -> "system embedded"
    char s3[] = "embedded system";
    reverseWords(s3);
    printf("測試 3 反轉結果: \"%s\" (預期: \"system embedded\")\n", s3);
    assert(strcmp(s3, "system embedded") == 0);

    printf("\n🎉 p1.c 反轉字串中的單字所有測試案例全數 100%% 通過！\n");
    return 0;
}
