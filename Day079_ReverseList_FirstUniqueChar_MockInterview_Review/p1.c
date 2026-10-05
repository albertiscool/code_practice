#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 單向鏈結串列節點定義
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 題目 1【反轉單向鏈結串列 (Reverse Linked List - LeetCode #206)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (全台灣所有 IC 廠/科技廠出題率 No.1 必考神題)
// 演算法：經典三指標雙向倒轉法 - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

struct ListNode* reverseList(struct ListNode* head)
{
    // 1. 初始化前驅指標 prev 為 NULL (因為反轉後的尾巴 next 必須指向 NULL)
    struct ListNode *prev = NULL;
    struct ListNode *curr = head;

    // 2. 逐一走訪每個節點，將箭頭向後翻轉
    while (curr != NULL)
    {
        struct ListNode *next_temp = curr->next; // 步驟一：先記住下一個節點 (防止斷鏈)
        curr->next = prev;                       // 步驟二：將當前節點指針向後倒轉
        prev = curr;                             // 步驟三：prev 往前走一步
        curr = next_temp;                        // 步驟四：curr 往前走一步
    }

    // 3. 當 curr 抵達 NULL 時，prev 剛好停在原本的最後一個節點 (即新的頭節點)
    return prev;
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
    // 測試 1: 1 -> 2 -> 3 -> 4 -> 5 反轉為 5 -> 4 -> 3 -> 2 -> 1
    int a1[5] = {1, 2, 3, 4, 5};
    struct ListNode* l1 = arrayToList(a1, 5);
    struct ListNode* r1 = reverseList(l1);

    int exp1[5] = {5, 4, 3, 2, 1};
    struct ListNode* curr = r1;
    for (int i = 0; i < 5; i++) {
        assert(curr != NULL && curr->val == exp1[i]);
        curr = curr->next;
    }
    assert(curr == NULL);
    printf("測試 1 通過: [1, 2, 3, 4, 5] 成功反轉為 [5, 4, 3, 2, 1]\n");
    freeList(r1);

    // 測試 2: NULL 反轉為 NULL
    struct ListNode* r2 = reverseList(NULL);
    assert(r2 == NULL);
    printf("測試 2 通過: 空串列 NULL 反轉正常 (NULL)\n");

    // 測試 3: 單節點 [42] 反轉為 [42]
    struct ListNode* l3 = createNode(42);
    struct ListNode* r3 = reverseList(l3);
    assert(r3 != NULL && r3->val == 42 && r3->next == NULL);
    printf("測試 3 通過: 單節點 [42] 反轉正常 [42]\n");
    freeList(r3);

    printf("\n🎉 p1.c 反轉單向鏈結串列所有測試案例全數 100%% 通過！\n");
    return 0;
}
