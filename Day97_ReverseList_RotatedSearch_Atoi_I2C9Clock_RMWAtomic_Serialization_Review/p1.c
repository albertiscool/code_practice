#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 題目 1【反轉單向鏈結串列 (Reverse Linked List - LeetCode #206)】
// 難度：🟡 一線 IC 設計廠 (聯詠/瑞昱/聯發科) & 系統廠高頻指標題
// 出題頻率：🔥🔥🔥🔥🔥 (韌體面試動態指標操作第一考點)
// 題目說明：給你單向鏈結串列的頭節點 head ，請你反轉該串列，並返回反轉後的鏈結串列頭節點。
//           你必須「原地 (in-place)」修改節點指標，時間複雜度 O(N)，額外空間複雜度 O(1)。
// 範例 1：
//    輸入：head = [1, 2, 3, 4, 5]
//    輸出：[5, 4, 3, 2, 1]
// 範例 2：
//    輸入：head = [1, 2]
//    輸出：[2, 1]
// 範例 3：
//    輸入：head = []
//    輸出：[]
// 限制條件：
//    - 節點數量在範圍 [0, 5000] 內
//    - -5000 <= Node.val <= 5000
//    - ⚠️ 禁止配置任何新節點，請純粹操作指標指向 (prev, curr, next)！
// ============================================================================

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* reverseList(struct ListNode* head) {
    // TODO: 請在純白板模式下完成實作 (請勿尋求提示)
    if(head == NULL)
    {
        return NULL;
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

// 輔助函式：建立節點
static struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

// 輔助函式：列印串列
static void printList(struct ListNode* head) {
    printf("[");
    struct ListNode* curr = head;
    while (curr) {
        printf("%d%s", curr->val, curr->next ? " -> " : "");
        curr = curr->next;
    }
    printf("]\n");
}

// 輔助函式：釋放串列
static void freeList(struct ListNode* head) {
    while (head) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    /* 測試測資 1: 1 -> 2 -> 3 -> 4 -> 5 */
    struct ListNode* head1 = createNode(1);
    head1->next = createNode(2);
    head1->next->next = createNode(3);
    head1->next->next->next = createNode(4);
    head1->next->next->next->next = createNode(5);

    printf("測資 1 原串列: ");
    printList(head1);
    head1 = reverseList(head1);
    printf("測資 1 反轉後: ");
    printList(head1);
    printf("預期結果    : [5 -> 4 -> 3 -> 2 -> 1]\n\n");
    freeList(head1);

    /* 測試測資 2: 1 -> 2 */
    struct ListNode* head2 = createNode(1);
    head2->next = createNode(2);
    head2 = reverseList(head2);
    printf("測資 2 反轉後: ");
    printList(head2);
    printf("預期結果    : [2 -> 1]\n\n");
    freeList(head2);

    /* 測試測資 3: 空串列 [] */
    struct ListNode* head3 = reverseList(NULL);
    printf("測資 3 (NULL): ");
    printList(head3);
    printf("預期結果    : []\n");

    return 0;
}
