#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * LeetCode #206: Reverse Linked List
 * 
 * 題目描述：
 * 給定一個單向鏈結串列的頭節點 head，請反轉該鏈結串列，並回傳反轉後的鏈結串列頭節點。
 * 
 * 限制條件：
 * - 節點數目介於 [0, 5000]
 * - -5000 <= Node.val <= 5000
 * 
 * 範例 1:
 *   輸入: head = [1,2,3,4,5]
 *   輸出: [5,4,3,2,1]
 * 
 * 範例 2:
 *   輸入: head = [1,2]
 *   輸出: [2,1]
 * 
 * 範例 3:
 *   輸入: head = []
 *   輸出: []
 * 
 * 面試深意：
 * 全科技業（群聯 Phison、瑞昱 Realtek、聯發科 MTK、電子五哥）研替面試白板題「出現頻率 No. 1」！
 * 考驗三指標（prev, curr, next）原地轉向手術，杜絕斷鏈遺失節點與記憶體洩漏。
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* reverseList(struct ListNode* head) {
    // 請在此處實現純白板程式碼
    if(head == NULL || head->next == NULL)
    {
        return head;
    }

    struct ListNode* pre = NULL;
    struct ListNode* curr = head;
    while(curr != NULL)
    {
        struct ListNode* next_node = curr->next;
        curr->next = pre;
        pre = curr;
        curr = next_node;
    }

    return pre;
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
    printf("=== Test P3: Reverse Linked List ===\n");
    // Test 1: [1,2,3,4,5] -> [5,4,3,2,1]
    struct ListNode* n1 = createNode(1);
    struct ListNode* n2 = createNode(2);
    struct ListNode* n3 = createNode(3);
    struct ListNode* n4 = createNode(4);
    struct ListNode* n5 = createNode(5);
    n1->next = n2; n2->next = n3; n3->next = n4; n4->next = n5;

    printf("Original: ");
    printList(n1);

    struct ListNode* res1 = reverseList(n1);
    printf("Reversed: ");
    printList(res1);

    assert(res1 != NULL && res1->val == 5);
    assert(res1->next->val == 4);
    assert(res1->next->next->val == 3);
    assert(res1->next->next->next->val == 2);
    assert(res1->next->next->next->next->val == 1);
    assert(res1->next->next->next->next->next == NULL);
    printf("Test 1 Passed!\n");

    // Test 2: [1,2] -> [2,1]
    struct ListNode* m1 = createNode(1);
    struct ListNode* m2 = createNode(2);
    m1->next = m2;
    struct ListNode* res2 = reverseList(m1);
    assert(res2 != NULL && res2->val == 2);
    assert(res2->next->val == 1);
    assert(res2->next->next == NULL);
    printf("Test 2 Passed!\n");

    // Test 3: empty list NULL -> NULL
    assert(reverseList(NULL) == NULL);
    printf("Test 3 Passed (Empty list handled safely)\n");

    printf("All tests passed!\n");
    return 0;
}

/* =========================================================================
 * 🚩 【Albert 白板實戰複盤與鏈結串列原地轉向深度解析】
 * =========================================================================
 * 
 * 🏆 LeetCode 提交結果：
 * - 狀態：Accepted (28/28 測試案例全數通過！)
 * - 執行時間：0 ms (Beats 100.00%)
 * - 記憶體：11.3 MB
 * - Submission ID: 2166111312
 * 
 * 💡 邏輯亮點（滿分白板表現）：
 * 1. 經典三指標手術（pre, curr, next_node）行雲流水，原地完成反轉！
 * 2. 指針暫存順序嚴謹：在覆寫 `curr->next = pre` 之前，先將 `next_node = curr->next` 備份，
 *    杜絕單向鏈結串列最常見的「斷鏈節點遺失（Dangling/Lost Node）」死穴。
 * 3. 嚴格 O(N) 時間複雜度、O(1) 空間複雜度，完全無動態配置與多餘記憶體開銷。
 * 
 * 🔬 面試官追問深度延伸（遞迴法 vs 迭代法）：
 * -------------------------------------------------------------------------
 * 面試官很常追問：「這題除了你的 O(1) 迭代法，能不能用遞迴（Recursion）寫出來？兩者在韌體中有何差異？」
 * 
 * 遞迴寫法架構：
 * struct ListNode* reverseListRecursive(struct ListNode* head) {
 *     if (head == NULL || head->next == NULL) return head;
 *     struct ListNode* new_head = reverseListRecursive(head->next);
 *     head->next->next = head; // 讓原本的下一個節點指回自己
 *     head->next = NULL;       // 斷開原本的向前連接，防止形成環狀死結
 *     return new_head;
 * }
 * 
 * 韌體工程師高分應答點：
 * - 遞迴法雖然程式碼精煉，但在單晶片/嵌入式系統中，遞迴會佔用 Call Stack 空間（空間複雜度 O(N)）。
 * - 若鏈結長度達數千個節點，容易引發「Stack Overflow（堆疊溢出）」直接觸發 HardFault 當機。
 * - 因此在工業級韌體中，【迭代法（Albert 的解法）】是唯一被允許且最具可靠性的做法！
 * ========================================================================= */
