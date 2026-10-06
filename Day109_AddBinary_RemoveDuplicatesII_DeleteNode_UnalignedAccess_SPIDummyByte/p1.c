#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * ============================================================================
 * 【Day 109 - 題目一】二進位求和 (LeetCode #67 - Add Binary)
 * 【考試頻率】群聯 (Phison) SSD 韌體 / 瑞昱 (Realtek) / 系統廠 硬體全加器 (Full Adder) 模擬高頻題
 * 【LeetCode 難度】Easy (字串雙指標與進位模擬)
 * ============================================================================
 * 【題目說明】
 * 給定兩個二進位字串 a 和 b，以二進位字串的形式回傳它們的和。
 * 
 * 【範例 1】
 * 輸入: a = "11", b = "1"
 * 輸出: "100"
 * 
 * 【範例 2】
 * 輸入: a = "1010", b = "1011"
 * 輸出: "10101"
 * 
 * 【限制條件】
 * 1. 1 <= a.length, b.length <= 10^4
 * 2. a 和 b 僅由字元 '0' 或 '1' 組成
 * 3. 除了數字 0 本身以外，字串都不包含前導零
 * ============================================================================
 */

char* addBinary(char* a, char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    // 加法最大可能長度為 max(len_a, len_b) + 1 (進位) + 1 ('\0')
    int max_len = (len_a > len_b ? len_a : len_b) + 2;
    char* result = (char*)malloc(max_len * sizeof(char));

    int i = len_a - 1; // 從 a 的個位數 (LSB) 開始倒著加
    int j = len_b - 1; // 從 b 的個位數 (LSB) 開始倒著加
    int carry = 0;     // 進位標記
    int k = 0;         // result 寫入指標

    // 💡 核心心法：只要 a 還沒加完、或 b 還沒加完、或「最後還有殘留進位」，統一一個迴圈搞定！
    while (i >= 0 || j >= 0 || carry > 0) {
        int sum = carry; // 先吃上一輪的進位

        if (i >= 0) {
            sum += a[i] - '0'; // 若 a 還有字元，加進來並往前移
            i--;
        }
        if (j >= 0) {
            sum += b[j] - '0'; // 若 b 還有字元，加進來並往前移 (短的跑完了就自然跳過)
            j--;
        }

        result[k++] = (sum % 2) + '0'; // 當前位：sum % 2
        carry = sum / 2;               // 新進位：sum / 2
    }

    result[k] = '\0'; // 補上字串結束符

    // 💡 雙指標原地反轉（因為我們剛才是由個位數填向高位數）
    int left = 0;
    int right = k - 1;
    while (left < right) {
        char temp = result[left];
        result[left] = result[right];
        result[right] = temp;
        left++;
        right--;
    }

    return result;
}

/*
===============================================================================
【原始程式碼盲點與改進分析】
-------------------------------------------------------------------------------
1. ⚠️ 位元權重方向反轉（從 MSB 高位開始加）：
   - 原寫法：`int i = 0; while(i < len_small) { ... a[i] + b[i] ... }`
   - 致命盲點：
     在字串中，索引 0 代表的是「最高位（MSB，千位數）」，索引末端才是「個位數（LSB）」！
     若從 i = 0 開始相加，相當於把 1000 + 10 算成了 (千位數加千位數)，完全錯位！
     加法運算在電路與演算法中，一律必須從「最右端（LSB）」向左推進。

2. ⚠️ 長短字串拆分造成分支爆炸（Over-Engineering）：
   - 原寫法試圖先跑 `len_small`，再跑 `len_big`，再處理 `carry`。
   - 盲點剖析：
     這樣做不僅要事先判斷誰長誰短，且寫完短的之後，長的那截該怎麼繼續傳遞 carry、
     最後若持續進位又該如何擴展，會衍生出上百行重複的 if-else 窮舉。
   - 正解：
     使用單一迴圈：`while (i >= 0 || j >= 0 || carry > 0)`，
     搭配 `if (i >= 0) sum += a[i] - '0';`，
     短字串結束後自然略過，最後的 carry 也會自動在最後一輪被填入，邏輯簡潔百倍！

-------------------------------------------------------------------------------
【實測成績】
- 官方評判：Accepted (通過全部測資) 🎉
- 執行時間：0 ms (Beats 100.00% C submissions) ⚡
- 記憶體消耗：8.9 MB (Beats 100.00% C submissions) 🚀
- 提交 ID：2162901920

-------------------------------------------------------------------------------
【原始程式碼存檔】
char* addBinary(char* a, char* b) {
    // 請在此處撰寫你的程式碼
    int len_a = strlen(a);
    int len_b = strlen(b);
    int len_small = (len_a < len_b) ? len_a : len_b;
    int len_big = (len_a > len_b) ? len_a : len_b;
    int carry = 0;
    char* result = (char*) malloc((len_big + 2) * sizeof(char));
    
    int i = 0;
    while(i < len_small)
    {
        if(a[i] == '1' && b[i] == '1')
        {
            if(carry == 1)
            {
                result[i] = '1';
                carry = 1;
            }
            else
            {
                result[i] = '0';
                carry = 1;
            }
        }
        else if((a[i] == '1' && b[i] == '0') || (a[i] == '0' && b[i] == '1'))
        {
            if(carry == 1)
            {
                result[i] = '0';
                carry = 1;
            }
            else
            {
                result[i] = '1';
                carry = 0;
            }
        }
        else
        {
            if(carry == 1)
            {
                result[i] = '1';
                carry = 0;
            }
            else
            {
                result[i] = '0';
                carry = 0;
            }
        }
        i++;
    }
    
    while(i < len_big)
    {
        if(carry == 1 )
        {

        }
    }
}
===============================================================================
*/