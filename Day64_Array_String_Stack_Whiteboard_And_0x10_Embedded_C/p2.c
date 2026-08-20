#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// 標準滿分解答：驗證迴文串 (Valid Palindrome) - 雙指標對撞法 ($O(N)$ 時間, $O(1)$ 空間)
// ============================================================================
bool isPalindrome(char *s)
{
    if (s == NULL) return true;

    char *first = s;
    char *last = s + strlen(s) - 1; // 快速指向字串最後一個有效字元

    while (first < last)
    {
        // 1. 跳過左邊的無效字元 (必須加上 first < last 防呆，避免全空格字串越界)
        while (first < last && !isalnum(*first))
        {
            first++;
        }

        // 2. 跳過右邊的無效字元 (必須加上 first < last 防呆，且是 last-- 往左退)
        while (first < last && !isalnum(*last))
        {
            last--;
        }

        // 3. 將兩邊字元轉為小寫進行比較 (tolower 會回傳小寫數值)
        if (tolower(*first) != tolower(*last))
        {
            return false; // 不相等，直接判定非迴文
        }

        // 4. 比對成功，雙指標繼續往中間靠攏
        first++;
        last--;
    }

    return true;
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    // 測試 1：標準包含標點符號迴文串
    char s1[] = "A man, a plan, a canal: Panama";
    printf("測試 1: \"%s\" -> 判定: %s\n", s1, isPalindrome(s1) ? "true" : "false");
    assert(isPalindrome(s1) == true);

    // 測試 2：非迴文串
    char s2[] = "race a car";
    printf("測試 2: \"%s\" -> 判定: %s\n", s2, isPalindrome(s2) ? "true" : "false");
    assert(isPalindrome(s2) == false);

    // 測試 3：純空格與標點符號 (極端案例)
    char s3[] = " .,  ";
    printf("測試 3: \"%s\" -> 判定: %s\n", s3, isPalindrome(s3) ? "true" : "false");
    assert(isPalindrome(s3) == true);

    // 測試 4：包含數字的迴文串
    char s4[] = "0P";
    printf("測試 4: \"%s\" -> 判定: %s\n", s4, isPalindrome(s4) ? "true" : "false");
    assert(isPalindrome(s4) == false);

    printf("\n🎉 所有測試案例全部 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（雙指標架構完全正確！）：
bool isPalindrome_original(char *s)
{
    char *first = s;
    char *last = s;

    // 💡 盲點 1：需寫 *last != '\0'，且走到結尾後要 last-- 退一格，否則 last 會指在 '\0' 上！
    while(*last != '\0')
    {
        last++;
    }
    last--; // 必須退一格

    while(first < last) 
    {
        // 💡 盲點 2（越界防呆）：內層 while 必須加上 first < last，否則遇到全空格字串會一路衝出記憶體當機！
        while(first < last && !isalnum(*first))
        {
            first++;
        }

        // 💡 盲點 3：右指標往左退是 last--（不是 last++）！
        while(first < last && !isalnum(*last))
        {
            last--;
        }

        // 💡 盲點 4：tolower() 是「回傳」小寫值，它不會原地修改記憶體！
        // 因此要直接在 if 比對：if (tolower(*first) != tolower(*last)) return false;
        if(tolower(*first) != tolower(*last))
        {
            return false;
        }
        else
        {
            first++;
            last--;
        }
    }
    return true;
}
*/