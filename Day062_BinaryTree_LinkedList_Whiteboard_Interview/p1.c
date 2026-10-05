#include <stdio.h>
#include <stdlib.h>

// 節點結構體
struct TreeNode {
    int val;                  // 節點存的數值
    struct TreeNode *left;    // 指向左小孩
    struct TreeNode *right;   // 指向右小孩
};

// ============================================================================
// 標準滿分解答：二元樹的最大深度 (Maximum Depth of Binary Tree)
// ============================================================================
int maxDepth(struct TreeNode *root)
{
    // 1. 終止條件：如果節點為空，深度為 0
    if (root == NULL)
    {
        return 0;
    }

    // 2. 遞迴分別計算左子樹與右子樹的深度
    int left_depth = maxDepth(root->left);
    int right_depth = maxDepth(root->right);

    // 3. 當前節點的深度 = 左右子樹較大者 + 1 (自己這一層)
    return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

// 輔助函式：建立新節點
struct TreeNode* createNode(int val) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    /* 建立測試二元樹：
           3
          / \
         9   20
            /  \
           15   7
    */
    struct TreeNode *root = createNode(3);
    root->left = createNode(9);
    root->right = createNode(20);
    root->right->left = createNode(15);
    root->right->right = createNode(7);

    printf("二元樹最大深度為: %d (預期: 3)\n", maxDepth(root));

    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（你的遞迴公式 1+max(...) 核心思維 100% 正確！）：
int maxDepth_original(struct TreeNode *root)
{
    struct TreeNode *curr = root;
    int count = 0;
    if(root == NULL)
    {
        return 0;
    }

    // 💡 盲點 1：如果是葉子節點 (left=NULL, right=NULL)，其實不需要特別寫 if，
    // 因為底下的遞迴 left 會回傳 0、right 會回傳 0，1 + max(0, 0) 自動就會算出 1！
    if(curr->left == NULL && curr->right == NULL)
    {
        return count; // 這裡若 return count 會回傳 0（但葉子節點深度應該是 1）
    }
    else
    {
        // 💡 盲點 2：C 語言需寫 root->left 與 root->right（不可只寫 left）。
        // 💡 盲點 3：C 語言標準函式庫預設沒有 max 函式，可用三元運算子 (a > b ? a : b)。
        // 💡 盲點 4：最後記得 return count;
        count = 1 + max(maxDepth(left), maxDepth(right));
    }
    return count;
}
*/