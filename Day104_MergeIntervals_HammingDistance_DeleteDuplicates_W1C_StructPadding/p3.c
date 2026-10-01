#include <stdio.h>
#include <stdlib.h>

/**
 * ============================================================================
 * 【Day 104 - 題目三】刪除排序鏈結串列中的重複節點 (LeetCode #83 - Remove Duplicates from Sorted List)
 * 【考試頻率】群聯 (Phison) / 電子五哥 (廣達、緯創、和碩) 經典指標操作題
 * 【LeetCode 難度】Easy (初階精準題)
 * ============================================================================
 * 【題目說明】
 * 給定一個已排序的單向鏈結串列的頭節點 head，請刪除所有重複的元素，使每個元素只出現一次。
 * 回傳同樣已排序的鏈結串列。
 * 
 * ⚠️ 韌體實務注意：
 * 在 C 語言韌體開發中，跳過重複節點時，請記得使用 free() 釋放被移除的節點記憶體，避免 Memory Leak。
 * 
 * 【範例 1】
 * 輸入: head = [1,1,2]
 * 輸出: [1,2]
 * 
 * 【範例 2】
 * 輸入: head = [1,1,2,3,3]
 * 輸出: [1,2,3]
 * 
 * 【限制條件】
 * 1. 鏈結串列中的節點數介於 [0, 300] 之間
 * 2. -100 <= Node.val <= 100
 * 3. 題目保證鏈結串列已依升序排序
 * ============================================================================
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* deleteDuplicates(struct ListNode* head) {
    if (head == NULL) {
        return NULL;
    }

    struct ListNode* curr = head;

    // 關鍵迴圈條件：必須確保 curr 和 curr->next 都不為空，避免 NULL 指標解參考
    while (curr != NULL && curr->next != NULL) {
        if (curr->val == curr->next->val) {
            // 發現重複節點：記錄重複者、跳過節點、釋放記憶體
            struct ListNode* dup = curr->next;
            curr->next = curr->next->next;
            free(dup);
            // ⚠️ 注意：此時 curr 不能前進！因為下一個新接上來的節點可能依然重複 (例如 1->1->1)
        } else {
            // 沒有重複時，curr 才往下走一個節點
            curr = curr->next;
        }
    }

    return head;
}

/*
===============================================================================
【原始程式碼盲點與改進分析】
-------------------------------------------------------------------------------
1. 空指標解參考引發 Segmentation Fault（死機盲點）：
   - 原寫法迴圈條件為：`while(curr != NULL)`
   - 當遍歷到鏈結串列的最後一個節點時，`curr->next` 是 NULL。
   - 此時執行 `struct ListNode* next_node = curr->next;`
     緊接著執行 `if(next_node->val == curr->val)`，直接對 NULL 指標解參考！
     硬體 MMU 立即捕捉到非法的記憶體存取，程式噴出 Segmentation Fault 當場崩潰。
   - 正確作法：迴圈條件必須嚴格檢查兩者：`while(curr != NULL && curr->next != NULL)`。

2. 連續重複節點無法徹底清除（指標過早推進）：
   - 原寫法在迴圈最後無條件執行了 `curr = curr->next;`。
   - 若遇到 3 個以上連續重複節點（例如 1 -> 1 -> 1 -> 2）：
     第一步：刪掉第 2 個 1，串列變成 1 -> 1 -> 2。
     但因為原程式碼緊接著執行了 `curr = curr->next;`，`curr` 直接跳到後面的 1！
     導致後續比對變成 1 與 2，最終結果仍殘留 1 -> 1，沒有徹底刪乾淨。
   - 正確作法：只有在「數值不相同（else 分支）」時，`curr` 才往前走；
     若數值相同，刪除重複節點後 `curr` 必須停在原地，繼續檢查新接上來的節點是否也是重複值。

-------------------------------------------------------------------------------
【原始程式碼存檔】
struct ListNode* deleteDuplicates(struct ListNode* head) {
    // 請在此處撰寫你的程式碼
    if(head == NULL || head->next == NULL)
    {
        return head;
    }
    struct ListNode* curr = head;
    while(curr != NULL)
    {
        struct ListNode* next_node = curr->next;
        if(next_node->val == curr->val)
        {
            curr->next = next_node->next;
            free(next_node);
        }
        curr = curr->next;
    }
    return head;
}
===============================================================================
*/
