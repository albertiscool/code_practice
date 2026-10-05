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
// 題目 2【兩個單向鏈結串列的相交節點 (Intersection of Two Linked Lists - LeetCode #160)】
// 難度：🟢 Easy / 🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥 (聯發科/Garmin/瑞昱高頻)
// 演算法：浪漫雙指標對換走訪法 ($O(N+M)$ 時間, $O(1)$ 空間)
// ============================================================================
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB)
{
    if (headA == NULL || headB == NULL) return NULL;

    struct ListNode *pA = headA;
    struct ListNode *pB = headB;

    // 只要 pA != pB 就繼續向前推進
    while (pA != pB)
    {
        // 🌟 關鍵點：必須是 (pA == NULL)，而不是 (pA->next == NULL)！
        // 讓指標真正踏上 NULL 格子，才能確保兩者走過的總步數精準等於 (A長度 + B長度)！
        // 且若不相交時，兩者會同時變為 NULL，從而平靜跳出迴圈。
        pA = (pA == NULL) ? headB : pA->next;
        pB = (pB == NULL) ? headA : pB->next;
    }

    // 迴圈跳出時，pA 要麼指向相交節點，要麼指向 NULL！
    return pA;
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
    // 建立公共相交部分: 8 -> 4 -> 5
    struct ListNode* common = createNode(8);
    common->next = createNode(4);
    common->next->next = createNode(5);

    // 建立 A 鏈結串列: 4 -> 1 -> (common)
    struct ListNode* headA = createNode(4);
    headA->next = createNode(1);
    headA->next->next = common;

    // 建立 B 鏈結串列: 5 -> 6 -> 1 -> (common)
    struct ListNode* headB = createNode(5);
    headB->next = createNode(6);
    headB->next->next = createNode(1);
    headB->next->next->next = common;

    // 測試相交節點
    struct ListNode* intersect = getIntersectionNode(headA, headB);
    assert(intersect != NULL);
    printf("測試 1 相交節點數值: %d (預期: 8)\n", intersect->val);
    assert(intersect->val == 8);

    printf("\n🎉 p2.c 鏈結串列相交節點 所有測試案例全數 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討：
struct ListNode *getIntersectionNode_original(struct ListNode *headA, struct ListNode *headB)
{
    struct ListNode *pA = headA;
    struct ListNode *pB = headB;
    while(pA != pB)
    {
        // 💡 盲點 1：寫成 (pA->next == NULL) 
        // 這會導致指標還沒踏上 NULL 就提前跳躍，步數少算了 1 步！
        // 更嚴重的後果是：如果兩條串列「完全不相交」，指標永遠踏不到 NULL，
        // 兩邊指標會在 headA 和 headB 之間無窮跳躍 (死迴圈)！
        
        pA = (pA->next == NULL) ? headB : pA->next;
        pB = (pB->next == NULL) ? headA : pB->next;
        
        // 💡 盲點 2：不需要內部 if 判斷
        // 只要改成 (pA == NULL)，當 pA == pB 時自然會跳出 while 迴圈！
    }
    return NULL;
}
*/
