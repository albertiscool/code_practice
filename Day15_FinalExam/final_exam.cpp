#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ==========================================
// 🏆 C 語言韌體工程師 - 終極大會考
// 範圍：Day 1 ~ Day 14 (指標、位元運算、資料結構、演算法)
// ==========================================

// --- [關卡 1：巨集與位元運算 (Day 6, Day 7)] ---
// 韌體工程師最常用巨集來設定暫存器。
// 請寫出一個巨集 `SET_BIT(REG, BIT)`，用來把變數 REG 的第 BIT 位元設為 1。
// 💡 提示：記得要把傳進來的參數用「括號」包好包滿，防止展開錯誤！
#define SET_BIT(REG, BIT) ((REG) |= (1<<(BIT)) )

// --- [關卡 2：環狀佇列極限優化 (Day 11)] ---
// 假設我們有一個大小為 256 (2 的 8 次方) 的環狀佇列，我們想用最快的「位元運算」來取代昂貴的 `% 256` 除法。
// 請問 tail 加 1 之後，要跟哪個數字做位元 AND (&) 運算來達到完美繞圈？
int get_next_tail(int current_tail) {
    return (current_tail + 1) & (255);
}

// --- [關卡 3：防溢位的二元搜尋 (Day 13)] ---
// 在二元搜尋中，為了防止 low + high 產生整數溢位 (Integer Overflow) 導致當機。
// 請填寫安全的 mid 計算公式。
// 💡 提示：請同時結合我們學過的「位元右移 >>」來取代除以 2，達到極限加速！
int get_safe_mid(int low, int high) {
    return low + ((high - low) >> 1); 
}

// --- [關卡 4：指標與記憶體 (Day 3, Day 14)] ---
// 我們要寫一個 swap 函式來交換兩個數字。請問在函式內部，要如何把 b 指標指向的「值」，存進 a 指標指向的「房間」裡？
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b; // 把 b 房間的值搬進 a 房間
    *b = temp;
}

// --- [關卡 5：連結串列的死迴圈偵測 (Day 10)] ---
typedef struct Node {
    int data;
    struct Node *next;
} Node_t;

// 請完成龜兔賽跑演算法 (Floyd's Cycle Detection)
// 為了確保兔子 (fast) 每次跳兩步前「不會踩空墜崖」而引發 Segmentation Fault。
// while 迴圈的兩道防護條件應該怎麼寫？(兩者缺一不可)
bool has_cycle(Node_t *head) {
    Node_t *slow = head;
    Node_t *fast = head;
    
    // 💡 提示：需要防止奇數與偶數長度的越界
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

// ==========================================
// 主程式：執行這裡來批改你的考卷！
// ==========================================
int main() {
    printf("--- 🏆 開始批改 14 天特訓最終測驗 🏆 ---\n\n");
    int score = 0;

    // 測試 1
    int reg = 0b0000;
    SET_BIT(reg, 2);
    if (reg == 0b0100) {
        printf("[✅ 關卡 1 通過] 巨集與位元運算非常扎實！\n");
        score += 20;
    } else {
        printf("[❌ 關卡 1 失敗] SET_BIT 巨集有誤！\n");
    }

    // 測試 2
    if (get_next_tail(255) == 0 && get_next_tail(10) == 11) {
        printf("[✅ 關卡 2 通過] 環狀佇列極限優化正確！\n");
        score += 20;
    } else {
        printf("[❌ 關卡 2 失敗] & 遮罩數字有誤！\n");
    }

    // 測試 3
    if (get_safe_mid(2000000000, 2100000000) == 2050000000) {
        printf("[✅ 關卡 3 通過] 成功避開 Integer Overflow，且完美使用位元加速！\n");
        score += 20;
    } else {
        printf("[❌ 關卡 3 失敗] mid 安全計算公式有誤！\n");
    }

    // 測試 4
    int x = 10, y = 20;
    swap(&x, &y);
    if (x == 20 && y == 10) {
        printf("[✅ 關卡 4 通過] 指標空間與取值觀念清晰！\n");
        score += 20;
    } else {
        printf("[❌ 關卡 4 失敗] swap 指標賦值錯誤！\n");
    }

    // 測試 5
    Node_t n1, n2, n3;
    n1.next = &n2; n2.next = &n3; n3.next = NULL;
    if (has_cycle(&n1) == false) {
        printf("[✅ 關卡 5 通過] 成功擋下 Segmentation Fault 危機！\n");
        score += 20;
    } else {
        printf("[❌ 關卡 5 失敗] while 防護條件有漏洞！\n");
    }

    printf("\n==========================================\n");
    printf("你的最終分數：%d / 100\n", score);
    if (score == 100) {
        printf("🎉 恭喜你！完美通過 14 天的終極特訓！\n");
        printf("你現在具備的觀念，已經完全具備挑戰一線科技大廠韌體/系統工程師面試的資格了！\n");
    } else {
        printf("💪 還有一些小錯誤喔，趕快去檢查一下 ________ 的地方吧！\n");
    }
    printf("==========================================\n");

    return 0;
}
