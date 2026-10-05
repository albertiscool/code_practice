#include <stdio.h>

// 節點結構定義
struct ListNode {
    int val;
    struct ListNode *next;
};

// ============================================================================
// 單向鏈結串列原地反轉 (修正完成版)
// ============================================================================
struct ListNode* reverseList(struct ListNode* head)
{
    struct ListNode *curr = head;
    struct ListNode *pre = NULL;
    
    while (curr != NULL)
    {
        struct ListNode *next_node = curr->next; // 修正 1：必須用 curr->next 記住「當前節點」的下一個位址
        curr->next = pre;                        // 將當前節點的指針轉向，指回前一個節點 pre
        pre = curr;                              // pre 往前移動
        curr = next_node;                        // curr 往前移動
    }
    
    return pre; // 修正 2：回傳反轉後的全新頭節點 pre
}

/*
// ⚠️ 原本程式碼寫法對照與微調說明：
struct ListNode* reverseList_original(struct ListNode* head)
{
    struct ListNode *curr = head;
    struct ListNode *pre = NULL;
    
    while(curr != NULL)
    {
        struct ListNode *next_node = head->next; 
        // ⚠️ 小問題 1：這裡如果寫 head->next，因為 head 永遠停在第 1 個節點，
        // 當 curr 走到第 2、3 個節點時，next_node 會抓錯變成永遠抓第 2 個節點！
        // 應改為：curr->next

        curr->next = pre;
        pre = curr;
        curr = next_node;
    }
    // ⚠️ 小問題 2：最後忘記寫 return pre; 回傳翻轉後的全新頭節點。
}
*/