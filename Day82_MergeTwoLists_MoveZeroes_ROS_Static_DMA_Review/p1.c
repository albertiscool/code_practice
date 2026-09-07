#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 單向鏈結串列節點定義
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 題目 1【合併兩個已排序的鏈結串列 (Merge Two Sorted Lists - LeetCode #21)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (全台灣所有科技廠/IC 廠白板題霸榜前 3 名)
// 演算法：虛擬頭節點 (Dummy Head) 雙指針穿針引線 - $O(N + M)$ 時間, $O(1)$ 空間
// ============================================================================

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2)
{
    // 1. 防呆守門員：若其中一條為空，直接回傳另一條
    if (list1 == NULL) return list2;
    if (list2 == NULL) return list1;

    // 2. 建立虛擬頭節點 dummy，尾指針 curr 起點直接指向 &dummy
    struct ListNode dummy;
    dummy.next = NULL;
    struct ListNode *curr = &dummy;

    // 3. 雙指針比較：誰小就接誰
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

    // 4. 收尾大招：將剩餘未走訪完的一整段鏈結直接串上（無需再跑迴圈）
    curr->next = (list1 != NULL) ? list1 : list2;

    // 5. 回傳真實的新頭節點
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
    // 測試 1: [1, 2, 4] 與 [1, 3, 4] 合併 ➔ [1, 1, 2, 3, 4, 4]
    int a1[] = {1, 2, 4};
    int b1[] = {1, 3, 4};
    struct ListNode* l1 = arrayToList(a1, 3);
    struct ListNode* l2 = arrayToList(b1, 3);
    struct ListNode* res1 = mergeTwoLists(l1, l2);

    int exp1[] = {1, 1, 2, 3, 4, 4};
    struct ListNode* curr = res1;
    for (int i = 0; i < 6; i++) {
        assert(curr != NULL && curr->val == exp1[i]);
        curr = curr->next;
    }
    assert(curr == NULL);
    printf("測試 1 通過: [1, 2, 4] 與 [1, 3, 4] 成功合併為 [1, 1, 2, 3, 4, 4]\n");
    freeList(res1);

    // 測試 2: [] 與 [] ➔ NULL
    struct ListNode* res2 = mergeTwoLists(NULL, NULL);
    assert(res2 == NULL);
    printf("測試 2 通過: 空串列合併正常 (NULL)\n");

    // 測試 3: [] 與 [0] ➔ [0]
    int b3[] = {0};
    struct ListNode* l3_2 = arrayToList(b3, 1);
    struct ListNode* res3 = mergeTwoLists(NULL, l3_2);
    assert(res3 != NULL && res3->val == 0 && res3->next == NULL);
    printf("測試 3 通過: [] 與 [0] 合併為 [0]\n");
    freeList(res3);

    printf("\n🎉 p1.c 合併兩個排序鏈結串列所有測試案例全數 100%% 通過！\n");
    return 0;
}
