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
// 題目 2【二元搜尋樹中第 K 小的元素 (Kth Smallest Element in a BST - LeetCode #230)】
// 難度：🟡 Medium (中等題) | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：BST 中序走訪 (In-Order Traversal) - $O(H + k)$ 時間, $O(H)$ 空間
// ============================================================================

void inorder(struct TreeNode* root, int k, int* count, int* ans)
{
    // 防呆或已找到答案提前剪枝
    if (root == NULL || *count >= k)
    {
        return;
    }

    // 1. 先走訪左子樹 (較小的值)
    inorder(root->left, k, count, ans);

    // 2. 走訪當前節點：報數加 1
    (*count)++;
    if (*count == k)
    {
        *ans = root->val; // 命中第 k 小元素！
        return;
    }

    // 3. 再走訪右子樹 (較大的值)
    inorder(root->right, k, count, ans);
}

int kthSmallest(struct TreeNode* root, int k)
{
    int count = 0;
    int ans = 0;
    inorder(root, k, &count, &ans);
    return ans;
}

// 輔助函式：建立節點
struct TreeNode* createTreeNode(int val) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 輔助函式：釋放樹記憶體
void freeTree(struct TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main()
{
    // 測試 1: [3, 1, 4, null, 2], k = 1 -> 1
    //       3
    //      / \
    //     1   4
    //      \
    //       2
    struct TreeNode* r1 = createTreeNode(3);
    r1->left = createTreeNode(1);
    r1->right = createTreeNode(4);
    r1->left->right = createTreeNode(2);

    int ans1 = kthSmallest(r1, 1);
    printf("測試 1 通過: 第 1 小的元素是: %d (預期: 1)\n", ans1);
    assert(ans1 == 1);

    int ans1_k2 = kthSmallest(r1, 2);
    printf("測試 1 補充: 第 2 小的元素是: %d (預期: 2)\n", ans1_k2);
    assert(ans1_k2 == 2);
    freeTree(r1);

    // 測試 2: [5, 3, 6, 2, 4, null, null, 1], k = 3 -> 3
    //         5
    //        / \
    //       3   6
    //      / \
    //     2   4
    //    /
    //   1
    struct TreeNode* r2 = createTreeNode(5);
    r2->left = createTreeNode(3);
    r2->right = createTreeNode(6);
    r2->left->left = createTreeNode(2);
    r2->left->right = createTreeNode(4);
    r2->left->left->left = createTreeNode(1);

    int ans2 = kthSmallest(r2, 3);
    printf("測試 2 通過: 第 3 小的元素是: %d (預期: 3)\n", ans2);
    assert(ans2 == 3);
    freeTree(r2);

    printf("\n🎉 p2.c 二元搜尋樹中第 K 小的元素所有測試案例全數 100%% 通過！\n");
    return 0;
}
