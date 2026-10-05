#include <stdio.h>
#include <assert.h>

// ============================================================================
// 標準滿分解答：買賣股票的最佳時機 (Best Time to Buy and Sell Stock)
// 演算法：單次遍歷狀態追蹤 ($O(N)$ 時間, $O(1)$ 空間)
// ============================================================================
int maxProfit(int* prices, int pricesSize)
{
    // 邊界防禦：如果天數小於等於 1 天，無法完成買賣交易
    if (pricesSize <= 1)
    {
        return 0;
    }

    int max_profit = 0;      // 記錄迄今為止的最大利潤 (p)
    int min_price = prices[0]; // 記錄迄今為止的歷史最低進貨價 (m)

    for (int i = 0; i < pricesSize; i++)
    {
        // 1. 如果今天價格比歷史最低價還便宜，更新進貨價
        if (prices[i] < min_price)
        {
            min_price = prices[i];
        }

        // 2. 如果今天賣出的利潤 (今天價格 - 歷史最低價) 大於歷史最高利潤，更新利潤
        if (prices[i] - min_price > max_profit)
        {
            max_profit = prices[i] - min_price;
        }
    }

    return max_profit;
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    // 測試 1：[7, 1, 5, 3, 6, 4] -> 第 2 天買入 (1)，第 5 天賣出 (6)，利潤 = 5
    int prices1[] = {7, 1, 5, 3, 6, 4};
    int ans1 = maxProfit(prices1, 6);
    printf("測試 1 獲利: %d (預期: 5)\n", ans1);
    assert(ans1 == 5);

    // 測試 2：一路下跌 [7, 6, 4, 3, 1] -> 不進行交易，利潤 = 0
    int prices2[] = {7, 6, 4, 3, 1};
    int ans2 = maxProfit(prices2, 5);
    printf("測試 2 獲利: %d (預期: 0)\n", ans2);
    assert(ans2 == 0);

    // 測試 3：單一天數 [5] -> 利潤 = 0
    int prices3[] = {5};
    int ans3 = maxProfit(prices3, 1);
    printf("測試 3 獲利: %d (預期: 0)\n", ans3);
    assert(ans3 == 0);

    printf("\n🎉 所有測試案例全部 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（演算法狀態追蹤邏輯 100% 完全正確！）：
int maxProfit_original(int* prices, int pricesSize)
{
    // 💡 盲點 1：邊界防呆，若 pricesSize == 0，prices[0] 會越界，先判斷 if (pricesSize <= 1) return 0;
    int p = 0, m = prices[0], i = 0;
    
    // 💡 盲點 2（無窮迴圈）：while 迴圈內部漏掉了 i++！
    // 改用 for (int i = 0; i < pricesSize; i++) 就不會忘記推進索引！
    while(i < pricesSize)
    {
        if(prices[i] < m)
        {
            m = prices[i];
        }

        if(prices[i] - m > p)
        {
            p = prices[i] - m;
        }
        i++; // 補上 i++
    }
    return p;
}
*/