#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

// ============================================================================
// 題目 1【驗證迴文字串 (Valid Palindrome - LeetCode #125)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠高頻經典題)
// 演算法：首尾對撞雙指針 (Two Pointers) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

bool isPalindrome(char* s)
{
    // 防呆處理
    if (s == NULL)
    {
        return true;
    }

    int first = 0;
    int last = strlen(s) - 1;

    while (first < last)
    {
        // 1. 左指標向前推進，跳過所有非英數字元
        while (first < last && !isalnum((unsigned char)s[first]))
        {
            first++;
        }

        // 2. 右指標向後倒退，跳過所有非英數字元
        while (first < last && !isalnum((unsigned char)s[last]))
        {
            last--;
        }

        // 3. 核心比較：統一轉小寫比對 (tolower 會回傳小寫數值)
        if (tolower((unsigned char)s[first]) != tolower((unsigned char)s[last]))
        {
            return false;
        }

        // 4. 兩端字元相同，雙指針同步向中間收縮！
        first++;
        last--;
    }

    return true;
}

int main()
{
    // 測試 1: "A man, a plan, a canal: Panama" -> true
    char s1[] = "A man, a plan, a canal: Panama";
    assert(isPalindrome(s1) == true);
    printf("測試 1 通過: \"A man, a plan, a canal: Panama\" 是有效迴文 (true)\n");

    // 測試 2: "race a car" -> false
    char s2[] = "race a car";
    assert(isPalindrome(s2) == false);
    printf("測試 2 通過: \"race a car\" 非迴文 (false)\n");

    // 測試 3: " " -> true
    char s3[] = " ";
    assert(isPalindrome(s3) == true);
    printf("測試 3 通過: 空格純無效字串是迴文 (true)\n");

    // 測試 4: "0P" -> false (數字與字母)
    char s4[] = "0P";
    assert(isPalindrome(s4) == false);
    printf("測試 4 通過: \"0P\" 非迴文 (false)\n");

    printf("\n🎉 p1.c 驗證迴文字串所有測試案例全數 100%% 通過！\n");
    return 0;
}
