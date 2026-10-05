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

char* reverseWords(char* s)
{
    if (s == NULL) return NULL;
    int len = strlen(s);

    // 步驟 1：原地清除多餘空格 (前導、尾隨、多重連續空格)
    int slow = 0, fast = 0;
    while (fast < len && s[fast] == ' ') fast++;
    while (fast < len) {
        if (s[fast] != ' ') {
            s[slow++] = s[fast++];
        } else {
            s[slow++] = ' ';
            while (fast < len && s[fast] == ' ') fast++;
        }
    }
    if (slow > 0 && s[slow - 1] == ' ') slow--;
    s[slow] = '\0';
    len = slow;
    if (len <= 1) return s;

    // 步驟 2：整體反轉
    reverse(s, 0, len - 1);

    // 步驟 3：單字內部個別反轉
    int start = 0;
    for (int end = 0; end <= len; end++)
    {
        if (s[end] == ' ' || s[end] == '\0')
        {
            reverse(s, start, end - 1);
            start = end + 1;
        }
    }
    return s;
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
