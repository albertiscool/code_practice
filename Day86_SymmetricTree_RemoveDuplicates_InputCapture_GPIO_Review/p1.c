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
// 題目 1【對稱二元樹 (Symmetric Tree - LeetCode #101)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠二元樹高頻面試題)
// 題目說明：給定一個二元樹的根節點 root，檢查它是否軸對稱（即關於其中心垂直軸鏡像對稱）。
// 範例 1：
//    輸入：root = [1,2,2,3,4,4,3]
//           1
//         /   \
//        2     2
//       / \   / \
//      3   4 4   3
//    輸出：true
// 範例 2：
//    輸入：root = [1,2,2,null,3,null,3]
//           1
//         /   \
//        2     2
//         \     \
//          3     3
//    輸出：false
// 限制條件：
//    - 樹中節點數目在範圍 [1, 1000] 內。
//    - -100 <= Node.val <= 100
// ============================================================================

bool checkMirror(struct TreeNode* t1, struct TreeNode* t2) {
    if(t1 == NULL && t2 == NULL)
    {
        return true;
    }
    else if(t1 == NULL && t2 != NULL)
    {
        return false;
    }
    else if (t1 != NULL && t2 == NULL)
    {
        return false;
    }
    else if(t1->val != t2->val)
    {
        return false;
    }
    else
    {
        return checkMirror(t1->left, t2->right) && checkMirror(t1->right, t2->left);
    }
}

bool isSymmetric(struct TreeNode* root) {
    // TODO: 請在純白板模式下完成實作 (請勿尋求提示)
    return checkMirror(root->left,root->right);
}

/* 輔助函式：建立新節點 */
struct TreeNode* createNode(int val) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->val = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/* 輔助函式：釋放樹的記憶體 */
void freeTree(struct TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void) {
    /* 測試測資 1: 對稱樹 [1,2,2,3,4,4,3] */
    struct TreeNode* root1 = createNode(1);
    root1->left = createNode(2);
    root1->right = createNode(2);
    root1->left->left = createNode(3);
    root1->left->right = createNode(4);
    root1->right->left = createNode(4);
    root1->right->right = createNode(3);

    bool ans1 = isSymmetric(root1);
    printf("測資 1 結果: %s (預期: true)\n", ans1 ? "true" : "false");
    freeTree(root1);

    /* 測試測資 2: 非對稱樹 [1,2,2,null,3,null,3] */
    struct TreeNode* root2 = createNode(1);
    root2->left = createNode(2);
    root2->right = createNode(2);
    root2->left->right = createNode(3);
    root2->right->right = createNode(3);

    bool ans2 = isSymmetric(root2);
    printf("測資 2 結果: %s (預期: false)\n", ans2 ? "true" : "false");
    freeTree(root2);

    /* 測試測資 3: 單節點樹 [1] */
    struct TreeNode* root3 = createNode(1);
    bool ans3 = isSymmetric(root3);
    printf("測資 3 結果: %s (預期: true)\n", ans3 ? "true" : "false");
    freeTree(root3);

    return 0;
}
