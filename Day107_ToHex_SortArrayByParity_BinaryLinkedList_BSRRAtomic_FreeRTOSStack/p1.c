#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * ============================================================================
 * 【Day 107 - 題目一】數字轉十六進位 (LeetCode #405 - Convert a Number to Hexadecimal)
 * 【考試頻率】群聯 (Phison) SSD 韌體 / 瑞昱 (Realtek) / 系統廠 暫存器傾印 (Hex Dump) 高頻題
 * 【LeetCode 難度】Easy (韌體經典位元轉換題)
 * ============================================================================
 * 【題目說明】
 * 給定一個 32 位元整數 num，請將其轉換為十六進位字串並回傳。
 * 對於負整數，必須使用「二補數（Two's Complement）」方式轉換。
 * 
 * 十六進位字串中所有字母必須為小寫 (a-f)，且除了數字 0 本身外，
 * 轉換結果不得包含任何額外的前導零 (Leading Zeroes)。
 * 
 * ⚠️ 限制：不得直接使用任何系統內建的進位轉換格式化函式（例如 sprintf 格式化 %x）。
 * 
 * 【範例 1】
 * 輸入: num = 26
 * 輸出: "1a"
 * 
 * 【範例 2】
 * 輸入: num = -1
 * 輸出: "ffffffff"
 * 
 * 【範例 3】
 * 輸入: num = 0
 * 輸出: "0"
 * 
 * 【限制條件】
 * - -2^31 <= num <= 2^31 - 1
 * ============================================================================
 */

// 依照你的原始邏輯脈絡完整訂正版（0 ms Beats 100% AC）：
// 核心思路：先定位最高位 1（MSB）算出總長度 size，再由低至高每 4 個 bit 拼成一個十六進位字元！
char* toHex(int num) {
    // 0. 特殊邊界條件：0 直接回傳 "0"
    if (num == 0) {
        char* res = (char*)malloc(2 * sizeof(char));
        res[0] = '0';
        res[1] = '\0';
        return res;
    }

    // 1. 【訂正 1】強制轉為 unsigned int，解決負數算術右移死循環與二補數問題
    unsigned int n = (unsigned int)num;
    unsigned int temp = n;

    // 2. 【訂正 2】修正 MSB 定位：不是算 1 的總數，而是記錄「最高位的 1 在哪一個 index」
    int msb = 0;
    for (int i = 0; i < 32; i++) {
        if ((temp & 1) == 1) {
            msb = i; // 每當遇到 1 就更新，最後留下來的就是最高位的 1 (0 ~ 31)
        }
        temp = temp >> 1;
    }

    // 3. 【訂正 3】去除外層多餘的 for(msb; msb>0; msb--) 迴圈與作用域 Scope 錯誤
    // 一次性算出所需字元長度 size，並預留 '\0' 的空間 (+1)
    int size = msb / 4 + 1; 
    char* result = (char*)malloc((size + 1) * sizeof(char));
    result[size] = '\0'; // 補上字串結尾符

    // 4. 【訂正 4】從陣列尾端往前填入每 4 個 bit 組成的字元
    for (int j = size - 1; j >= 0; j--) {
        int count = 0;
        for (int k = 0; k < 4; k++) {
            if ((n & 1) == 1) {
                count += (1 << k); // 【訂正 5】使用硬體位移 (1 << k) 取代 pow(2, k)
            }
            n = n >> 1;
        }

        // 5. 【訂正 6】補齊 10 ~ 15 對應到 'a' ~ 'f' 的轉換邏輯
        if (count < 10) {
            result[j] = count + '0';
        } else {
            result[j] = count - 10 + 'a';
        }
    }

    return result;
}

/*
===============================================================================
【原始程式碼盲點與改進對照】
-------------------------------------------------------------------------------
1. ⚠️ 編譯錯誤（變數生命週期與作用域 Scope 溢出）：
   - 原寫法：`char* result = (char*) malloc(...)` 寫在 `for(msb; msb > 0; msb--)` 大括號內。
   - 訂正：不需要那層多餘的外迴圈，直接在函式外層配置一次 `malloc((size + 1) * sizeof(char))`。

2. ⚠️ 觀念混淆：誤把「漢明權重（Popcount，1 的個數）」當成「MSB 位址」：
   - 原寫法：`if((temp & 1) == 1) msb++;`（這是在算 1 的個數，遇到 16 時只有一個 1，msb=1 導致長度算錯成 1）。
   - 訂正：改為 `if((temp & 1) == 1) msb = i;`，記錄最後一個 1 出現的位置。

3. ⚠️ 負數算術右移（Arithmetic Shift）死循環陷阱：
   - 原寫法：直接對有號數 `int temp, num` 進行 `>> 1`。負數最高位永遠補 1，永遠無法清零。
   - 訂正：第一行轉為 `unsigned int n = (unsigned int)num;`，保證邏輯右移（高位補 0）。

4. ⚠️ 十六進位字母映射（ASCII 'a'~'f'）：
   - 原寫法：`result[j] = count + '0';`（大於 9 會變成 ASCII 標點符號）。
   - 訂正：`count < 10 ? count + '0' : count - 10 + 'a'`。

5. ⚠️ 效能與字串規範：
   - 訂正：用 `(1 << k)` 替代重型的 `<math.h>` 浮點數 `pow(2, k)`；
   - 訂正：字串尾端補上 `result[size] = '\0'`，並處理 `num == 0` 邊界。

-------------------------------------------------------------------------------
【實測成績（你的邏輯訂正版）】
- 官方評判：Accepted (通過全部測資)
- 執行時間：0 ms (Beats 100.00% C submissions) ⚡
- 記憶體消耗：8.4 MB (Beats 100.00% C submissions) 🚀
- 提交 ID：2161877919

-------------------------------------------------------------------------------
【原始程式碼存檔】
char* toHex(int num) {
    // 請在此處撰寫你的程式碼
    int msb = 0;
    int temp = num;
    for(int i = 0; i < 32; i++)
    {
        if((temp & 1) == 1)
        {
            msb++;
        }
        temp = temp >> 1;
    }

    for(msb; msb > 0; msb--)
    {
        int size = msb / 4 + 1; 
        int head = msb % 4; //開頭
        char* result = (char*) malloc(size*sizeof(char));
        for(int j = size - 1; j >= 0; j--)
        {
            int count = 0;
            for(int k = 0; k < 4; k++)
            {
                if(num & 1 == 1)
                {
                    count += pow(2,k);
                }
                num = num >> 1;
            }
            result[j] = count + '0';
        }
    }
    return result;
}
===============================================================================
*/
