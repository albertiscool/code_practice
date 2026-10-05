#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 單向鏈結串列節點定義
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 題目 1【鏈結串列的中間節點 (Middle of the Linked List - LeetCode #876)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠白板高頻題)
// 演算法：快慢雙指標法 (Fast & Slow Pointers) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

struct ListNode* middleNode(struct ListNode* head)
{
    // 防呆處理
    if (head == NULL)
    {
        return NULL;
    }
    
    // 快慢雙指標：起點皆從 head 出發
    // slow 每次走 1 步，fast 每次走 2 步
    // 當 fast 走到尾巴時，slow 剛好停在正中間！
    struct ListNode *slow = head;
    struct ListNode *fast = head;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
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
    // 測試 1: [1, 2, 3, 4, 5] (奇數長度) ➔ 中間節點為 3
    int a1[] = {1, 2, 3, 4, 5};
    struct ListNode* l1 = arrayToList(a1, 5);
    struct ListNode* mid1 = middleNode(l1);
    assert(mid1 != NULL && mid1->val == 3);
    printf("測試 1 通過: [1, 2, 3, 4, 5] 中間節點為: %d (預期: 3)\n", mid1->val);
    freeList(l1);

    // 測試 2: [1, 2, 3, 4, 5, 6] (偶數長度) ➔ 第二個中間節點為 4
    int a2[] = {1, 2, 3, 4, 5, 6};
    struct ListNode* l2 = arrayToList(a2, 6);
    struct ListNode* mid2 = middleNode(l2);
    assert(mid2 != NULL && mid2->val == 4);
    printf("測試 2 通過: [1, 2, 3, 4, 5, 6] 中間節點為: %d (預期: 4)\n", mid2->val);
    freeList(l2);

    // 測試 3: 單節點 [42] ➔ 中間節點為 42
    struct ListNode* l3 = createNode(42);
    struct ListNode* mid3 = middleNode(l3);
    assert(mid3 != NULL && mid3->val == 42);
    printf("測試 3 通過: 單節點 [42] 中間節點為: %d (預期: 42)\n", mid3->val);
    freeList(l3);

    printf("\n🎉 p1.c 鏈結串列中間節點所有測試案例全數 100%% 通過！\n");
    return 0;
}
