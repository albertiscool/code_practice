#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Definition for a binary tree node.
 */
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// ============================================================================
// 題目 1【翻轉二元樹 (Invert Binary Tree - LeetCode #226)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥
// 題目說明：給定一棵二元樹的根節點 root，翻轉這棵二元樹（將所有節點的左右子樹對調），
//           並回傳翻轉後的根節點。
// 範例 1：
//    輸入：root = [4,2,7,1,3,6,9]
//    輸出：[4,7,2,9,6,3,1]
// 範例 2：
//    輸入：root = [2,1,3]
//    輸出：[2,3,1]
// 範例 3：
//    輸入：root = []
//    輸出：[]
// 限制條件：
//    - 樹中節點數目範圍在 [0, 100] 之間。
//    - -100 <= Node.val <= 100
// ============================================================================
struct TreeNode* invertTree(struct TreeNode* root) {
    if (root == NULL) {
        return NULL;
    }
    
    // 1. 交換目前節點的左右子樹指標
    struct TreeNode *temp = root->left;
    root->left = root->right;
    root->right = temp;

    // 2. 遞迴翻轉左右子樹
    invertTree(root->left);
    invertTree(root->right);

    return root;
}

/*
 * ============================================================================
 * 【原始錯誤實作紀錄與盲點分析】
 * 錯誤原因：
 * 1. 邊界條件錯誤：在遇到葉子節點 (root->left == NULL && root->right == NULL) 時，
 *    誤寫為 `return NULL;`。
 * 2. 後果：導致上層呼叫 `root->left = invertTree(root->left)` 接收到 NULL，
 *    整棵樹的所有葉子節點會被全部切斷丟失！若整棵樹只有一個節點也會直接回傳 NULL。
 * ============================================================================
struct TreeNode* invertTree_wrong(struct TreeNode* root) {
    if(root == NULL)
    {
        return NULL;
    }
    
    if(root->left == NULL && root->right == NULL)
    {
        return NULL; // ❌ 盲點：葉子節點不該 return NULL，若要提早返回應是 return root;
    }
    else
    {
        struct TreeNode *temp = root->left;
        root->left = root->right;
        root->right = temp;
    }

    root->left = invertTree(root->left);
    root->right = invertTree(root->right);

    return root;
}
*/

/* 輔助函式：建立新節點 */
struct TreeNode* createNode(int val) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->val = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/* 輔助函式：印出前序走訪 (Preorder Traversal) 供驗證 */
void printPreorder(struct TreeNode* root) {
    if (root == NULL) {
        printf("null ");
        return;
    }
    printf("%d ", root->val);
    printPreorder(root->left);
    printPreorder(root->right);
}

/* 輔助函式：釋放樹的記憶體 */
void freeTree(struct TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void) {
    /* 測試測資：
     * 原樹：
     *       4
     *     /   \
     *    2     7
     *   / \   / \
     *  1   3 6   9
     */
    struct TreeNode* root = createNode(4);
    root->left = createNode(2);
    root->right = createNode(7);
    root->left->left = createNode(1);
    root->left->right = createNode(3);
    root->right->left = createNode(6);
    root->right->right = createNode(9);

    printf("原始樹 (前序走訪): ");
    printPreorder(root);
    printf("\n");

    struct TreeNode* invertedRoot = invertTree(root);

    printf("翻轉後 (前序走訪): ");
    printPreorder(invertedRoot);
    printf("\n");
    printf("預期翻轉後前序應為: 4 7 9 6 2 3 1 \n");

    freeTree(invertedRoot);
    return 0;
}
