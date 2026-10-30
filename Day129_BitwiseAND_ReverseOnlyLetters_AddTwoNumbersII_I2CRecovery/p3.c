#include <stdio.h>
#include <stdlib.h>

/*
 * LeetCode #445 - Add Two Numbers II
 *
 * Description:
 * You are given two non-empty linked lists representing two non-negative integers.
 * The most significant digit comes first and each of their nodes contains a single digit.
 * Add the two numbers and return the sum as a linked list.
 *
 * You may assume the two numbers do not contain any leading zero, except the number 0 itself.
 *
 * Constraints:
 * The number of nodes in each linked list is in the range [1, 100].
 * 0 <= Node.val <= 9
 * It is guaranteed that the list represents a number that does not have leading zeros.
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

static struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode* prev = NULL;
    struct ListNode* curr = head;
    while (curr != NULL) {
        struct ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    // 步驟 1：將兩鏈結串列反轉，讓個位數（低位）移至頭部
    l1 = reverseList(l1);
    l2 = reverseList(l2);

    int carry = 0;
    struct ListNode* res_head = NULL;

    // 步驟 2：從個位數開始依序相加並處理進位
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

        carry = sum / 10;

        // 步驟 3：使用「頭插法（Head Insertion）」建立新節點
        // 如此一來先算出的低位會自然被推向尾端，最終結果自然為高位在前
        struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
        node->val = sum % 10;
        node->next = res_head;
        res_head = node;
    }

    return res_head;
}

static struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

static struct ListNode* createList(int arr[], int size) {
    if (size == 0) return NULL;
    struct ListNode* head = createNode(arr[0]);
    struct ListNode* curr = head;
    for (int i = 1; i < size; i++) {
        curr->next = createNode(arr[i]);
        curr = curr->next;
    }
    return head;
}

static void printList(struct ListNode* head) {
    printf("[");
    while (head != NULL) {
        printf("%d", head->val);
        if (head->next != NULL) printf(" -> ");
        head = head->next;
    }
    printf("]\n");
}

int main(void) {
    // Test Case 1: [7,2,4,3] + [5,6,4] -> [7,8,0,7] (7243 + 564 = 7807)
    int arr1[] = {7, 2, 4, 3};
    int arr2[] = {5, 6, 4};
    struct ListNode* l1 = createList(arr1, 4);
    struct ListNode* l2 = createList(arr2, 3);
    printf("Test 1 Result: ");
    struct ListNode* res1 = addTwoNumbers(l1, l2);
    printList(res1);
    printf("Expected: [7 -> 8 -> 0 -> 7]\n\n");

    // Test Case 2: [2,4,3] + [5,6,4] -> [8,0,7] (243 + 564 = 807)
    int arr3[] = {2, 4, 3};
    int arr4[] = {5, 6, 4};
    struct ListNode* l3 = createList(arr3, 3);
    struct ListNode* l4 = createList(arr4, 3);
    printf("Test 2 Result: ");
    struct ListNode* res2 = addTwoNumbers(l3, l4);
    printList(res2);
    printf("Expected: [8 -> 0 -> 7]\n\n");

    // Test Case 3: [0] + [0] -> [0]
    int arr5[] = {0};
    int arr6[] = {0};
    struct ListNode* l5 = createList(arr5, 1);
    struct ListNode* l6 = createList(arr6, 1);
    printf("Test 3 Result: ");
    struct ListNode* res3 = addTwoNumbers(l5, l6);
    printList(res3);
    printf("Expected: [0]\n\n");

    // Test Case 4: [9,9,9] + [1] -> [1,0,0,0]
    int arr7[] = {9, 9, 9};
    int arr8[] = {1};
    struct ListNode* l7 = createList(arr7, 3);
    struct ListNode* l8 = createList(arr8, 1);
    printf("Test 4 Result: ");
    struct ListNode* res4 = addTwoNumbers(l7, l8);
    printList(res4);
    printf("Expected: [1 -> 0 -> 0 -> 0]\n");

    return 0;
}
