#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Definition for singly-linked list.
 */
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 題目 1【迴文鏈結串列 (Palindrome Linked List - LeetCode #234)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (聯發科、瑞昱、群聯、台達白板高頻經典題)
// 題目說明：給定一個單向鏈結串列的頭節點 head，請判斷該鏈結串列是否為迴文。
//           如果是，返回 true；否則，返回 false。
// 範例 1：
//    輸入：head = [1,2,2,1]
//    輸出：true
// 範例 2：
//    輸入：head = [1,2]
//    輸出：false
// 限制條件：
//    - 鏈結串列中的節點數目在範圍 [1, 10^5] 內。
//    - 0 <= Node.val <= 9
//    - 進階挑戰：你能否用 O(N) 時間複雜度和 O(1) 空間複雜度解決此題？
// ============================================================================

bool isPalindrome(struct ListNode* head) {
    if (head == NULL || head->next == NULL) {
        return true;
    }

    // 1. 快慢雙指針尋找中點
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 2. 反轉後半段鏈結串列 (從 slow 開始反轉)
    struct ListNode* prev = NULL;
    struct ListNode* curr = slow;
    while (curr != NULL) {
        struct ListNode* next_node = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next_node;
    }

    // 3. 前後兩段同步比對 (p1 從 head 出發，p2 從反轉後的尾巴 prev 出發)
    struct ListNode* p1 = head;
    struct ListNode* p2 = prev;
    while (p2 != NULL) {
        if (p1->val != p2->val) {
            return false;
        }
        p1 = p1->next;
        p2 = p2->next;
    }

    return true;
}

/*
 * ============================================================================
 * 【原始錯誤實作紀錄與盲點分析】
 * 錯誤原因：
 * 1. 致命邏輯運算子錯誤 (導致 Segfault 崩潰)：
 *    `while(fast != NULL || fast->next != NULL)` 誤用了 `||`！
 *    當 fast 走到最後一個節點時 (fast->next == NULL)，因為短路特性，
 *    迴圈內執行 `fast->next->next` 會直接對 NULL 取 next，引發記憶體存取崩潰！
 *    必須是邏輯且：`while (fast != NULL && fast->next != NULL)`。
 * 2. 鏈結串列成環 (Cycle in Linked List)：
 *    反轉時令 `pre = slow; curr = slow->next;`，接著執行 `curr->next = pre;`，
 *    但此時 slow->next 原本就指向 curr，兩者互相指來指去形成了「死循環環形鏈結」！
 *    反轉時 prev 的初始值必須是 NULL。
 * 3. 比對指標錯位：
 *    反轉結束後，後半段的新頭節點是 `pre` (原尾巴)，
 *    後續比對時卻拿舊的 `slow` 去走訪，導致比對了錯誤的節點。
 * ============================================================================
bool isPalindrome_wrong(struct ListNode* head) {
    if(head == NULL)
    {
        return false;
    }
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    while(fast != NULL || fast->next != NULL) // ❌ 盲點 1：誤用 ||，當 fast->next 為 NULL 時造成 Segfault
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    struct ListNode* curr = slow->next;
    struct ListNode* pre = slow;
    while(curr != NULL)
    {
        struct ListNode* next_node = curr->next;
        curr->next = pre; // ❌ 盲點 2：slow 與 curr 互相指向，鏈結串列直接成環！
        pre = curr;
        curr = next_node;
    }
    struct ListNode* slow2 = head;
    while(slow != NULL) // ❌ 盲點 3：應拿反轉後的新頭節點 pre 與 head 比對，而非 slow
    {
        if(slow->val != slow2->val)
        {
            return false;
        }
        slow = slow->next;
        slow2 = slow2->next;
    }

    return true;
}
*/

/* 輔助函式：建立新節點 */
struct ListNode* createNode(int val) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

/* 輔助函式：釋放鏈結串列 */
void freeList(struct ListNode* head) {
    while (head != NULL) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    /* 測試測資 1: 1 -> 2 -> 2 -> 1 (true) */
    struct ListNode* h1 = createNode(1);
    h1->next = createNode(2);
    h1->next->next = createNode(2);
    h1->next->next->next = createNode(1);

    bool ans1 = isPalindrome(h1);
    printf("測資 1 結果: %s (預期: true)\n", ans1 ? "true" : "false");
    freeList(h1);

    /* 測試測資 2: 1 -> 2 (false) */
    struct ListNode* h2 = createNode(1);
    h2->next = createNode(2);

    bool ans2 = isPalindrome(h2);
    printf("測資 2 結果: %s (預期: false)\n", ans2 ? "true" : "false");
    freeList(h2);

    /* 測試測資 3: 1 -> 2 -> 3 -> 2 -> 1 (奇數長度, true) */
    struct ListNode* h3 = createNode(1);
    h3->next = createNode(2);
    h3->next->next = createNode(3);
    h3->next->next->next = createNode(2);
    h3->next->next->next->next = createNode(1);

    bool ans3 = isPalindrome(h3);
    printf("測資 3 結果: %s (預期: true)\n", ans3 ? "true" : "false");
    freeList(h3);

    return 0;
}
