#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * LeetCode #61: Rotate List
 * 
 * 題目描述：
 * 給定一個單向鏈結串列的頭節點 head，請將該鏈結串列向右旋轉 k 個位置。
 * 
 * 限制條件：
 * - 節點數目介於 [0, 500]
 * - -100 <= Node.val <= 100
 * - 0 <= k <= 2 * 10^9
 * 
 * 範例 1:
 *   輸入: head = [1,2,3,4,5], k = 2
 *   輸出: [4,5,1,2,3]
 * 
 * 範例 2:
 *   輸入: head = [0,1,2], k = 4
 *   輸出: [2,0,1]
 * 
 * 範例 3:
 *   輸入: head = [], k = 0
 *   輸出: []
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* rotateRight(struct ListNode* head, int k) {
    // 邊界條件：空串列、單一節點、或旋轉 0 次，直接回傳
    if (head == NULL || head->next == NULL || k == 0) {
        return head;
    }

    // 1. 計算串列長度 len，並讓 curr 停在尾節點 (tail)
    struct ListNode* curr = head;
    int len = 1;
    while (curr->next != NULL) {
        curr = curr->next;
        len++;
    }

    // 2. 有效旋轉次數取模
    k = k % len;
    if (k == 0) {
        return head; // 旋轉為整數倍，無需操作
    }

    // 3. 首尾相接成環
    curr->next = head;

    // 4. 向右旋轉 k 步 = 新尾巴位於原尾巴往後走 (len - k) 步
    int steps_to_new_tail = len - k;
    for (int i = 0; i < steps_to_new_tail; i++) {
        curr = curr->next;
    }

    // 5. 斷環並取得新頭節點
    struct ListNode* new_head = curr->next;
    curr->next = NULL;

    return new_head;
}

// 輔助函式：建立與列印鏈結串列
struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

void printList(struct ListNode* head) {
    while (head) {
        printf("%d%s", head->val, head->next ? "->" : "");
        head = head->next;
    }
    printf("\n");
}

int main(void) {
    printf("=== Test P3: Rotate List ===\n");
    // Test 1: [1,2,3,4,5], k = 2 -> [4,5,1,2,3]
    struct ListNode* n1 = createNode(1);
    struct ListNode* n2 = createNode(2);
    struct ListNode* n3 = createNode(3);
    struct ListNode* n4 = createNode(4);
    struct ListNode* n5 = createNode(5);
    n1->next = n2; n2->next = n3; n3->next = n4; n4->next = n5;

    printf("Original: ");
    printList(n1);

    struct ListNode* res1 = rotateRight(n1, 2);
    printf("Rotated (k=2): ");
    printList(res1);

    assert(res1 != NULL && res1->val == 4);
    assert(res1->next->val == 5);
    assert(res1->next->next->val == 1);
    printf("Test 1 Passed!\n");

    // Test 2: [0,1,2], k = 4 -> [2,0,1]
    struct ListNode* m0 = createNode(0);
    struct ListNode* m1 = createNode(1);
    struct ListNode* m2 = createNode(2);
    m0->next = m1; m1->next = m2;

    struct ListNode* res2 = rotateRight(m0, 4);
    printf("Rotated (k=4): ");
    printList(res2);
    assert(res2 != NULL && res2->val == 2);
    assert(res2->next->val == 0);
    assert(res2->next->next->val == 1);
    printf("Test 2 Passed!\n");

    // Test 3: empty list
    assert(rotateRight(NULL, 10) == NULL);
    printf("Test 3 Passed: NULL list handled safely!\n");

    printf("All tests passed!\n");
    return 0;
}

/* =========================================================================
 * 🚩 【Albert 原始嘗試盲點剖析與韌體架構深入複盤】
 * =========================================================================
 * 
 * 原始撰寫程式碼：
 * -------------------------------------------------------------------------
 * struct ListNode* rotateRight(struct ListNode* head, int k) {
 *     if(head == NULL || k == 0)
 *     {
 *         return head;
 *     }
 *     struct ListNode* curr = head;
 *     int len = 0;
 *     while(curr != NULL && curr->next != NULL;) // ⚠️ 語法分號錯誤
 *     {
 *         curr = curr->next;
 *         len++;
 *     }
 *     k = k % len;                               // ⚠️ 若 len = 0 觸發除以零 SIGFPE
 *     curr->next = head;
 *     for(int i = 0; i < k; i++)                 // ⚠️ 步數方向顛倒（走 k 步是向左旋轉）
 *     {
 *         head = head->next;
 *         curr = curr->next;
 *     }
 *     curr->next = NULL;
 *     return head;
 * }
 * -------------------------------------------------------------------------
 * 
 * 💡 邏輯亮點：
 * 1. 成功想到將尾節點 curr 與 head 相連（curr->next = head）形成閉環。
 * 2. 成功想到用模運算 `k = k % len` 解決 k 高達 2*10^9 的超大旋轉量。
 * 
 * ❌ 盲點 1：長度計算初始值偏差（Off-by-one 與除以零崩潰）
 * -------------------------------------------------------------------------
 * - curr 從 head（第 1 個節點）出發，停在尾節點時走過的步數比總長度少 1。
 * - 若 len 初始化為 0，則 5 個節點只算到 len = 4。
 * - 若串列只有 1 個節點 [1]，while 迴圈根本不執行，len 停在 0，下一行 `k % 0` 直接觸發 Floating Point Exception (SIGFPE) 當機！
 * - 修正：`len` 應初始化為 1，`while(curr->next != NULL)` 結束時 len 正好是真實長度。
 * 
 * ❌ 盲點 2：向右旋轉 vs 向左旋轉（步數方向）
 * -------------------------------------------------------------------------
 * - 單向鏈結串列指標只能「往前走」。
 * - 將串列「向右旋轉 k 步」，代表倒數第 k 個節點成為新的頭部。
 * - 因此「新的尾巴」在原尾巴往後走 `len - k` 步，而不是走 `k` 步！
 * - 走 k 步的結果是「向左旋轉」（例如 [1,2,3,4,5], k=2 走 2 步會得到 [3,4,5,1,2]，而非正確的 [4,5,1,2,3]）。
 * ========================================================================= */
