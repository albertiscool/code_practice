#include <stdio.h>
#include <string.h>

// ============================================================================
// 寫法 1：100% 順著你的原版思路修正（單一函式直接對撞，無額外 helper）
// ============================================================================
void reverse_words(char *s)
{
    if (s == NULL || *s == '\0') return;

    // 1. 你的第 1 步：找整條字串的尾巴 last
    char *first = s;
    char *last = s;
    
    while (*last != '\0') // 修正：檢查字元是不是 '\0'，不是指標 NULL
    {
        last++;           // 修正：單純 last++ 往後走
    }
    last--;               // 往回退一格，指到最後一個有效字元 (例如 'e')

    // 2. 你的第 2 步：雙指標對撞，翻轉整條字串！
    // 例："the sky is blue" ➡️ "eulb si yks eht"
    while (first < last)  // 修正：終止條件是兩指標碰頭 (first < last)
    {
        char temp = *first;
        *first = *last;
        *last = temp;
        first++;
        last--;
    }

    // 3. 你的第 3 步：由左往右掃描字串，遇到空格就翻轉該單字！
    char *word_start = s;
    char *ptr = s;

    while (*ptr != '\0')
    {
        if (*ptr == ' ') // 遇到空格，代表 [word_start, ptr - 1] 是一個單字！
        {
            // 直接用你寫的雙指標對調邏輯，翻轉這個單字 (例："eulb" ➡️ "blue")：
            char *w_left = word_start;
            char *w_right = ptr - 1;
            while (w_left < w_right)
            {
                char temp = *w_left;
                *w_left = *w_right;
                *w_right = temp;
                w_left++;
                w_right--;
            }
            word_start = ptr + 1; // 下一個單字的開頭
        }
        ptr++;
    }

    // 4. 補上最後一個單字的翻轉（結尾是 '\0' 的單字，例："eht" ➡️ "the"）
    char *w_left = word_start;
    char *w_right = ptr - 1;
    while (w_left < w_right)
    {
        char temp = *w_left;
        *w_left = *w_right;
        *w_right = temp;
        w_left++;
        w_right--;
    }
}

// ============================================================================
// 寫法 2：模組化 Helper 寫法（面試最推薦簡潔版）
// ============================================================================
void reverse_range(char *start, char *end)
{
    while (start < end)
    {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void reverse_words_modular(char *s)
{
    if (s == NULL || *s == '\0') return;

    int len = strlen(s);

    // 階段 1：全字串翻轉
    reverse_range(s, s + len - 1);

    // 階段 2：逐字翻轉
    char *word_start = s;
    char *curr = s;

    while (*curr != '\0')
    {
        if (*curr == ' ')
        {
            reverse_range(word_start, curr - 1);
            word_start = curr + 1;
        }
        curr++;
    }
    reverse_range(word_start, curr - 1);
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    char str1[] = "the sky is blue";
    printf("原字串: \"%s\"\n", str1);
    reverse_words(str1);
    printf("翻轉後: \"%s\"\n\n", str1);

    char str2[] = "hello world";
    printf("原字串: \"%s\"\n", str2);
    reverse_words(str2);
    printf("翻轉後: \"%s\"\n", str2);

    return 0;
}

/*
// ⚠️ 你的原始程式碼比對：
void reverse_words_original(char *s)
{
    char *first = s;
    char *last = s;
    while(last != NULL)      // 盲點 1: 應為 *last != '\0'
    {
        last = last++;       // 盲點 2: 應為 last++
    }
    last = last--;

    while(first != NULL)     // 盲點 3: 應為 first < last
    {
        char temp = *first;
        *first = *last;
        *last = temp;
        first++;
        last--;
    }
    
    first--;

    while(first != NULL)
    {
        if(first == '\0')    // 盲點 4: 應為 *ptr == ' '
        {
            char *a = fisrt;
            reverse_words(a);
        }
        first--;
    }
}
*/