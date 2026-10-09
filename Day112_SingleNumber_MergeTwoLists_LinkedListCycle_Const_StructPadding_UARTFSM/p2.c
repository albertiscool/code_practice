#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * LeetCode #21: Merge Two Sorted Lists
 * 
 * 題目描述：
 * 將兩個升序鏈結串列 list1 與 list2 合併為一個新的升序鏈結串列並回傳。
 * 新鏈結串列應該是透過拼接給定的兩個串列的所有節點組成的（原地重接指針，不需額外 malloc 節點）。
 * 
 * 限制條件：
 * - 兩個串列的節點數目範圍在 [0, 50]
 * - -100 <= Node.val <= 100
 * - list1 與 list2 均按升序（遞增）順序排列
 * 
 * 範例 1:
 *   輸入: list1 = [1,2,4], list2 = [1,3,4]
 *   輸出: [1,1,2,3,4,4]
 * 
 * 範例 2:
 *   輸入: list1 = [], list2 = []
 *   輸出: []
 * 
 * 範例 3:
 *   輸入: list1 = [], list2 = [0]
 *   輸出: [0]
 * 
 * 面試深意：
 * 電子五哥（廣達、緯創、和碩）與 IC 設計廠研替白板常客，考驗單向鏈結串列指標手術與邊界處理。
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {
    // 請在此處實現純白板程式碼
    if (list1 == NULL) return list2;
    if (list2 == NULL) return list1;

    struct ListNode dummy;
    dummy.next = NULL;
    struct ListNode* tail = &dummy;

    while (list1 != NULL && list2 != NULL) {
        if (list1->val <= list2->val) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }

    tail->next = (list1 != NULL) ? list1 : list2;
    return dummy.next;
}

// 輔助函式：建立與列印鏈結串列
struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

void printList(struct ListNode* head) {
    while (head) {
        printf("%d%s", head->val, head->next ? "->" : "");
        head = head->next;
    }
    printf("\n");
}

int main(void) {
    printf("=== Test P2: Merge Two Sorted Lists ===\n");
    // Test 1: [1,2,4] + [1,3,4] -> [1,1,2,3,4,4]
    struct ListNode* l1_1 = createNode(1);
    struct ListNode* l1_2 = createNode(2);
    struct ListNode* l1_4 = createNode(4);
    l1_1->next = l1_2; l1_2->next = l1_4;

    struct ListNode* l2_1 = createNode(1);
    struct ListNode* l2_3 = createNode(3);
    struct ListNode* l2_4 = createNode(4);
    l2_1->next = l2_3; l2_3->next = l2_4;

    struct ListNode* res1 = mergeTwoLists(l1_1, l2_1);
    printf("Result 1: ");
    printList(res1);

    int expected1[] = {1, 1, 2, 3, 4, 4};
    struct ListNode* p = res1;
    for (int i = 0; i < 6; i++) {
        assert(p != NULL && p->val == expected1[i]);
        p = p->next;
    }
    assert(p == NULL);
    printf("Test 1 Passed!\n");

    // Test 2: [] + [] -> []
    assert(mergeTwoLists(NULL, NULL) == NULL);
    printf("Test 2 Passed: empty lists\n");

    // Test 3: [] + [0] -> [0]
    struct ListNode* l3_0 = createNode(0);
    struct ListNode* res3 = mergeTwoLists(NULL, l3_0);
    assert(res3 != NULL && res3->val == 0 && res3->next == NULL);
    printf("Test 3 Passed: one empty list\n");

    printf("All tests passed!\n");
    return 0;
}

/* =========================================================================
 * 🚩 【Albert 白板實戰複盤與有序鏈結串列合併深度解析】
 * =========================================================================
 * 
 * 🏆 LeetCode 提交結果：
 * - 狀態：Accepted (208/208 測試案例全數通過！)
 * - 執行時間：0 ms (Beats 100.00%)
 * - 記憶體：11.6 MB
 * - Submission ID: 2167263006
 * 
 * 原始撰寫程式碼片段：
 * -------------------------------------------------------------------------
 * struct ListNode dummy;
 * struct ListNode* tail = dummy.next;           // ⚠️ 盲點 1：未初始化野指標，指向隨機垃圾地址
 * 
 * while(list1 != NULL && list2 != NULL) {
 *     if(list1->val <= list2->val) {
 *         tail->next = list1;                   // ⚠️ SEGV 當機！且 list1 未往前推進 (死迴圈)
 *     } else {
 *         tail->next = list2;                   // ⚠️ list2 未往前推進 (死迴圈)
 *     }
 *     tail = tail->next;
 * }
 * return dummy.next;                            // ⚠️ 盲點 3：未拼接殘留未走完的串列
 * -------------------------------------------------------------------------
 * 
 * 💡 邏輯亮點：
 * 1. 成功想出「虛擬節點 Dummy Node」避免手動特判誰是第一個 head 的進階思路！
 * 2. 數值大小比對 `list1->val <= list2->val` 與尾端串接 `tail->next` 概念完全正確。
 * 
 * ❌ 盲點 1：區域變數未初始化與野指標（AddressSanitizer: SEGV Crash）
 * -------------------------------------------------------------------------
 * - `struct ListNode dummy;` 是 Stack 上的區域變數，其成員未初始化時為隨機記憶體垃圾值。
 * - 寫 `struct ListNode* tail = dummy.next;` 會讓 `tail` 變成野指標（Dangling Pointer）。
 * - 隨後執行 `tail->next = list1;` 企圖解引用野指標位址，立即引發非法記憶體訪問當機（SIGSEGV）！
 * - 修正：`dummy` 的地址是實體的，`dummy.next` 應初始化為 NULL，且 `tail` 應指向 `&dummy`！
 *   ```c
 *   struct ListNode dummy;
 *   dummy.next = NULL;
 *   struct ListNode* tail = &dummy;
 *   ```
 * 
 * ❌ 盲點 2：忘記推進輸入鏈結指針（無窮死迴圈 Infinite Loop）
 * -------------------------------------------------------------------------
 * - 當我們把 `list1` 掛到 `tail->next` 後，`list1` 必須前進到下一個節點 `list1 = list1->next;`！
 * - 若忘記推進，`list1` 與 `list2` 永遠保持非空，`while` 迴圈將無限次原地旋轉死鎖。
 * 
 * ❌ 盲點 3：殘留尾端未拼接（丟失後續長鏈節點）
 * -------------------------------------------------------------------------
 * - 當 `list1` 或 `list2` 其中一個走到底（變成 NULL）時，迴圈終止。
 * - 但另一個串列可能還有剩餘節點（例如長度不對稱）。
 * - 單向鏈結串列天生具備「有序性」，剩下的一整串可以直接一筆劃掛上去：
 *   ```c
 *   tail->next = (list1 != NULL) ? list1 : list2;
 *   ```
 * ========================================================================= */
