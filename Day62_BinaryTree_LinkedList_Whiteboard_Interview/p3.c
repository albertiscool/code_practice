#include <stdio.h>
#include <stdlib.h>

// 節點結構體
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 標準滿分解答：合併兩個已排序的鏈結串列 (Merge Two Sorted Lists)
// ============================================================================
struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2)
{
    // 1. 建立假頭節點 dummy 與移動工頭 tail
    struct ListNode dummy;
    dummy.next = NULL;
    struct ListNode *tail = &dummy;

    // 2. 當兩條鏈結串列都「同時還有節點」時才進行大小比較 (必須用 &&)
    while (list1 != NULL && list2 != NULL)
    {
        if (list1->val <= list2->val)
        {
            tail->next = list1;
            list1 = list1->next;
        }
        else
        {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next; // 每次接完，工頭往前推一格
    }

    // 3. 收尾神技：剩下的那條鏈結串列已經是排好序的，直接「一整串接上去」即可！(不需要 while 迴圈)
    if (list1 != NULL)
    {
        tail->next = list1;
    }
    else
    {
        tail->next = list2;
    }

    // 4. 回傳 dummy 後面接的第一個真實節點 (注意 dummy 是物件，用 . 存取)
    return dummy.next;
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
    // list1: 1 -> 2 -> 4
    struct ListNode *l1 = createNode(1);
    l1->next = createNode(2);
    l1->next->next = createNode(4);

    // list2: 1 -> 3 -> 4
    struct ListNode *l2 = createNode(1);
    l2->next = createNode(3);
    l2->next->next = createNode(4);

    printf("List 1: ");
    printList(l1);
    printf("List 2: ");
    printList(l2);

    struct ListNode *merged = mergeTwoLists(l1, l2);

    printf("Merged: ");
    printList(merged); // 預期: 1 -> 1 -> 2 -> 3 -> 4 -> 4 -> NULL

    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（整體架構與 dummy+tail 觀念 100% 正確！）：
struct ListNode* mergeTwoLists_original(struct ListNode* list1, struct ListNode* list2)
{
    struct ListNode dummy;
    struct ListNode *tail = &dummy;

    // 💡 盲點 1：條件必須是 && (AND)，不可用 || (OR)！
    // 如果用 ||，當其中一條已經走到 NULL 時，進迴圈存取 list1->val 會發生 Null Pointer 當機！
    // (另外條件括號內多了一個分號 ;)
    while(list1 != NULL && list2 != NULL)
    {
        if(list1->val <= list2->val)
        {
            tail->next = list1;
            list1 = list1->next;
            tail = tail->next;
        }
        else
        {
            tail->next = list2;
            list2 = list2->next;
            tail = tail->next;
        }
    }

    // 💡 盲點 2：收尾時不需要寫 while 迴圈！
    // 因為 list2 本身就是已經連好的一整條鏈結串列，直接 tail->next = list2 就把整條帶過來了！
    if(list1 == NULL)
    {
        tail->next = list2; // 一行搞定！
    }
    else
    {
        tail->next = list1; // 一行搞定！
    }

    // 💡 盲點 3：dummy 是實體物件（非指標），存取成員要用點 . (dummy.next)，不是箭頭 ->
    return dummy.next;
}
*/