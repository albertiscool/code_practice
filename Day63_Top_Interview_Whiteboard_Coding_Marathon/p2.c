#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 節點結構體
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 標準滿分解答：尋找鏈結串列中間節點 (Middle of the Linked List) - 快慢指標法
// ============================================================================
struct ListNode* middleNode(struct ListNode* head)
{
    // 防呆邊界條件
    if (head == NULL)
    {
        return NULL;
    }

    struct ListNode *slow = head;
    struct ListNode *fast = head;

    // 快慢指標核心迴圈：快指標每次 2 步，慢指標每次 1 步
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow; // 快指標到終點時，慢指標恰好落在中間！
}

// 輔助函式：建立新節點
struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

// 輔助函式：列印鏈結串列
void printList(struct ListNode* head) {
    while (head != NULL) {
        printf("%d -> ", head->val);
        head = head->next;
    }
    printf("NULL\n");
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    // 測試案例 1：奇數長度 [1, 2, 3, 4, 5] -> 中間節點為 3
    struct ListNode *l1 = createNode(1);
    l1->next = createNode(2);
    l1->next->next = createNode(3);
    l1->next->next->next = createNode(4);
    l1->next->next->next->next = createNode(5);

    printf("奇數鏈結串列: ");
    printList(l1);
    struct ListNode *mid1 = middleNode(l1);
    printf("中間節點值: %d (預期: 3)\n\n", mid1->val);
    assert(mid1->val == 3);

    // 測試案例 2：偶數長度 [1, 2, 3, 4, 5, 6] -> 第二個中間節點為 4
    struct ListNode *l2 = createNode(1);
    l2->next = createNode(2);
    l2->next->next = createNode(3);
    l2->next->next->next = createNode(4);
    l2->next->next->next->next = createNode(5);
    l2->next->next->next->next->next = createNode(6);

    printf("偶數鏈結串列: ");
    printList(l2);
    struct ListNode *mid2 = middleNode(l2);
    printf("中間節點值: %d (預期: 4)\n\n", mid2->val);
    assert(mid2->val == 4);

    printf("🎉 所有測試案例全部 100%% 通過！\n");
    return 0;
}