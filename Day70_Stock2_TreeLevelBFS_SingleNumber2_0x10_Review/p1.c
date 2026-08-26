#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 1【買賣股票的最佳時機 II (Best Time to Buy and Sell Stock II - LeetCode #122)】
// 難度：🟡 Medium (中等題) | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：貪心演算法 (Greedy Algorithm) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================
int maxProfit(int* prices, int pricesSize)
{
    // 防呆處理
    if (prices == NULL || pricesSize <= 1)
    {
        return 0;
    }

    int max_profit = 0;

    // 貪心策略：只要明天價格大於今天，就執行一次交易 (買入今天，賣出明天)
    // 所有正數利潤累加即為全域最高總利潤！
    for (int i = 0; i < pricesSize - 1; i++)
    {
        if (prices[i + 1] > prices[i])
        {
            max_profit += (prices[i + 1] - prices[i]);
        }
    }

    return max_profit;
}

int main()
{
    // 測試 1: [7, 1, 5, 3, 6, 4] -> 7
    int p1[6] = {7, 1, 5, 3, 6, 4};
    int ans1 = maxProfit(p1, 6);
    printf("測試 1 最大利潤: %d (預期: 7)\n", ans1);
    assert(ans1 == 7);

    // 測試 2: [1, 2, 3, 4, 5] -> 4
    int p2[5] = {1, 2, 3, 4, 5};
    int ans2 = maxProfit(p2, 5);
    printf("測試 2 最大利潤: %d (預期: 4)\n", ans2);
    assert(ans2 == 4);

    // 測試 3: [7, 6, 4, 3, 1] -> 0
    int p3[5] = {7, 6, 4, 3, 1};
    int ans3 = maxProfit(p3, 5);
    printf("測試 3 最大利潤: %d (預期: 0)\n", ans3);
    assert(ans3 == 0);

    printf("\n🎉 p1.c 買賣股票 II 所有測試案例全數 100%% 通過！\n");
    return 0;
}
