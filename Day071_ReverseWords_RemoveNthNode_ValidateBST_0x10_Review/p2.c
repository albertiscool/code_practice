#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 單向鏈結串列節點定義
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 題目 2【刪除鏈結串列的倒數第 N 個節點 (Remove Nth Node From End of List - LeetCode #19)】
// 難度：🟡 Medium (中等題) | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：快慢雙指標 + 哨兵假節點 (Fast & Slow Pointers with Dummy Head) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================
struct ListNode* removeNthFromEnd(struct ListNode* head, int n)
{
    // 防呆處理
    if (head == NULL)
    {
        return NULL;
    }

    // 1. 建立哨兵節點 dummy (接在 head 前面，完美處理刪除頭節點的情況)
    struct ListNode dummy;
    dummy.val = 0;
    dummy.next = head; // head 本身就是 struct ListNode* 指標

    struct ListNode *slow = &dummy;
    struct ListNode *fast = &dummy;

    // 2. 讓 fast 先往前走 n + 1 步，與 slow 拉開 (n + 1) 個間距
    for (int i = 0; i < n + 1; i++)
    {
        if (fast == NULL) break;
        fast = fast->next;
    }

    // 3. fast 與 slow 同步每次走 1 步，直到 fast 走到結尾 NULL
    while (fast != NULL)
    {
        slow = slow->next;
        fast = fast->next;
    }

    // 4. 此時 slow 剛好停在要被刪除節點的「前一個節點」！
    // 跳過目標節點以完成刪除
    struct ListNode* to_delete = slow->next;
    slow->next = slow->next->next;
    
    // (可選) 釋放被刪除的節點記憶體 free(to_delete)

    // 5. 回傳真正的頭節點
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
    // 測試 1: [1, 2, 3, 4, 5], n = 2 -> [1, 2, 3, 5]
    int arr1[5] = {1, 2, 3, 4, 5};
    struct ListNode* list1 = arrayToList(arr1, 5);
    list1 = removeNthFromEnd(list1, 2);
    assert(list1->val == 1);
    assert(list1->next->val == 2);
    assert(list1->next->next->val == 3);
    assert(list1->next->next->next->val == 5);
    assert(list1->next->next->next->next == NULL);
    printf("測試 1 通過: [1, 2, 3, 4, 5] 刪除倒數第 2 個 ➔ [1, 2, 3, 5]\n");
    freeList(list1);

    // 測試 2: [1, 2], n = 1 -> [1]
    int arr2[2] = {1, 2};
    struct ListNode* list2 = arrayToList(arr2, 2);
    list2 = removeNthFromEnd(list2, 1);
    assert(list2->val == 1);
    assert(list2->next == NULL);
    printf("測試 2 通過: [1, 2] 刪除倒數第 1 個 ➔ [1]\n");
    freeList(list2);

    // 測試 3: [1], n = 1 -> []
    int arr3[1] = {1};
    struct ListNode* list3 = arrayToList(arr3, 1);
    list3 = removeNthFromEnd(list3, 1);
    assert(list3 == NULL);
    printf("測試 3 通過: [1] 刪除倒數第 1 個 ➔ NULL\n");
    freeList(list3);

    printf("\n🎉 p2.c 刪除鏈結串列倒數第 N 個節點所有測試案例全數 100%% 通過！\n");
    return 0;
}
