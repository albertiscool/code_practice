#include <stdio.h>
#include <stdlib.h>

// 節點結構體
struct TreeNode {
    int val;                  // 節點存的數值
    struct TreeNode *left;    // 指向左小孩
    struct TreeNode *right;   // 指向右小孩
};

// ============================================================================
// 標準滿分解答：二元樹翻轉 / 鏡像 (Invert Binary Tree)
// ============================================================================
struct TreeNode* invertTree(struct TreeNode* root)
{
    // 1. 終止條件：如果節點為空，回傳 NULL
    if (root == NULL)
    {
        return NULL;
    }

    // 2. 原地交換當前節點的左右小孩
    struct TreeNode *temp = root->left;
    root->left = root->right;
    root->right = temp;

    // 3. 遞迴呼叫左右小孩，讓底下的小孩也各自完成翻轉
    invertTree(root->left);
    invertTree(root->right);

    // 4. 回傳翻轉後的根節點
    return root;
}

// 輔助函式：建立新節點
struct TreeNode* createNode(int val) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 輔助函式：中序遍歷印出二元樹
void printInOrder(struct TreeNode* root) {
    if (root == NULL) return;
    printInOrder(root->left);
    printf("%d ", root->val);
    printInOrder(root->right);
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    /* 建立測試二元樹：
           4
         /   \
        2     7
       / \   / \
      1   3 6   9
    */
    struct TreeNode *root = createNode(4);
    root->left = createNode(2);
    root->right = createNode(7);
    root->left->left = createNode(1);
    root->left->right = createNode(3);
    root->right->left = createNode(6);
    root->right->right = createNode(9);

    printf("翻轉前中序遍歷: ");
    printInOrder(root); // 預期: 1 2 3 4 6 7 9
    printf("\n");

    invertTree(root);

    printf("翻轉後中序遍歷: ");
    printInOrder(root); // 預期: 9 7 6 4 3 2 1
    printf("\n");

    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（演算法交換邏輯 100% 正確！）：
struct TreeNode* invertTree_original(struct TreeNode* root)
{
    if(root == NULL)
    {
        return NULL; // 💡 盲點 1：回傳型態是 struct TreeNode*，要寫 return NULL;（不可只寫 return;）
    }

    // 💡 交換邏輯完美！
    struct TreeNode *temp = root->left;
    root->left = root->right;
    root->right = temp;

    // 💡 盲點 2：呼叫函式時不需要在前面加形態宣告 struct TreeNode*，直接寫函式名即可！
    // 💡 盲點 3：最後要記得寫 return root; 把根節點傳出去！
    invertTree(root->left);
    invertTree(root->right);

    return root;
}
*/