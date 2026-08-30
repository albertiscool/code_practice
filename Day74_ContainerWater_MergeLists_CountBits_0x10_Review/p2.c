#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 單向鏈結串列節點定義
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 題目 2【合併兩個已排序鏈結串列 (Merge Two Sorted Lists - LeetCode #21)】
// 難度：🟢 Easy / 🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥 (白板必考穿針引線題)
// 演算法：Dummy Head 雙指標穿針引線法 - $O(N + M)$ 時間, $O(1)$ 空間
// ============================================================================

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2)
{
    // 防呆處理
    if (list1 == NULL) return list2;
    if (list2 == NULL) return list1;

    // 1. 建立哨兵節點 dummy，以及穿針引線指針 curr
    struct ListNode dummy;
    dummy.val = 0;
    dummy.next = NULL;
    struct ListNode *curr = &dummy;

    // 2. 只要兩條串列都還有節點 (list1 != NULL && list2 != NULL)，就比較並串接較小者
    while (list1 != NULL && list2 != NULL)
    {
        if (list1->val <= list2->val)
        {
            curr->next = list1;
            list1 = list1->next;
        }
        else
        {
            curr->next = list2;
            list2 = list2->next;
        }
        curr = curr->next;
    }

    // 3. 迴圈結束後，必有一條串列已空，將剩餘未走完的一整條直接接在 curr 後面！
    if (list1 == NULL)
    {
        curr->next = list2;
    }
    else
    {
        curr->next = list1;
    }

    // 4. 回傳真正合併後的頭節點
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
    // 測試 1: [1, 2, 4] 與 [1, 3, 4] -> [1, 1, 2, 3, 4, 4]
    int a1[3] = {1, 2, 4};
    int b1[3] = {1, 3, 4};
    struct ListNode* l1 = arrayToList(a1, 3);
    struct ListNode* l2 = arrayToList(b1, 3);
    struct ListNode* res1 = mergeTwoLists(l1, l2);

    int expected1[6] = {1, 1, 2, 3, 4, 4};
    struct ListNode* curr = res1;
    for (int i = 0; i < 6; i++) {
        assert(curr != NULL && curr->val == expected1[i]);
        curr = curr->next;
    }
    assert(curr == NULL);
    printf("測試 1 合併通過: [1, 2, 4] + [1, 3, 4] ➔ [1, 1, 2, 3, 4, 4]\n");
    freeList(res1);

    // 測試 2: [] 與 [0] -> [0]
    struct ListNode* l3 = NULL;
    struct ListNode* l4 = createNode(0);
    struct ListNode* res2 = mergeTwoLists(l3, l4);
    assert(res2 != NULL && res2->val == 0 && res2->next == NULL);
    printf("測試 2 合併通過: [] + [0] ➔ [0]\n");
    freeList(res2);

    printf("\n🎉 p2.c 合併兩個已排序鏈結串列所有測試案例全數 100%% 通過！\n");
    return 0;
}
