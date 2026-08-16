#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

// 節點結構定義
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 1. 快慢指標偵測是否有環 (Floyd's Cycle Detection)
// ============================================================================
bool hasCycle(struct ListNode *head)
{
    if (head == NULL || head->next == NULL)
    {
        return false;
    }

    struct ListNode *slow = head;
    struct ListNode *fast = head;

    // 標準迴圈條件：確保 fast 與 fast->next 不為 NULL
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;       // 慢指標每次走 1 步
        fast = fast->next->next; // 快指標每次走 2 步

        if (slow == fast)        // 如果快慢指標相遇，代表有環 (Cycle)！
        {
            return true;
        }
    }

    return false; // 快指標走到 NULL，代表沒有環
}

// ============================================================================
// 2. 尋找環的入口節點 (Floyd's Cycle-Finding Algorithm Part II: Entry Node)
// ============================================================================
/*
數學證明簡記：
L: 起點到環入口距離, b: 環入口到相遇點距離, c: 相遇點到環入口剩餘距離, C: 環長度 (C = b + c)
D_slow = L + b
D_fast = L + b + k*C
因為 D_fast = 2 * D_slow ➡️ L + b + k*C = 2(L + b) ➡️ L = k*C - b = (k-1)*C + c ➡️ L = c !
結論：一個指標從 Head 出發，另一個從相遇點出發，每次皆走 1 步，再次相遇處即為環的入口！
*/
struct ListNode *detectCycleEntry(struct ListNode *head)
{
    if (head == NULL || head->next == NULL)
    {
        return NULL;
    }

    struct ListNode *slow = head;
    struct ListNode *fast = head;

    // 階段 1：使用快慢指標找出相遇點
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) // 找到相遇點！
        {
            // 階段 2：ptr1 從 Head 出發，ptr2 從相遇點出發
            struct ListNode *ptr1 = head;
            struct ListNode *ptr2 = slow;

            // 兩者每次均走 1 步，直到再次相遇
            while (ptr1 != ptr2)
            {
                ptr1 = ptr1->next;
                ptr2 = ptr2->next;
            }

            return ptr1; // 精準命中環的入口節點！
        }
    }

    return NULL; // 無環
}

/*
// ⚠️ 你的原始 hasCycle 寫法對照與解析：
bool hasCycle_original(struct ListNode *head)
{
    if(head == NULL || head->next == NULL)
    {
        return false;
    }
    struct ListNode *fast = head;
    struct ListNode *slow = head;

    while((fast->next != NULL) && (fast->next->next != NULL))
    {
        fast = fast->next->next;
        slow = slow->next;
        if(slow == fast)
        {
            return true;
        }
    }
    return false;
}
*/