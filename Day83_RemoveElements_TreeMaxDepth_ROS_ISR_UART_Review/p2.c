#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 二元樹節點定義
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// ============================================================================
// 題目 2【二元樹的最大深度 (Maximum Depth of Binary Tree - LeetCode #104)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (樹狀結構入門必考神題)
// 演算法：DFS 後序遍歷遞迴 (Divide and Conquer) - $O(N)$ 時間, $O(H)$ 空間
// ============================================================================

int maxDepth(struct TreeNode* root)
{
    // 1. 終止條件：空節點深度為 0
    if (root == NULL)
    {
        return 0;
    }

    // 2. 先分別計算左子樹與右子樹的深度 (存成變數避免重複遞迴導致 O(2^N))
    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);

    // 3. 當前節點的深度 = 左右子樹較大者 + 1 (算入自己這一層！)
    return 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
}

// 輔助函式：建立樹節點
struct TreeNode* createTreeNode(int val) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 輔助函式：釋放二元樹
void freeTree(struct TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main()
{
    // 測試 1:
    //        3
    //       / \
    //      9  20
    //        /  \
    //       15   7
    // 最大深度為 3
    struct TreeNode* root1 = createTreeNode(3);
    root1->left = createTreeNode(9);
    root1->right = createTreeNode(20);
    root1->right->left = createTreeNode(15);
    root1->right->right = createTreeNode(7);

    int d1 = maxDepth(root1);
    printf("測試 1 通過: 樹深度為: %d (預期: 3)\n", d1);
    assert(d1 == 3);
    freeTree(root1);

    // 測試 2: [1, null, 2]
    struct TreeNode* root2 = createTreeNode(1);
    root2->right = createTreeNode(2);
    int d2 = maxDepth(root2);
    printf("測試 2 通過: 樹深度為: %d (預期: 2)\n", d2);
    assert(d2 == 2);
    freeTree(root2);

    // 測試 3: 空樹 NULL ➔ 深度為 0
    int d3 = maxDepth(NULL);
    printf("測試 3 通過: 空樹深度為: %d (預期: 0)\n", d3);
    assert(d3 == 0);

    // 測試 4: 單節點 [42] ➔ 深度為 1
    struct TreeNode* root4 = createTreeNode(42);
    int d4 = maxDepth(root4);
    printf("測試 4 通過: 單節點樹深度為: %d (預期: 1)\n", d4);
    assert(d4 == 1);
    freeTree(root4);

    printf("\n🎉 p2.c 二元樹最大深度所有測試案例全數 100%% 通過！\n");
    return 0;
}
