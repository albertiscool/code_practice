#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 3【爬樓梯 (Climbing Stairs - LeetCode #70)】
// 演算法：滾動變數動態規劃 (Rolling Dynamic Programming) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================
int climbStairs(int n)
{
    // 邊界條件：1 階只有 1 種方法，2 階有 2 種方法
    if (n <= 2)
    {
        return n;
    }

    int first = 1;  // 代表 f(n-2)
    int second = 2; // 代表 f(n-1)
    int result = 0;

    // 從第 3 階開始往上滾動計算至第 n 階 ($O(N)$ 時間, $O(1)$ 空間)
    for (int i = 3; i <= n; i++)
    {
        result = first + second; // f(n) = f(n-1) + f(n-2)
        first = second;          // 變數向前滾動
        second = result;
    }

    return result;
}

int main()
{
    // 測試 1: n = 2 -> 2
    assert(climbStairs(2) == 2);
    printf("測試 1 (n=2) -> 方法數: %d (預期: 2)\n", climbStairs(2));

    // 測試 2: n = 3 -> 3
    assert(climbStairs(3) == 3);
    printf("測試 2 (n=3) -> 方法數: %d (預期: 3)\n", climbStairs(3));

    // 測試 3: n = 5 -> 8
    assert(climbStairs(5) == 8);
    printf("測試 3 (n=5) -> 方法數: %d (預期: 8)\n", climbStairs(5));

    // 測試 4: n = 1 -> 1
    assert(climbStairs(1) == 1);
    printf("測試 4 (n=1) -> 方法數: %d (預期: 1)\n", climbStairs(1));

    // 測試 5: 較大數據 n = 45 -> 預期秒出，不會爆棧或超時
    printf("測試 5 (n=45) -> 方法數: %d\n", climbStairs(45));

    printf("\n🎉 p3.c 所有測試案例全數 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（數學公式完全正確，但時間複雜度需優化）：
int climbStairs_recursive(int n)
{
    if(n == 1) return 1;
    else if(n == 2) return 2;

    // 💡 致命問題（時間爆表 $O(2^N)$ 與 Stack 溢位）：
    // 這種純遞迴寫法會重複計算極多子問題！
    // 例如求 climbStairs(45) 會觸發 $2^{45} \approx 35,000,000,000,000$ 次函式呼叫，在 LeetCode 上會超時 (TLE)！
    // 在 MCU 韌體上更會因為 Stack 深度太深導致 Stack Overflow 當機！
    // 正解：改用 for 迴圈 + 2 個滾動變數 (first, second)，做到 $O(N)$ 時間、$O(1)$ 空間！
    return climbStairs_recursive(n-1) + climbStairs_recursive(n-2);
}
*/
