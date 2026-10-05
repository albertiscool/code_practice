#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// 標準滿分解答：有效的括號 (Valid Parentheses) - Stack 堆疊模擬法 ($O(N)$ 時間, $O(N)$ 空間)
// 🌟 經典神技：遇到左括號時，直接將「預期的右括號」推進堆疊！
// ============================================================================
bool isValid(char *s)
{
    if (s == NULL) return false;

    int len = strlen(s);
    // 奇數長度絕對不可能成對閉合，直接返回 false (微秒級剪枝優化)
    if (len % 2 != 0) return false;

    char stack[len + 1]; // 利用區域變數在 CPU Stack 上建立陣列 (零 malloc 開銷)
    int top = -1;        // top = -1 代表堆疊為空

    while (*s != '\0')
    {
        // 1. 遇到左括號，將對應的「預期右括號」壓入堆疊
        if (*s == '(')
        {
            stack[++top] = ')';
        }
        else if (*s == '[')
        {
            stack[++top] = ']';
        }
        else if (*s == '{')
        {
            stack[++top] = '}';
        }
        // 2. 遇到右括號：檢查堆疊是否為空 (top < 0) 或 彈出的字元是否不匹配
        else if (top < 0 || stack[top--] != *s)
        {
            return false; // 提早遇到右括號或類型不吻合
        }

        s++;
    }

    // 3. 走訪結束：必須確認堆疊「完全清空」(top == -1)，排除如 "((" 沒閉合的情況
    return top == -1;
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    // 測試 1：標準成對 "()"
    assert(isValid("()") == true);
    printf("測試 1 \"()\" -> 判定: true\n");

    // 測試 2：多種類型成對 "()[]{}"
    assert(isValid("()[]{}") == true);
    printf("測試 2 \"()[]{}\" -> 判定: true\n");

    // 測試 3：類型不匹配 "(]" -> false
    assert(isValid("(]") == false);
    printf("測試 3 \"(]\" -> 判定: false\n");

    // 測試 4：順序交錯交叉 "([)]" -> false
    assert(isValid("([)]") == false);
    printf("測試 4 \"([)]\" -> 判定: false\n");

    // 測試 5：嵌套匹配 "{[]}" -> true
    assert(isValid("{[]}") == true);
    printf("測試 5 \"{[]}\" -> 判定: true\n");

    // 測試 6：未閉合左括號 "(" 或 "(((" -> false
    assert(isValid("(") == false);
    printf("測試 6 \"(\" -> 判定: false\n");

    // 測試 7：單一右括號 ")" 或 "]" -> false
    assert(isValid("]") == false);
    printf("測試 7 \"]\" -> 判定: false\n");

    printf("\n🎉 所有測試案例全部 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（Stack 大架構完全正確！）：
bool isValid_original(char *s)
{
    if(s == NULL) return false;

    char stack[strlen(s) + 1];
    int top = -1;
    
    // 💡 盲點 1：需解引用 *s != '\0'（不能只寫 s != '\0'）
    while(*s != '\0')
    {
        if(*s == '(' || *s == '[' || *s == '{')
        {
            stack[++top] = *s;
        }
        else if(*s == ')')
        {
            // 💡 盲點 2（Underflow 越界）：如果一開始就來 ')'，top 是 -1，stack[-1] 會記憶體越界當機！
            // 必須先判斷 top < 0
            // 💡 盲點 3（漏掉 Pop）：比對成功後必須 top-- 彈出，否則左括號永遠留著！
            if(top < 0 || stack[top--] != '(') return false;
        }
        else if(*s == ']')
        {
            if(top < 0 || stack[top--] != '[') return false;
        }
        else if(*s == '}')
        {
            // 💡 盲點 4：左括號是大括號 '{'，不是 '}'
            if(top < 0 || stack[top--] != '{') return false;
        }
        s++;
    }
    // 💡 盲點 5：結尾必須 return top == -1，因為如果字串是 "("，堆疊沒清空不能算有效！
    return top == -1;
}
*/