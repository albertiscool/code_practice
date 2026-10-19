#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #82 - Remove Duplicates from Sorted List II
 * 
 * 題目描述：
 * 給定一個已排序的單向鏈結串列的頭節點 head。
 * 請刪除所有「具有重複數字」的節點，只保留原始串列中「只出現過一次」的數字。
 * 回傳已排序的鏈結串列。
 * 
 * 限制條件：
 * 串列中節點數在範圍 [0, 300] 內
 * -100 <= Node.val <= 100
 * 題目資料保證鏈結串列已經按升序排序
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

/* =========================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * -------------------------------------------------------------------------
 * struct ListNode* deleteDuplicates(struct ListNode* head) {
 *     if(head == NULL || head->next == NULL) return head;
 * 
 *     struct ListNode dummy;
 *     dummy.next = head;
 *     struct ListNode* pre = &dummy;
 * 
 *     struct ListNode* slow = head;
 *     struct ListNode* fast = head->next;
 *     while(fast != NULL)
 *     {
 *         if(fast->val == slow->val)
 *         {
 *             while(fast->val == slow->val)
 *             {
 *                 struct ListNode* next_node = fast->next;
 *                 free(fast);
 *                 fast = next_node;
 *             }
 *             free(slow);
 *             pre->next = fast;
 *             slow = fast;
 *             fast = fast->next;
 *         }
 *         else
 *         {
 *             pre = pre->next;
 *             slow = slow->next;
 *             fast = fast->next;
 *         }
 *     }
 *     return dummy.next;
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點剖析】：
 * 1. 邊界 NULL 指標解引用 (Null Pointer Dereference)：
 *    - 在內部 `while(fast->val == slow->val)` 迴圈中：
 *      當重複節點恰好延伸到鏈結串列末端時（例如 [1, 2, 3, 3]），
 *      最後一個 3 刪除後 `fast` 會被賦值為 NULL。
 *      下一次迴圈條件檢查 `fast->val` 時，直接存取 NULL 指標造成 Runtime Error！
 *      -> 防禦修復：改為 `while (fast != NULL && fast->val == slow->val)`。
 * 2. 尾端步進防禦：
 *    - 同樣在末端重複的情況下，離開內部迴圈時 `fast` 已經是 NULL。
 *    - 隨後若無條件執行 `fast = fast->next;`，會再次觸發 NULL 指標解引用。
 *      -> 防禦修復：加入 `if (fast != NULL) fast = fast->next;`。
 * =========================================================================
 */

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* deleteDuplicates(struct ListNode* head) {
    if (head == NULL || head->next == NULL) return head;

    struct ListNode dummy;
    dummy.next = head;
    struct ListNode* pre = &dummy;

    struct ListNode* slow = head;
    struct ListNode* fast = head->next;

    while (fast != NULL) {
        if (fast->val == slow->val) {
            // 防禦修復 1：加入 fast != NULL 防禦邊界
            while (fast != NULL && fast->val == slow->val) {
                struct ListNode* next_node = fast->next;
                free(fast);
                fast = next_node;
            }
            free(slow);
            pre->next = fast;
            slow = fast;
            // 防禦修復 2：若 fast 已至結尾，避免 NULL->next
            if (fast != NULL) {
                fast = fast->next;
            }
        } else {
            pre = pre->next;
            slow = slow->next;
            fast = fast->next;
        }
    }
    return dummy.next;
}

// 輔助測試函式
struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

void printList(struct ListNode* head) {
    struct ListNode* curr = head;
    while (curr) {
        printf("%d -> ", curr->val);
        curr = curr->next;
    }
    printf("NULL\n");
}

int main() {
    // 測試範例 1: 1 -> 2 -> 3 -> 3 -> 4 -> 4 -> 5
    struct ListNode* head1 = createNode(1);
    head1->next = createNode(2);
    head1->next->next = createNode(3);
    head1->next->next->next = createNode(3);
    head1->next->next->next->next = createNode(4);
    head1->next->next->next->next->next = createNode(4);
    head1->next->next->next->next->next->next = createNode(5);

    printf("Original 1: ");
    printList(head1);
    struct ListNode* res1 = deleteDuplicates(head1);
    printf("Result 1: ");
    printList(res1);
    printf("Expected 1: 1 -> 2 -> 5 -> NULL\n\n");

    // 測試範例 2: 1 -> 1 -> 1 -> 2 -> 3
    struct ListNode* head2 = createNode(1);
    head2->next = createNode(1);
    head2->next->next = createNode(1);
    head2->next->next->next = createNode(2);
    head2->next->next->next->next = createNode(3);

    printf("Original 2: ");
    printList(head2);
    struct ListNode* res2 = deleteDuplicates(head2);
    printf("Result 2: ");
    printList(res2);
    printf("Expected 2: 2 -> 3 -> NULL\n");

    return 0;
}
