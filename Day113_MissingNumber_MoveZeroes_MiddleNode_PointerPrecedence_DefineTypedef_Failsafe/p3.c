#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * LeetCode #876: Middle of the Linked List
 * 
 * 題目描述：
 * 給定一個單向鏈結串列的頭節點 head，請回傳鏈結串列的中間節點。
 * 如果有兩個中間節點（即節點個數為偶數），則回傳「第二個中間節點」。
 * 
 * 限制條件：
 * - 串列中節點的數目在範圍 [1, 100] 內
 * - 1 <= Node.val <= 100
 * 
 * 範例 1:
 *   輸入: head = [1,2,3,4,5]
 *   輸出: 節點 3 (值為 3 的節點，其後序列為 [3,4,5])
 * 
 * 範例 2:
 *   輸入: head = [1,2,3,4,5,6]
 *   輸出: 節點 4 (值為 4 的節點，其後序列為 [4,5,6])
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* middleNode(struct ListNode* head) {
    // 請在此處實現純白板程式碼
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    while(fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
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
    printf("=== Test P3: Middle of the Linked List ===\n");
    // Test 1: [1,2,3,4,5] (奇數個) -> 3
    struct ListNode* a1 = createNode(1);
    struct ListNode* a2 = createNode(2);
    struct ListNode* a3 = createNode(3);
    struct ListNode* a4 = createNode(4);
    struct ListNode* a5 = createNode(5);
    a1->next = a2; a2->next = a3; a3->next = a4; a4->next = a5;

    struct ListNode* res1 = middleNode(a1);
    assert(res1 != NULL && res1->val == 3);
    printf("Test 1 Passed: [1,2,3,4,5] -> middle node is 3\n");

    // Test 2: [1,2,3,4,5,6] (偶數個) -> 4
    struct ListNode* b1 = createNode(1);
    struct ListNode* b2 = createNode(2);
    struct ListNode* b3 = createNode(3);
    struct ListNode* b4 = createNode(4);
    struct ListNode* b5 = createNode(5);
    struct ListNode* b6 = createNode(6);
    b1->next = b2; b2->next = b3; b3->next = b4; b4->next = b5; b5->next = b6;

    struct ListNode* res2 = middleNode(b1);
    assert(res2 != NULL && res2->val == 4);
    printf("Test 2 Passed: [1,2,3,4,5,6] -> middle node is 4 (second middle)\n");

    // Test 3: [1] (單節點) -> 1
    struct ListNode* c1 = createNode(1);
    struct ListNode* res3 = middleNode(c1);
    assert(res3 != NULL && res3->val == 1);
    printf("Test 3 Passed: [1] -> middle node is 1\n");

    // Test 4: [1, 2] (雙節點) -> 2
    struct ListNode* d1 = createNode(1);
    struct ListNode* d2 = createNode(2);
    d1->next = d2;
    struct ListNode* res4 = middleNode(d1);
    assert(res4 != NULL && res4->val == 2);
    printf("Test 4 Passed: [1, 2] -> middle node is 2\n");

    printf("All tests passed!\n");
    return 0;
}

/* =========================================================================
 * 🚩 【Albert 白板實戰複盤與鏈結串列快慢指針深度解析】
 * =========================================================================
 * 
 * 🏆 LeetCode 提交結果：
 * - 狀態：Accepted (36/36 測試案例全數通過！)
 * - 執行時間：0 ms (Beats 100.00%)
 * - 記憶體：9.2 MB
 * - Submission ID: 2169094225
 * 
 * 💡 邏輯亮點（滿分白板表現）：
 * 1. 零提示下純白板秒殺！經典快慢指針（Fast & Slow Pointers）一次遍歷（One Pass）！
 * 2. 邊界條件防禦嚴密：`while(fast != NULL && fast->next != NULL)` 完美相容奇數長度、偶數長度與單節點。
 * 3. 時間複雜度 O(N)，額外空間 O(1)。
 * 
 * 🔬 奇數 vs 偶數長度數學歸納演變：
 * -------------------------------------------------------------------------
 * 1. 奇數長度（如 5 個節點：1 -> 2 -> 3 -> 4 -> 5）：
 *    - 初始化：slow = 1, fast = 1
 *    - 第 1 步：slow = 2, fast = 3
 *    - 第 2 步：slow = 3, fast = 5 (此時 fast->next == NULL，迴圈停止！)
 *    - 回傳 slow = 3（精準命中唯一中間節點）。
 * 
 * 2. 偶數長度（如 6 個節點：1 -> 2 -> 3 -> 4 -> 5 -> 6）：
 *    - 初始化：slow = 1, fast = 1
 *    - 第 1 步：slow = 2, fast = 3
 *    - 第 2 步：slow = 3, fast = 5
 *    - 第 3 步：slow = 4, fast = NULL (此時 fast == NULL，迴圈停止！)
 *    - 回傳 slow = 4（精準命中題意要求的「第二個中間節點」）。
 * 
 * 💡 面試官追問延伸（Reorder List / Merge Sort 必備前置技巧）：
 * 「如果偶數長度時，題目要求回傳【第一個中間節點】（例如長度 6 回傳節點 3），快慢指針起點該如何微調？」
 * - 答：只需將快指針初始化為 `fast = head->next`，即可讓 slow 停在第一個中間節點！
 * ========================================================================= */
