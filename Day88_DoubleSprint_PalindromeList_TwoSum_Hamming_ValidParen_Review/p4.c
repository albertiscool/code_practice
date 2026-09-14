#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// 題目 4【有效的括號 (Valid Parentheses - LeetCode #20)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠 Stack 堆疊結構白板首選題)
// 題目說明：給定一個只包括 '('，')'，'{'，'}'，'['，']' 的字串 s，判斷字串是否有效。
//           有效字串需滿足：
//           1. 左括號必須用相同類型的右括號閉合。
//           2. 左括號必須以正確的順序閉合。
//           3. 每個右括號都有一個對應的相同類型的左括號。
// 範例 1：
//    輸入：s = "()"
//    輸出：true
// 範例 2：
//    輸入：s = "()[]{}"
//    輸出：true
// 範例 3：
//    輸入：s = "(]"
//    輸出：false
// 範例 4：
//    輸入：s = "([])"
//    輸出：true
// 限制條件：
//    - 1 <= s.length <= 10^4
//    - s 僅由括號 '()[]{}' 組成
// ============================================================================

bool isValid(char* s) {
    int len = strlen(s);
    // 奇數長度必不可能是成對合法括號
    if (len % 2 != 0) {
        return false;
    }

    // 建立字元堆疊 (大小給予 len + 1 防溢位)
    char stack[10005];
    int p = 0;

    for (int i = 0; i < len; i++) {
        if (s[i] == '(' || s[i] == '[' || s[i] == '{') {
            stack[p++] = s[i]; // 左括號入棧 (Push)
        } else if (s[i] == ')') {
            if (p <= 0 || stack[p - 1] != '(') return false;
            p--; // 右括號匹配成功，出棧 (Pop)
        } else if (s[i] == ']') {
            if (p <= 0 || stack[p - 1] != '[') return false;
            p--;
        } else if (s[i] == '}') {
            if (p <= 0 || stack[p - 1] != '{') return false;
            p--;
        }
    }

    // 核心關鍵：走訪結束後，堆疊必須為空 (所有左括號都必須被成功匹配閉合)！
    return p == 0;
}

/*
 * ============================================================================
 * 【原始錯誤實作紀錄與盲點分析】
 * 錯誤原因：
 * 1. 致命結尾返回漏洞 (未檢查 Stack 是否清空)：
 *    在迴圈結束後直接寫了 `return true;`！
 *    如果輸入是未閉合的偶數長度字串 (例如 "((" 或 "((()))((" )，
 *    迴圈跑完時 Stack 裡還堆滿了左括號 (p > 0)，程式卻依然誤判回傳 true！
 *    正確寫法必須檢查：`return p == 0;`。
 * 2. 出棧運算子順序問題：
 *    寫了 `stack[p--] = 0;`。
 *    因為後置遞減 (Post-decrement)，實際上清空的是 `stack[p]` 而非頂端的 `stack[p - 1]`。
 *    在 C 語言模擬 Stack 中，出棧只需要簡單的 `p--;` 即可，不需做多餘賦值。
 * ============================================================================
bool isValid_wrong(char* s) {
    int len = strlen(s);
    if(len % 2 != 0)
    {
        return false;
    }
    int stack[10000] = {0};
    int p = 0;
    for(int i = 0; i < len ; i++)
    {
        if(s[i] == '(' || s[i] == '[' || s[i] == '{')
        {
            stack[p++] = s[i];
        }
        else if(s[i] == ')')
        {
            if(p<=0 || stack[p-1] != '(' )
            {
                return false;
            }
            else
            {
                stack[p--] = 0; // ⚠️ 清空了錯誤索引 stack[p]，其實只要 p-- 即可
            }
        }
        else if(s[i] == ']')
        {
            if(p<=0 || stack[p-1] != '[')
            {
                return false;
            }
            else
            {
                stack[p--] = 0;
            }
        }
        else
        {
            if(p<=0 || stack[p-1] != '{')
            {
                return false;
            }
            else
            {
                stack[p--] = 0;
            }
        }
    }
    return true; // ❌ 盲點：若殘留左括號 (如 "((")，會誤判為 true！應為 return p == 0;
}
*/

int main(void) {
    /* 測試測資 1 */
    char s1[] = "()";
    printf("測資 1 結果: %s (預期: true)\n", isValid(s1) ? "true" : "false");

    /* 測試測資 2 */
    char s2[] = "()[]{}";
    printf("測資 2 結果: %s (預期: true)\n", isValid(s2) ? "true" : "false");

    /* 測試測資 3 */
    char s3[] = "(]";
    printf("測資 3 結果: %s (預期: false)\n", isValid(s3) ? "true" : "false");

    /* 測試測資 4 */
    char s4[] = "([])";
    printf("測資 4 結果: %s (預期: true)\n", isValid(s4) ? "true" : "false");

    /* 測試測資 5 (邊界單括號) */
    char s5[] = "[";
    printf("測資 5 結果: %s (預期: false)\n", isValid(s5) ? "true" : "false");

    return 0;
}


#define exchange(uint32_t n) (((n) & (0x000000ff)) << 24 || ((n) & (0x0000ff00)) << 8 || ((n) & (0x00ff0000)) >> 8 || ((n) & (0xff000000)) >> 24)

