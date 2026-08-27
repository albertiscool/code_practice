#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

// 二元樹節點定義
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// ============================================================================
// 題目 3【驗證二元搜尋樹 (Validate Binary Search Tree - LeetCode #98)】
// 難度：🟡 Medium (中等題) | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：區間界限遞迴法 (Range Boundary Recursion) - $O(N)$ 時間, $O(H)$ 空間
// ============================================================================

// 輔助遞迴函式：驗證 node 節點數值是否落在嚴格的開區間 (min_val, max_val) 之間
bool validate(struct TreeNode* node, long long min_val, long long max_val)
{
    // 空節點本身符合 BST 定義
    if (node == NULL) return true;

    // 若當前節點違反開區間範圍，直接判定為非法 BST
    if (node->val <= min_val || node->val >= max_val)
    {
        return false;
    }

    // 遞迴驗證左子樹與右子樹：
    // 1. 走訪左子樹時，數值上限更新為當前節點值 node->val
    // 2. 走訪右子樹時，數值下限更新為當前節點值 node->val
    return validate(node->left, min_val, node->val) &&
           validate(node->right, node->val, max_val);
}

bool isValidBST(struct TreeNode* root)
{
    // 使用 64-bit 最小/最大值 LLONG_MIN 與 LLONG_MAX 避開 32-bit INT 邊界溢位問題
    return validate(root, LLONG_MIN, LLONG_MAX);
}

// 輔助函式：建立節點
struct TreeNode* createNode(int val) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 輔助函式：釋放樹
void freeTree(struct TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main()
{
    // 測試 1: [2, 1, 3] -> true
    //      2
    //     / \
    //    1   3
    struct TreeNode* t1 = createNode(2);
    t1->left = createNode(1);
    t1->right = createNode(3);
    assert(isValidBST(t1) == true);
    printf("測試 1 通過: [2, 1, 3] 是合法 BST (true)\n");
    freeTree(t1);

    // 測試 2: [5, 1, 4, null, null, 3, 6] -> false
    //        5
    //       / \
    //      1   4
    //         / \
    //        3   6
    struct TreeNode* t2 = createNode(5);
    t2->left = createNode(1);
    t2->right = createNode(4);
    t2->right->left = createNode(3);
    t2->right->right = createNode(6);
    assert(isValidBST(t2) == false);
    printf("測試 2 通過: [5, 1, 4, null, null, 3, 6] 不是合法 BST (false)\n");
    freeTree(t2);

    printf("\n🎉 p3.c 驗證二元搜尋樹所有測試案例全數 100%% 通過！\n");
    return 0;
}
