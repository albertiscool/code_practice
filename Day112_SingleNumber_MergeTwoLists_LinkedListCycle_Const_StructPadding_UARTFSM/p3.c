#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/**
 * LeetCode #141: Linked List Cycle
 * 
 * 題目描述：
 * 給定一個鏈結串列的頭節點 head，請判斷鏈結串列中是否存在「環（Cycle）」。
 * 如果鏈結串列中有某個節點可以透過連續跟隨 next 指標再次到達，則鏈結串列中存在環。
 * 若存在環，回傳 true；否則回傳 false。
 * 
 * 限制條件：
 * - 串列中節點數目在範圍 [0, 10^4] 內
 * - -10^5 <= Node.val <= 10^5
 * - 進階挑戰：你能使用 O(1) 常數額外記憶體解決此問題嗎？
 * 
 * 範例 1:
 *   輸入: head = [3,2,0,-4], 尾節點 -4 接回第 1 個節點 2
 *   輸出: true
 * 
 * 範例 2:
 *   輸入: head = [1,2], 尾節點 2 接回第 0 個節點 1
 *   輸出: true
 * 
 * 範例 3:
 *   輸入: head = [1]
 *   輸出: false
 * 
 * 面試深意：
 * 系統廠與 IC 設計廠研替白板必考指針追蹤題，考驗在 O(1) 空間下偵測環形結構。
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

bool hasCycle(struct ListNode *head) {
    // 請在此處實現純白板程式碼
    if(head == NULL || head->next == NULL)
    {
        return false;
    }

    struct ListNode* slow = head;
    struct ListNode* fast = head;

    while(fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast)
        {
            return true;
        }
    }

    return false;
}

// 輔助函式：建立節點
struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

int main(void) {
    printf("=== Test P3: Linked List Cycle ===\n");
    // Test 1: [3, 2, 0, -4] with cycle at pos 1 (node 2)
    struct ListNode* a1 = createNode(3);
    struct ListNode* a2 = createNode(2);
    struct ListNode* a3 = createNode(0);
    struct ListNode* a4 = createNode(-4);
    a1->next = a2; a2->next = a3; a3->next = a4;
    a4->next = a2; // make cycle to node 2
    assert(hasCycle(a1) == true);
    printf("Test 1 Passed: Cycle detected\n");

    // Test 2: [1, 2] with cycle at pos 0
    struct ListNode* b1 = createNode(1);
    struct ListNode* b2 = createNode(2);
    b1->next = b2;
    b2->next = b1; // make cycle to node 1
    assert(hasCycle(b1) == true);
    printf("Test 2 Passed: 2-node cycle detected\n");

    // Test 3: [1] no cycle
    struct ListNode* c1 = createNode(1);
    assert(hasCycle(c1) == false);
    printf("Test 3 Passed: Single node without cycle\n");

    // Test 4: empty list
    assert(hasCycle(NULL) == false);
    printf("Test 4 Passed: Empty list NULL handled\n");

    // Test 5: linear list [1, 2, 3] no cycle
    struct ListNode* d1 = createNode(1);
    struct ListNode* d2 = createNode(2);
    struct ListNode* d3 = createNode(3);
    d1->next = d2; d2->next = d3;
    assert(hasCycle(d1) == false);
    printf("Test 5 Passed: Linear list without cycle\n");

    printf("All tests passed!\n");
    return 0;
}

/* =========================================================================
 * 🚩 【Albert 白板實戰複盤與快慢指標環形偵測深度解析】
 * =========================================================================
 * 
 * 🏆 LeetCode 提交結果：
 * - 狀態：Accepted (29/29 測試案例全數通過！)
 * - 執行時間：10 ms
 * - 記憶體：12 MB
 * - Submission ID: 2167268620
 * 
 * 💡 邏輯亮點（滿分白板表現）：
 * 1. 在完全零提示下，秒寫出 Floyd 判圈演算法（龜兔賽跑演算法 Floyd's Cycle Detection）！
 * 2. 邊界保護嚴密：`while(fast != NULL && fast->next != NULL)` 完美防禦 `fast->next->next`
 *    可能觸發的 NULL 指標解引用 Segmentation Fault！
 * 3. 空間複雜度嚴格 O(1)，時間複雜度 O(N)，遠優於 Hash Table 的 O(N) 空間解。
 * 
 * 🔬 面試官連環追問 1：為什麼快指標走 2 步、慢指標走 1 步「一定會相遇」？會不會剛好跳過去？
 * -------------------------------------------------------------------------
 * - 假設當 slow 進入環後，fast 與 slow 在環內的距離差為 d（步數）。
 * - 每一輪迭代：slow 走 1 步，fast 走 2 步，兩者之間的相對距離縮小：2 - 1 = 1 步！
 * - 距離變化序列為：d, d-1, d-2, ..., 2, 1, 0。
 * - 由於每次只縮小「恰好 1 步」，因此距離必然會遞減到 0，**絕對不可能跨過（跳過）彼此外擦肩而過**！
 * 
 * 🔬 面試官連環追問 2：如果快指標每次走 3 步，還能保證一定相遇嗎？
 * -------------------------------------------------------------------------
 * - 若 fast 走 3 步，相對速度差為 3 - 1 = 2 步。
 * - 距離縮小為：d - 2。若環長度為偶數且起始距離為奇數，兩者有可能剛好以 1 步之差擦身而過，
 *   雖然在後續繞圈中仍可能因同餘理論最終相遇，但循環圈數與收斂時間將大幅增加！
 * - 因此「快 2 慢 1」是數學上保證在 1 圈內必然相遇的最優且最簡常數！
 * ========================================================================= */
