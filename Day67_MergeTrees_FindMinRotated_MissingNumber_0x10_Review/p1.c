#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// 二元樹節點定義
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// ============================================================================
// 題目 1【合併兩個二元樹 (Merge Two Binary Trees - LeetCode #617)】
// 演算法：前序遞迴合併法 ($O(N)$ 時間, $O(H)$ 空間)
// ============================================================================
struct TreeNode* mergeTrees(struct TreeNode* root1, struct TreeNode* root2)
{
    // 1. 邊界與遞迴終止條件 (Base Cases)
    if (root1 == NULL) return root2; // 若 root1 為空，直接回傳 root2 做為子樹
    if (root2 == NULL) return root1; // 若 root2 為空，直接回傳 root1 做為子樹

    // 2. 兩節點皆存在，將數值相加至 root1
    root1->val += root2->val;

    // 3. 左右子樹分別遞迴合併 (必須在 return 之前執行！)
    root1->left = mergeTrees(root1->left, root2->left);
    root1->right = mergeTrees(root1->right, root2->right);

    // 4. 回傳合併後的根節點
    return root1;
}

// 輔助函式：建立新節點
struct TreeNode* createNode(int val) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 輔助函式：釋放二元樹
void freeTree(struct TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main()
{
    // 建立樹 1:   1
    //           /   \
    //          3     2
    //         /
    //        5
    struct TreeNode* t1 = createNode(1);
    t1->left = createNode(3);
    t1->right = createNode(2);
    t1->left->left = createNode(5);

    // 建立樹 2:   2
    //           /   \
    //          1     3
    //           \     \
    //            4     7
    struct TreeNode* t2 = createNode(2);
    t2->left = createNode(1);
    t2->right = createNode(3);
    t2->left->right = createNode(4);
    t2->right->right = createNode(7);

    // 合併結果應為:
    //         3
    //       /   \
    //      4     5
    //     / \     \
    //    5   4     7
    struct TreeNode* merged = mergeTrees(t1, t2);

    assert(merged != NULL);
    assert(merged->val == 3);
    assert(merged->left->val == 4);
    assert(merged->right->val == 5);
    assert(merged->left->left->val == 5);
    assert(merged->left->right->val == 4);
    assert(merged->right->right->val == 7);

    printf("\n🎉 p1.c 合併二元樹 所有測試案例全數 100%% 通過！\n");
    freeTree(merged);
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（數值相加與左右遞迴思路 100% 正確！）：
struct TreeNode* mergeTrees_original(struct TreeNode* root1, struct TreeNode* root2)
{
    // 💡 盲點 1：如果兩棵樹都是 NULL，進入 else 後 root1->val 會記憶體越界 (Segmentation Fault)！
    // 正確邊界寫法：
    // if (root1 == NULL) return root2; (若 root1 為空，回傳 root2，不論 root2 是否為空都正確)
    // if (root2 == NULL) return root1;

    if(root1 == NULL && root2 != NULL) return root2;
    else if(root1 != NULL && root2 == NULL) return root1;
    else
    {
        root1->val = root1->val + root2->val;
        // 💡 盲點 2：return root1 放在這裡會提早跳出函式，導致底下的左右子樹遞迴 (line 42, 43) 永遠執行不到！
        return root1; 
    }

    // 💡 下面這兩行變成了不可達代碼 (Unreachable Code)
    root1->left = mergeTrees(root1->left,root2->left);
    root1->right = mergeTrees(root1->right,root2->right);
}
*/
