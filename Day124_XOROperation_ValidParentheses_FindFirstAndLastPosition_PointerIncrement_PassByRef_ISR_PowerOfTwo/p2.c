#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * LeetCode #20 - Valid Parentheses
 * 
 * 題目描述：
 * 給定一個只包含字元 '(', ')', '{', '}', '[' 和 ']' 的字串 s，判斷字串是否有效。
 * 
 * 有效字串需滿足：
 * 1. 左括號必須用相同類型的右括號閉合。
 * 2. 左括號必須以正確的順序閉合。
 * 3. 每個右括號都有一個對應的相同類型左括號。
 * 
 * 範例 1：
 * 輸入：s = "()"
 * 輸出：true
 * 
 * 範例 2：
 * 輸入：s = "()[]{}"
 * 輸出：true
 * 
 * 範例 3：
 * 輸入：s = "(]"
 * 輸出：false
 * 
 * 範例 4：
 * 輸入：s = "([])"
 * 輸出：true
 * 
 * 限制條件：
 * 1 <= s.length <= 10^4
 * s 僅由括號字元 '()[]{}' 組成
 */

/*
 * =========================================================================
 * 【LeetCode 官方提交成績】：
 * - 狀態：Accepted (通過 103/103 測試案例)
 * - 執行時間：0 ms (擊敗 100.00% C 語言提交)
 * - 記憶體：8.4 MB
 * - Submission ID：2179734436
 * -------------------------------------------------------------------------
 * 【使用者原始白板程式碼】：
 * bool isValid(char* s) {
 *     int len = strlen(s);
 *     char stack[len] = {'0'}; // 盲點 1：C99 規範變長陣列 (VLA) 不可使用初始化列表
 *     int curr = -1;
 *     for(int i = 0; i < len; i++)
 *     {
 *         if(s[i] == '(') { curr++; stack[curr] = ')'; }
 *         else if(s[i] == '[') { curr++; stack[curr] = ']'; }
 *         else if(s[i] == '{') { curr++; stack[curr] = '}'; }
 *         else
 *         {
 *             if(s[i] != stack[curr]) // 盲點 2：若開頭為右括號，curr 為 -1，存取 stack[-1] 越界當機
 *             {
 *                 return false;
 *             }
 *             else { curr--; }
 *         }
 *     }
 *     return curr == -1;
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點與面試亮點剖析】：
 * 1. 演算法亮點（極度聰明的匹配設計）：
 *    - 遇到左括號時，不是把左括號壓棧，而是「直接將對應的右括號壓入 Stack」！
 *    - 這樣在遇到右括號時，只需直接比較 s[i] == stack[curr]，代碼極度精簡，面試官非常賞識這種技巧。
 * 2. 編譯錯誤（VLA 初始化禁忌）：
 *    - C99 標準（ISO/IEC 9899 6.7.8）明文規定，變長陣列（Variable Length Array, VLA）
 *      不能使用大括號初始化 `= {'0'}`，會引發編譯錯誤。宣告為 char stack[len]; 即可。
 * 3. 陣列下標負數越界（Stack Underflow）：
 *    - 若輸入以右括號開頭（例如 "]" 或 ")("），此時 Stack 為空（curr == -1），
 *      直接存取 stack[curr] 等同讀取 stack[-1]，會引發 Runtime Error: index -1 out of bounds！
 *    - 加上短路求值邊界保護：if (curr == -1 || s[i] != stack[curr]) return false; 即完美解決！
 * =========================================================================
 */

bool isValid(char* s) {
    int len = strlen(s);
    // 奇數長度字串絕對不可能成對閉合，先行快篩
    if (len % 2 != 0) return false;

    char stack[len];
    int curr = -1;

    for (int i = 0; i < len; i++) {
        if (s[i] == '(') {
            curr++;
            stack[curr] = ')';
        } else if (s[i] == '[') {
            curr++;
            stack[curr] = ']';
        } else if (s[i] == '{') {
            curr++;
            stack[curr] = '}';
        } else {
            // 修復：檢查 curr == -1 防止負數越界
            if (curr == -1 || s[i] != stack[curr]) {
                return false;
            } else {
                curr--;
            }
        }
    }
    return curr == -1;
}

int main() {
    printf("Test 1: %s (Expected: true)\n", isValid("()") ? "true" : "false");
    printf("Test 2: %s (Expected: true)\n", isValid("()[]{}") ? "true" : "false");
    printf("Test 3: %s (Expected: false)\n", isValid("(]") ? "true" : "false");
    printf("Test 4: %s (Expected: true)\n", isValid("([])") ? "true" : "false");
    printf("Test 5: %s (Expected: false)\n", isValid("([)]") ? "true" : "false");

    return 0;
}
