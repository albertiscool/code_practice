#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 2【字串轉換整數 (String to Integer - atoi 核心版)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (系統廠與韌體面試必考手寫題)
// 演算法：三階段線性走訪 (Three-Phase Linear Scan) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

int my_atoi(const char* s)
{
    // 防呆處理
    if (s == NULL)
    {
        return 0;
    }

    int i = 0;
    int sign = 1;
    int result = 0;

    // 階段 1：跳過前導空格
    while (s[i] == ' ')
    {
        i++;
    }

    // 階段 2：判斷正負號
    if (s[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (s[i] == '+')
    {
        i++;
    }

    // 階段 3：連續讀取數字字元，遇非數字或結尾 '\0' 立即停止
    while (s[i] >= '0' && s[i] <= '9')
    {
        // 核心十進位左移累加：原本數字 * 10 + 新進來的個位數
        result = result * 10 + (s[i] - '0');
        i++;
    }

    return sign * result;
}

int main()
{
    // 測試 1: "42" -> 42
    assert(my_atoi("42") == 42);
    printf("測試 1 通過: \"42\" 轉換為 42\n");

    // 測試 2: "   -42" -> -42
    assert(my_atoi("   -42") == -42);
    printf("測試 2 通過: \"   -42\" 轉換為 -42\n");

    // 測試 3: "4193 with words" -> 4193
    assert(my_atoi("4193 with words") == 4193);
    printf("測試 3 通過: \"4193 with words\" 轉換為 4193\n");

    // 測試 4: "words and 987" -> 0
    assert(my_atoi("words and 987") == 0);
    printf("測試 4 通過: \"words and 987\" 轉換為 0\n");

    // 測試 5: "+123" -> 123
    assert(my_atoi("+123") == 123);
    printf("測試 5 通過: \"+123\" 轉換為 123\n");

    // 測試 6: NULL 與空字串 -> 0
    assert(my_atoi("") == 0);
    assert(my_atoi(NULL) == 0);
    printf("測試 6 通過: 空字串與 NULL 轉換為 0\n");

    printf("\n🎉 p2.c 字串轉整數 (my_atoi) 所有測試案例全數 100%% 通過！\n");
    return 0;
}
