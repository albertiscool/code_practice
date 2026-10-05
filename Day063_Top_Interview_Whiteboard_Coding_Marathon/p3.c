#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 節點結構體
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// ============================================================================
// 寫法 1：你的迭代迴圈法 (Iterative) - 嵌入式系統最推薦 $O(1)$ 零 Stack 開銷！
// ============================================================================
struct TreeNode* searchBST(struct TreeNode* root, int val)
{
    struct TreeNode *curr = root;

    // 🌟 標準迴圈條件：只要 curr 還沒走到 NULL 就繼續找！
    while (curr != NULL)
    {
        if (curr->val == val)
        {
            return curr; // 找到了，直接回傳該節點！
        }
        else if (val < curr->val)
        {
            curr = curr->left;  // 目標較小，往左走
        }
        else
        {
            curr = curr->right; // 目標較大，往右走
        }
    }

    return NULL; // 走到 NULL 都沒找到
}

// ============================================================================
// 寫法 2：極簡遞迴法 (Recursive) - 只要 4 行代碼！
// ============================================================================
struct TreeNode* searchBST_recursive(struct TreeNode* root, int val)
{
    if (root == NULL || root->val == val)
    {
        return root;
    }
    return (val < root->val) ? searchBST_recursive(root->left, val) : searchBST_recursive(root->right, val);
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
    /* 建立測試 BST：
            4
          /   \
         2     7
        / \
       1   3
    */
    struct TreeNode *root = createNode(4);
    root->left = createNode(2);
    root->right = createNode(7);
    root->left->left = createNode(1);
    root->left->right = createNode(3);

    // 測試 1：搜尋中間節點 2 (預期: 找到節點 2)
    struct TreeNode *res1 = searchBST(root, 2);
    printf("搜尋 2 -> 找到節點: %d\n", res1 ? res1->val : -1);
    assert(res1 != NULL && res1->val == 2);

    // 測試 2：搜尋葉子節點 1 (預期: 找到節點 1)
    struct TreeNode *res2 = searchBST(root, 1);
    printf("搜尋 1 -> 找到節點: %d\n", res2 ? res2->val : -1);
    assert(res2 != NULL && res2->val == 1);

    // 測試 3：搜尋不存在的節點 5 (預期: NULL)
    struct TreeNode *res3 = searchBST(root, 5);
    printf("搜尋 5 -> 結果: %s\n", res3 == NULL ? "NULL (未找到)" : "找到");
    assert(res3 == NULL);

    printf("\n🎉 所有測試案例全部 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（你選擇的迭代法在韌體上非常優秀！）：
struct TreeNode* searchBST_original(struct TreeNode* root, int val)
{
    struct TreeNode *curr = root;
    if(root == NULL)
    {
        return NULL;
    }

    // 💡 唯一盲點：不可寫 (curr->left != NULL && curr->right != NULL)！
    // 否則當走到「只有單邊小孩」或「葉子節點 (如 1 或 3)」時，迴圈會提前跳出沒比對到數值！
    // 正確寫法為：while (curr != NULL) 即可！
    while(curr->left != NULL && curr->right != NULL)
    {
        if(curr->val == val)
        {
            return curr;
        }
        else if(curr->val > val)
        {
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }
    }
    
    return NULL;
}
*/