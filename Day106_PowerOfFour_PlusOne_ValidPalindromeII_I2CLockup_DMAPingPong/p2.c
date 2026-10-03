#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * ============================================================================
 * 【Day 106 - 題目二】加一 (LeetCode #66 - Plus One)
 * 【考試頻率】電子五哥 (廣達、緯創、和碩、仁寶) 陣列進位與記憶體配置題
 * 【LeetCode 難度】Easy (初階精準題)
 * ============================================================================
 * 【題目說明】
 * 給定一個由整數組成的非空陣列 digits，表示一個非負整數。
 * 大整數的最高位排在陣列的開頭，每個元素只包含一個數字 0-9。
 * 請將該整數加一，並以陣列形式回傳結果。
 * 
 * ⚠️ 限制與注意事項：
 * 1. 除了整數 0 本身之外，這個整數不會以零開頭。
 * 2. 在 C 語言中，若發生連續進位（例如 [9, 9, 9] 加一變成 [1, 0, 0, 0]），
 *    需要動態 malloc 一個長度為 digitsSize + 1 的新陣列回傳。
 * 
 * 【範例 1】
 * 輸入: digits = [1,2,3]
 * 輸出: [1,2,4]
 * 解釋: 輸入整數為 123，加一後為 124。
 * 
 * 【範例 2】
 * 輸入: digits = [4,3,2,1]
 * 輸出: [4,3,2,2]
 * 
 * 【範例 3】
 * 輸入: digits = [9]
 * 輸出: [1,0]
 * 
 * 【限制條件】
 * 1. 1 <= digits.length <= 100
 * 2. 0 <= digits[i] <= 9
 * ============================================================================
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* plusOne(int* digits, int digitsSize, int* returnSize) {
    // 從最後一位 (最低位) 開始往前檢查進位
    for (int i = digitsSize - 1; i >= 0; i--) {
        if (digits[i] < 9) {
            digits[i]++;
            // ⚠️ 關鍵：必須回填 *returnSize，告訴呼叫端回傳陣列的大小！
            *returnSize = digitsSize;
            int* result = (int*)malloc(digitsSize * sizeof(int));
            memcpy(result, digits, digitsSize * sizeof(int));
            return result;
        }
        digits[i] = 0; // 當前位是 9，加一後變 0，繼續向前進位
    }

    // 若迴圈結束仍未 return，代表全部都是 9 (例如 999 + 1 = 1000)
    // 陣列長度增加 1
    *returnSize = digitsSize + 1;
    // 使用 calloc 直接將整塊記憶體初始化為 0
    int* result = (int*)calloc(digitsSize + 1, sizeof(int));
    result[0] = 1; // 只有最高位為 1，其餘全為 0
    return result;
}

/*
===============================================================================
【原始程式碼盲點與改進分析】
-------------------------------------------------------------------------------
1. ⚠️ 漏填 *returnSize（C 語言介面致命失誤！）：
   - 題目函式簽章為：`int* plusOne(int* digits, int digitsSize, int* returnSize)`
   - 呼叫端（如 LeetCode 評判系統或主程式）傳入指標 `int* returnSize`，
     目的是「由你來告訴它回傳的陣列長度是多少」！
   - 原寫法在兩個 return 分支中，完全沒有寫入 `*returnSize`：
     - 一般情況漏寫：`*returnSize = digitsSize;`
     - 全為 9 情況漏寫：`*returnSize = digitsSize + 1;`
   - 後果：呼叫端拿不到正確的長度（讀到未初始化的隨機記憶體垃圾值），直接判定錯誤或當機。

2. 邏輯構思相當精巧：
   - 原程式碼中利用 `digits[curr] = 0` 一路改為 0，
     在全為 9 時使用 `memcpy(result + 1, digits, digitsSize * sizeof(int))`
     並將 `result[0] = 1`，這段思考非常敏銳且正確！
   - 唯一需要補上的就是 `<string.h>` 標頭檔與 `*returnSize` 的設定。

3. 韌體極簡寫法優化：
   - 全為 9 的情況，可以使用 `calloc(digitsSize + 1, sizeof(int))` 代替 `malloc + memcpy`。
   - `calloc` 會自動將整塊記憶體填滿 0，接著只需寫一行 `result[0] = 1;` 即可優雅收工！

-------------------------------------------------------------------------------
【實測成績】
- 官方評判：Accepted (通過全部測資)
- 執行時間：0 ms (Beats 100.00% C submissions) ⚡
- 記憶體消耗：10.0 MB (Beats 100.00% C submissions) 🚀

-------------------------------------------------------------------------------
【原始程式碼存檔】
int* plusOne(int* digits, int digitsSize, int* returnSize) {
    // 請在此處撰寫你的程式碼
    int curr = digitsSize - 1;
    while(digits[curr] == 9)
    {
        digits[curr] = 0;
        if(curr == 0)
        {
            int* result = (int*) malloc((digitsSize + 1) * sizeof(int));
            memcpy(result + 1, digits, digitsSize*sizeof(int));
            result[0] = 1;
            return result;
        }
        else
        {
            curr--;
        }
    }
    digits[curr] += 1;
    int* result = (int*) malloc((digitsSize) * sizeof(int));
    memcpy(result, digits, digitsSize*sizeof(int));
    return result;
}
===============================================================================
*/
