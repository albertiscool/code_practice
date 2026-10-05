#include <stdio.h>

// 題目：利用遞迴計算「費氏數列 (Fibonacci Sequence)」
// 費氏數列的規則：
// 第 0 項 = 0
// 第 1 項 = 1
// 第 n 項 = 第 n-1 項 + 第 n-2 項 (前面兩項相加)
// 數列長這樣：0, 1, 1, 2, 3, 5, 8, 13, 21, 34...

int fibonacci(int n) {
    // 1. 終止條件 (Base Case)：老大親自出馬
    // 思考：當 n 到底部（0 或 1）的時候，我們可以直接交出答案，不需要叫小弟了。
    if (n == 0 || n == 1) {
        return n;
    }

    // 2. 信任小弟 (Delegate)：把大問題切小
    // 呼叫一號小弟，去幫我算出「前一項 (n-1)」的答案
    int prev1 = fibonacci(n-1); 
    
    // 呼叫二號小弟，去幫我算出「前兩項 (n-2)」的答案
    int prev2 = fibonacci(n-2); 

    // 3. 收尾工作：老大把小弟的心血結晶組合起來
    // 費氏數列的定義是「前兩項相加」，所以把兩個小弟的答案加起來，交接上去！
    return prev1 + prev2;
}

int main() {
    printf("--- Day 10: 遞迴特訓 (費氏數列) ---\n");
    
    int target = 6; // 我們要算第 6 項 (預期答案會是 8)
    int result = fibonacci(target);
    
    printf("Fibonacci 數列的第 %d 項是: %d\n", target, result);
    
    // 自動防呆驗證
    if (result == 8) {
        printf("🎉 恭喜！你的遞迴邏輯完全正確！\n");
    } else {
        printf("❌ 答案不對喔，再檢查一下填空的地方！\n");
    }

    return 0;
}
