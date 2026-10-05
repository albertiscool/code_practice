#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// 單向鏈結串列節點
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 題目 3【鏈結串列環的入口點 (Linked List Cycle II - LeetCode #142)】
// 難度：🟡 Medium (中等題) | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：Floyd 龜兔賽跑兩階段演算法 ($O(N)$ 時間, $O(1)$ 空間)
// ============================================================================
struct ListNode *detectCycle(struct ListNode *head)
{
    struct ListNode *slow = head;
    struct ListNode *fast = head;

    // 階段 1：檢測是否有環 (slow 走 1 步，fast 走 2 步)
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;

        // 兩指標相遇，說明有環！
        if (slow == fast)
        {
            // 階段 2：尋找環的入口點
            // 把 slow 重新放回 head，fast 留在原相遇點
            slow = head;

            // 🌟 關鍵修正：兩個人此時改為「每次都只走 1 步」！
            while (slow != fast)
            {
                slow = slow->next;
                fast = fast->next; // 走 1 步 (不可再走 2 步)
            }

            // 再次相遇點 100% 恰好就是環的入口點！
            return slow;
        }
    }

    // 若 fast 走到 NULL，說明沒有環
    return NULL;
}

// 輔助函式：建立新節點
struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

int main()
{
    // 建立帶環鏈結串列: 3 -> 2 -> 0 -> -4
    //                      ^            |
    //                      |____________| (入口在 2)
    struct ListNode* node3 = createNode(3);
    struct ListNode* node2 = createNode(2);
    struct ListNode* node0 = createNode(0);
    struct ListNode* node4 = createNode(-4);

    node3->next = node2;
    node2->next = node0;
    node0->next = node4;
    node4->next = node2; // 環的入口在 node2 (val = 2)

    // 測試帶環鏈結串列
    struct ListNode* entry = detectCycle(node3);
    assert(entry != NULL);
    printf("測試 1 環入口數值: %d (預期: 2)\n", entry->val);
    assert(entry == node2);

    // 測試無環鏈結串列: 1 -> 2 -> NULL
    struct ListNode* n1 = createNode(1);
    n1->next = createNode(2);
    assert(detectCycle(n1) == NULL);

    printf("\n🎉 p3.c 鏈結串列環入口點 所有測試案例全數 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（兩階段架構與 slow=head 重置 100% 正確！）：
struct ListNode *detectCycle_original(struct ListNode *head)
{
    struct ListNode *slow = head;
    struct ListNode *fast = head;
    while(fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast)
        {
            slow = head;
            while(slow != fast)
            {
                slow = slow->next;
                // 💡 唯一微小筆誤：
                // 進入階段 2 尋找入口點時，fast 必須改為每次只走 1 步 (fast = fast->next)！
                // 如果寫成 fast = fast->next->next，速度不同兩者無法在入口點碰頭！
                fast = fast->next->next;
            }
            return slow;
        }
    }
    return NULL;
}
*/
