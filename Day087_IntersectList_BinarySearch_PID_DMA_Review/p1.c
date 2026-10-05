#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Definition for singly-linked list.
 */
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 題目 1【相交鏈結串列 (Intersection of Two Linked Lists - LeetCode #160)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (聯發科、瑞昱、群聯、台達白板高頻經典題)
// 題目說明：給你兩個單向鏈結串列的頭節點 headA 和 headB，請你找出並返回兩個單向鏈結串列
//           相交的起始節點。如果兩個鏈結串列不存在相交節點，返回 NULL。
//           整個過程不可破壞原有鏈結串列的結構。
// 範例 1：
//    A:          a1 -> a2 \
//                          c1 -> c2 -> c3
//    B:    b1 -> b2 -> b3 /
//    輸出：相交節點 c1
// 範例 2：
//    A:    1 -> 2
//    B:    3 -> 4
//    輸出：NULL (不相交)
// 限制條件：
//    - listA 中節點數目為 m，listB 中節點數目為 n。
//    - 1 <= m, n <= 3 * 10^4
//    - 1 <= Node.val <= 10^5
//    - 時間複雜度必須為 O(m + n)，且僅能使用 O(1) 額外空間！
// ============================================================================

struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) {
    if (headA == NULL || headB == NULL) {
        return NULL;
    }
    
    struct ListNode *currA = headA;
    struct ListNode *currB = headB;
    
    // 當 currA == currB 時跳出迴圈：
    // 1. 若兩鏈結串列相交，兩指針走過相同總步數 (lenA + lenB) 後，會在交點處相遇 (currA == currB == 交點)。
    // 2. 若兩鏈結串列不相交，兩指針走完各自與對方的長度後，會「同時」抵達 NULL (currA == currB == NULL)，安全跳出！
    while (currA != currB) {
        currA = (currA == NULL) ? headB : currA->next;
        currB = (currB == NULL) ? headA : currB->next;
    }
    
    return currA;
}

/*
 * ============================================================================
 * 【原始錯誤實作紀錄與盲點分析】
 * 錯誤原因：
 * 1. 致命無限迴圈 (Infinite Loop)：
 *    在迴圈內寫：
 *       currA = currA->next;
 *       if (currA == NULL) currA = headB;
 *    這導致 currA 與 currB 只要一走到尾端 NULL，就會「立刻被強制換頭重新出發」。
 *    指針本身永遠不會以 NULL 的狀態參與 `while (currA != currB)` 的相等比對！
 * 2. 後果：
 *    當兩個鏈結串列「完全不相交」時，因為指針永遠停不下來（永不保持為 NULL），
 *    兩者會在兩個串列之間無止境地來回漫遊，導致程式進入死循環 (TLE / 凍結)！
 *
 * 正確思維：
 * 必須先判斷目前是不是 NULL：若當前是 NULL 才換頭，否則走 next。
 * 讓 NULL 作為實質的節點參與步數對齊，這樣不相交時兩者最後一步才會同時為 NULL 並正常終止。
 * ============================================================================
struct ListNode *getIntersectionNode_wrong(struct ListNode *headA, struct ListNode *headB) {
    if(headA == NULL || headB == NULL)
    {
        return NULL;
    }
    struct ListNode *currA = headA;
    struct ListNode *currB = headB;
    while(currA != currB)
    {
        currA = currA->next;
        currB = currB->next;
        if(currA == NULL)
        {
            currA = headB; // ❌ 盲點：走到 NULL 立即被換頭，指針永遠不可能同時為 NULL 終止！
        }
        if(currB == NULL)
        {
            currB = headA;
        }
    }
    return currA;
}
*/

/* 輔助函式：建立新節點 */
struct ListNode* createNode(int val) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

int main(void) {
    /* 建立相交測試測資:
     * A: 4 -> 1 \
     *             8 -> 4 -> 5
     * B: 5 -> 6 -> 1 /
     */
    struct ListNode* common = createNode(8);
    common->next = createNode(4);
    common->next->next = createNode(5);

    // List A
    struct ListNode* headA = createNode(4);
    headA->next = createNode(1);
    headA->next->next = common;

    // List B
    struct ListNode* headB = createNode(5);
    headB->next = createNode(6);
    headB->next->next = createNode(1);
    headB->next->next->next = common;

    struct ListNode* ans1 = getIntersectionNode(headA, headB);
    printf("測資 1 結果: 相交於數值 %d (預期: 8)\n", ans1 ? ans1->val : -1);

    /* 建立不相交測資:
     * A: 2 -> 6 -> 4
     * B: 1 -> 5
     */
    struct ListNode* noCrossA = createNode(2);
    noCrossA->next = createNode(6);
    noCrossA->next->next = createNode(4);

    struct ListNode* noCrossB = createNode(1);
    noCrossB->next = createNode(5);

    struct ListNode* ans2 = getIntersectionNode(noCrossA, noCrossB);
    printf("測資 2 結果: %s (預期: NULL)\n", ans2 == NULL ? "NULL" : "非 NULL");

    // 釋放記憶體
    free(headA->next); free(headA);
    free(headB->next->next); free(headB->next); free(headB);
    free(common->next->next); free(common->next); free(common);
    free(noCrossA->next->next); free(noCrossA->next); free(noCrossA);
    free(noCrossB->next); free(noCrossB);

    return 0;
}
