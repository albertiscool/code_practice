#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 題目 2【買賣股票的最佳時機 (Best Time to Buy and Sell Stock - LeetCode #121)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥
// 題目說明：給定一個陣列 prices，其中 prices[i] 表示某支股票在第 i 天的價格。
//           你只能選擇「某一天買入」這支股票，並選擇在「未來的某一個不同的日子賣出」該股票。
//           設計一個演算法來計算你所能獲取的「最大利潤」。
//           如果你不能獲取任何利潤，返回 0。
// 範例 1：
//    輸入：prices = [7,1,5,3,6,4]
//    輸出：5
//    解釋：在第 2 天（價格 = 1）買入，在第 5 天（價格 = 6）賣出，最大利潤 = 6 - 1 = 5。
//          注意利潤不能是 7 - 1 = 6，因為你必須先買入才能賣出。
// 範例 2：
//    輸入：prices = [7,6,4,3,1]
//    輸出：0
//    解釋：在此情況下，沒有交易完成，所以最大利潤為 0。
// 限制條件：
//    - 1 <= prices.length <= 10^5
//    - 0 <= prices[i] <= 10^4
// ============================================================================
int maxProfit(int* prices, int pricesSize) {
    if (prices == NULL || pricesSize <= 1) {
        return 0;
    }
    
    int max = 0;
    int slow = 0; // 買入日指標
    int fast = 1; // 賣出日指標

    while (fast < pricesSize) {
        if (prices[fast] < prices[slow]) {
            // 今天找到了更低的進場成本！直接把買入日換到今天 (大幅降低成本)
            slow = fast;
        } else {
            // 今天賣出有獲利，檢查是否打破歷史最大利潤
            int profit = prices[fast] - prices[slow];
            if (profit > max) {
                max = profit;
            }
        }
        fast++; // 賣出日持續向前探索
    }
    return max;
}

/*
 * ============================================================================
 * 【原始錯誤實作紀錄與盲點分析】
 * 錯誤原因：
 * 在 else 區塊中執行了 `slow++`。
 * 當價格稍微回檔但沒有打破歷史最大利潤時 (例如 prices[fast] - prices[slow] <= max)，
 * 程式碼會把原本在歷史最低點的買入日 (slow) 強制往後推移 (slow++)。
 * 導致後續遇到更高暴漲價格時，買入基準點已經跑掉，算出來的獲利縮水 (例如測資 1 輸出 4 而非 5)。
 *
 * 正確思維：
 * slow (買入日) 只有在發現「比原本買入價更便宜」時才需要更新，
 * 且應直接跳轉：`slow = fast;`。
 * ============================================================================
int maxProfit_wrong(int* prices, int pricesSize) {
    if(prices == NULL)
    {
        return 0;
    }
    
    int max = 0;
    int slow = 0;
    int fast = 1;
    while(fast < pricesSize)
    {
        if(prices[fast] - prices[slow] > max)
        {
            max = prices[fast] - prices[slow];
            fast++;
        }
        else
        {
            slow++; // ❌ 盲點：獲利沒創新高，不代表買入點要往前推！
            fast++;
        }
    }
    return max;
}
*/

int main(void) {
    /* 測試測資 1 */
    int prices1[] = {7, 1, 5, 3, 6, 4};
    int n1 = sizeof(prices1) / sizeof(prices1[0]);
    int ans1 = maxProfit(prices1, n1);
    printf("測資 1 結果: %d (預期: 5)\n", ans1);

    /* 測試測資 2 */
    int prices2[] = {7, 6, 4, 3, 1};
    int n2 = sizeof(prices2) / sizeof(prices2[0]);
    int ans2 = maxProfit(prices2, n2);
    printf("測資 2 結果: %d (預期: 0)\n", ans2);

    /* 測試測資 3 (邊界條件) */
    int prices3[] = {2, 4, 1};
    int n3 = sizeof(prices3) / sizeof(prices3[0]);
    int ans3 = maxProfit(prices3, n3);
    printf("測資 3 結果: %d (預期: 2)\n", ans3);

    return 0;
}
