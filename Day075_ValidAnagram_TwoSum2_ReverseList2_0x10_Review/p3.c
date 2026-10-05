#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 單向鏈結串列節點定義
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 題目 3【反轉鏈結串列 II - 指定區間反轉 (Reverse Linked List II - LeetCode #92)】
// 難度：🟡 Medium (中等題) | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠高頻指標題)
// 演算法：Dummy Head + 局部頭插法 (Head-Insertion In-Place Reverse) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

struct ListNode* reverseBetween(struct ListNode* head, int left, int right)
{
    // 防呆處理 (若為空或 left == right 則不需要反轉)
    if (head == NULL || left == right)
    {
        return head;
    }

    // 1. 建立 dummy 哨兵節點，接在 head 前面
    struct ListNode dummy;
    dummy.val = 0;
    dummy.next = head;

    struct ListNode *pre = &dummy;

    // 2. pre 往前走 (left - 1) 步，精準停在「反轉區間的前一個節點」
    for (int i = 0; i < left - 1; i++)
    {
        pre = pre->next;
    }

    // 3. curr 指向反轉區間的第一個節點 (反轉完成後會變成該區間的尾巴)
    struct ListNode *curr = pre->next;

    // 4. 進行 (right - left) 次頭插操作，將後續節點一顆一顆拔到 pre 後面
    for (int i = 0; i < right - left; i++)
    {
        struct ListNode *next_node = curr->next; // 抓出要被拔起來往前插的節點
        curr->next = next_node->next;            // 把拔掉後的空隙接上
        next_node->next = pre->next;             // 將 next_node 插到 pre 後面
        pre->next = next_node;                   // 更新 pre 的下一個為剛插進來的節點
    }

    // 5. 回傳真正反轉後的頭節點
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
    // 測試 1: [1, 2, 3, 4, 5], left = 2, right = 4 -> [1, 4, 3, 2, 5]
    int a1[5] = {1, 2, 3, 4, 5};
    struct ListNode* l1 = arrayToList(a1, 5);
    l1 = reverseBetween(l1, 2, 4);
    int exp1[5] = {1, 4, 3, 2, 5};
    struct ListNode* curr = l1;
    for (int i = 0; i < 5; i++) {
        assert(curr != NULL && curr->val == exp1[i]);
        curr = curr->next;
    }
    assert(curr == NULL);
    printf("測試 1 通過: [1, 2, 3, 4, 5], left=2, right=4 ➔ [1, 4, 3, 2, 5]\n");
    freeList(l1);

    // 測試 2: [5], left = 1, right = 1 -> [5]
    int a2[1] = {5};
    struct ListNode* l2 = arrayToList(a2, 1);
    l2 = reverseBetween(l2, 1, 1);
    assert(l2 != NULL && l2->val == 5 && l2->next == NULL);
    printf("測試 2 通過: [5], left=1, right=1 ➔ [5]\n");
    freeList(l2);

    printf("\n🎉 p3.c 指定區間反轉鏈結串列 (Reverse Linked List II) 所有測試案例全數 100%% 通過！\n");
    return 0;
}
