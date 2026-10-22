#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #2 - Add Two Numbers
 * 
 * 題目描述：
 * 給定兩個非空的單向鏈結串列，表示兩個非負整數。
 * 每個數字的各位數位以「逆序」方式存儲，且每個節點僅包含一個數字（0 ~ 9）。
 * 請將這兩個數相加，並將結果以相同形式的鏈結串列回傳。
 * 
 * 你可以假設除了數字 0 本身之外，這兩個數都不會以 0 開頭。
 * 
 * 範例 1：
 * 輸入：l1 = [2, 4, 3], l2 = [5, 6, 4]
 * 輸出：[7, 0, 8]
 * 解釋：342 + 465 = 807。
 * 
 * 範例 2：
 * 輸入：l1 = [0], l2 = [0]
 * 輸出：[0]
 * 
 * 範例 3：
 * 輸入：l1 = [9, 9, 9, 9, 9, 9, 9], l2 = [9, 9, 9, 9]
 * 輸出：[8, 9, 9, 9, 0, 0, 0, 1]
 * 
 * 限制條件：
 * 每個鏈結串列中的節點數在範圍 [1, 100] 內
 * 0 <= Node.val <= 9
 * 保證輸入的鏈結串列代表的數字不含前導零（除 0 本身外）
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
/*
 * =========================================================================
 * 【LeetCode 官方提交成績】：
 * - 狀態：Accepted (通過 1569/1569 測試案例)
 * - 執行時間：0 ms (擊敗 100.00% C 語言提交)
 * - 記憶體：13.4 MB
 * - Submission ID：2178691468
 * -------------------------------------------------------------------------
 * 【使用者原始白板程式碼】：
 * struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
 *     // 請於此處實作純白板解法
 *     int num1 = 0;
 *     int num2 = 0;
 *     struct ListNode* curr1 = l1;
 *     struct ListNode* curr2 = l2;
 *     while(curr1 != NULL)
 *     {
 *         num1 += curr1->val;
 *         num1 *= 10;
 *         curr1 = curr1->next;
 *     }
 *     while(curr2 != NULL)
 *     {
 *         num2 += curr2->val;
 *         num2 *= 10;
 *         curr2 = curr2->next;
 *     }
 *     int num_result = num1 + num2;
 *     struct ListNode* curr3 = (num1 > num2) ? l1 : l2;
 *     struct ListNode* new_head = curr3;
 *     while(curr3->next != NULL)
 *     {
 *         curr3->val = num_result % 10;
 *         num_result = num_result / 10;
 *         curr3 = curr3->next;
 *     }
 *     curr3->val = num_result % 10;
 *     num_result = num_result / 10;
 *     if(num_result != 0)
 *     {
 *         struct ListNode* tail = (struct ListNode*) malloc(1 * sizeof(struct ListNode));
 *         tail->val = num_result;
 *         tail->next = NULL;
 *         curr3->next = tail;
 *     }
 *     struct ListNode* pre = NULL;
 *     struct ListNode* curr = new_head;
 *     while(curr != NULL)
 *     {
 *         struct ListNode* next_node = curr->next;
 *         curr->next = pre;
 *         pre = curr;
 *         curr = next_node;
 *     }
 *     return pre;
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點剖析】：
 * 1. 大數溢位（Integer Overflow）：
 *    - 題目限制節點數高達 100 位數！32 位元 int 最大僅約 2*10^9（約 9~10 位），
 *      直接試圖將鏈結串列轉為 int num1 與 num2 會引發嚴重的整數溢位（Overflow）。
 * 2. 題目儲存順序與計算方向：
 *    - 題目開頭就說明數字是「逆序存儲」（即 head 就是個位數 LSB）。
 *      這表示我們根本不需要將它轉為整數或大費周章翻轉，可以直接從頭節點像昨天 Day122 P2 (Add Strings)
 *      一樣「逐位相加、維護 carry 進位」！
 * 3. 虛擬頭節點（Dummy Node）與逐位前進：
 *    - 透過 dummy 節點簡化表頭建立，當 l1、l2 或 carry 任一非零時持續前進，
 *      一步到位且能優雅處理最高位進位（如 99 + 1 = 100）。
 * =========================================================================
 */

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode dummy;
    dummy.next = NULL;
    struct ListNode* curr = &dummy;
    int carry = 0;

    // 當 l1 或 l2 還有節點，或者還有進位未處理時持續迴圈
    while (l1 != NULL || l2 != NULL || carry != 0) {
        int sum = carry;

        if (l1 != NULL) {
            sum += l1->val;
            l1 = l1->next;
        }

        if (l2 != NULL) {
            sum += l2->val;
            l2 = l2->next;
        }

        carry = sum / 10; // 計算進位

        // 配置新節點存放當前個位數值 (sum % 10)
        struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
        node->val = sum % 10;
        node->next = NULL;

        curr->next = node;
        curr = curr->next;
    }

    return dummy.next;
}

// 輔助測試函式
struct ListNode* createList(int* arr, int size) {
    if (size == 0) return NULL;
    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->val = arr[0];
    head->next = NULL;
    struct ListNode* curr = head;
    for (int i = 1; i < size; i++) {
        curr->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        curr->next->val = arr[i];
        curr->next->next = NULL;
        curr = curr->next;
    }
    return head;
}

void printList(struct ListNode* head) {
    struct ListNode* curr = head;
    while (curr) {
        printf("%d", curr->val);
        if (curr->next) printf(" -> ");
        curr = curr->next;
    }
    printf("\n");
}

int main() {
    int a1[] = {2, 4, 3};
    int a2[] = {5, 6, 4};
    struct ListNode* l1 = createList(a1, 3);
    struct ListNode* l2 = createList(a2, 3);
    printf("l1: "); printList(l1);
    printf("l2: "); printList(l2);
    struct ListNode* sum = addTwoNumbers(l1, l2);
    printf("Result: "); printList(sum);
    printf("Expected: 7 -> 0 -> 8\n");

    return 0;
}
