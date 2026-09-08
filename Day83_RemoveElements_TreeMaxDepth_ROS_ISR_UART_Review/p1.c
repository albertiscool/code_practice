#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 單向鏈結串列節點定義
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 題目 1【移除鏈結串列元素 (Remove Linked List Elements - LeetCode #203)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠白板高頻題)
// 演算法：虛擬頭節點 (Dummy Head) 單指標/雙指標刪除 - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

struct ListNode* removeElements(struct ListNode* head, int val)
{
    // 1. 建立虛擬頭節點 dummy，消除刪除 head 節點時的特殊邊界問題
    struct ListNode dummy;
    dummy.next = head;

    struct ListNode *pre = &dummy;
    struct ListNode *curr = head;

    // 2. 走訪整條串列
    while (curr != NULL)
    {
        if (curr->val == val)
        {
            struct ListNode *next_node = curr->next;
            pre->next = next_node; // 前驅節點跳過 curr，直接指向下一個
            free(curr);            // 釋放記憶體避免 Memory Leak
            curr = next_node;      // 關鍵一步：curr 推進到下一個節點！
        }
        else
        {
            pre = curr;            // 沒刪除時，pre 和 curr 雙雙往前進
            curr = curr->next;
        }
    }

    return dummy.next;
}

// 輔助函式：建立節點
struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

// 輔助函式：將陣列轉為鏈結串列
struct ListNode* arrayToList(int* arr, int size) {
    if (size == 0) return NULL;
    struct ListNode* head = createNode(arr[0]);
    struct ListNode* curr = head;
    for (int i = 1; i < size; i++) {
        curr->next = createNode(arr[i]);
        curr = curr->next;
    }
    return head;
}

// 輔助函式：釋放鏈結串列
void freeList(struct ListNode* head) {
    while (head) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main()
{
    // 測試 1: [1, 2, 6, 3, 4, 5, 6], val = 6 ➔ [1, 2, 3, 4, 5]
    int a1[] = {1, 2, 6, 3, 4, 5, 6};
    struct ListNode* l1 = arrayToList(a1, 7);
    struct ListNode* res1 = removeElements(l1, 6);
    int exp1[] = {1, 2, 3, 4, 5};
    struct ListNode* curr = res1;
    for (int i = 0; i < 5; i++) {
        assert(curr != NULL && curr->val == exp1[i]);
        curr = curr->next;
    }
    assert(curr == NULL);
    printf("測試 1 通過: [1, 2, 6, 3, 4, 5, 6] 移除 6 成功為 [1, 2, 3, 4, 5]\n");
    freeList(res1);

    // 測試 2: [] 移除 1 ➔ NULL
    struct ListNode* res2 = removeElements(NULL, 1);
    assert(res2 == NULL);
    printf("測試 2 通過: 空串列處理正常 (NULL)\n");

    // 測試 3: [7, 7, 7, 7], val = 7 ➔ [] (全部刪除)
    int a3[] = {7, 7, 7, 7};
    struct ListNode* l3 = arrayToList(a3, 4);
    struct ListNode* res3 = removeElements(l3, 7);
    assert(res3 == NULL);
    printf("測試 3 通過: [7, 7, 7, 7] 全數移除 7 成功為 NULL\n");

    printf("\n🎉 p1.c 移除鏈結串列元素所有測試案例全數 100%% 通過！\n");
    return 0;
}
