#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// 單向鏈結串列節點定義
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 題目 1【環形鏈結串列 (Linked List Cycle - LeetCode #141)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠高頻經典題)
// 演算法：Floyd 快慢雙指標判圈法 (Tortoise and Hare) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

bool hasCycle(struct ListNode *head)
{
    // 1. 防呆處理：空串列必無環
    if (head == NULL)
    {
        return false;
    }

    // 2. 快慢雙指標初始皆指向 head
    struct ListNode *slow = head;
    struct ListNode *fast = head;
    
    // 3. fast 每次走 2 步，slow 每次走 1 步
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;

        // 若快慢指標相遇，代表串列中存在環！
        if (slow == fast)
        {
            return true;
        }
    }

    // 4. fast 抵達結尾 NULL，代表無環
    return false;
}

// 輔助函式：建立節點
struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

int main()
{
    // 測試 1: 3 -> 2 -> 0 -> -4，其中 -4 指向節點 2 (有環)
    struct ListNode* n1 = createNode(3);
    struct ListNode* n2 = createNode(2);
    struct ListNode* n3 = createNode(0);
    struct ListNode* n4 = createNode(-4);
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n2; // 形成環

    assert(hasCycle(n1) == true);
    printf("測試 1 通過: [3 -> 2 -> 0 -> -4 (連回 2)] 成功檢測出有環 (true)\n");

    // 測試 2: 1 -> 2 -> 3 -> NULL (無環)
    struct ListNode* a1 = createNode(1);
    struct ListNode* a2 = createNode(2);
    struct ListNode* a3 = createNode(3);
    a1->next = a2;
    a2->next = a3;
    assert(hasCycle(a1) == false);
    printf("測試 2 通過: [1 -> 2 -> 3 -> NULL] 正確檢測為無環 (false)\n");

    // 測試 3: NULL (空串列)
    assert(hasCycle(NULL) == false);
    printf("測試 3 通過: NULL 空串列正確檢測為無環 (false)\n");

    // 釋放無環串列記憶體
    free(a1); free(a2); free(a3);
    // 斷開環以釋放測試 1 記憶體
    n4->next = NULL;
    free(n1); free(n2); free(n3); free(n4);

    printf("\n🎉 p1.c 環形鏈結串列所有測試案例全數 100%% 通過！\n");
    return 0;
}
