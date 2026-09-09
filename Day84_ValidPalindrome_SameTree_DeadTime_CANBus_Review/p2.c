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
// 題目 2【相同的樹 (Same Tree - LeetCode #100)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠白板高頻題)
// 演算法：DFS 雙樹同步遞迴 - $O(\min(N, M))$ 時間, $O(\min(H1, H2))$ 空間
// ============================================================================

bool isSameTree(struct TreeNode* p, struct TreeNode* q)
{
    // 1. 情況一：兩者皆為空節點 ➔ 結構相同，回傳 true
    if (p == NULL && q == NULL)
    {
        return true;
    }

    // 2. 情況二：其中一個為空、另一個不為空 ➔ 結構不同，回傳 false (亦防止後續存取 NULL 造成 Segfault)
    if (p == NULL || q == NULL)
    {
        return false;
    }

    // 3. 情況三：兩者皆存在，但節點數值不同 ➔ 回傳 false
    if (p->val != q->val)
    {
        return false;
    }

    // 4. 情況四：當前節點完全相同，同步遞迴驗證左子樹與右子樹
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
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
    // 測試 1: 相同的樹 [1, 2, 3] 與 [1, 2, 3] ➔ true
    struct TreeNode* p1 = createTreeNode(1);
    p1->left = createTreeNode(2);
    p1->right = createTreeNode(3);

    struct TreeNode* q1 = createTreeNode(1);
    q1->left = createTreeNode(2);
    q1->right = createTreeNode(3);

    assert(isSameTree(p1, q1) == true);
    printf("測試 1 通過: [1, 2, 3] 與 [1, 2, 3] 判定為相同 (true)\n");
    freeTree(p1);
    freeTree(q1);

    // 測試 2: 結構不同 [1, 2] 與 [1, null, 2] ➔ false
    struct TreeNode* p2 = createTreeNode(1);
    p2->left = createTreeNode(2);

    struct TreeNode* q2 = createTreeNode(1);
    q2->right = createTreeNode(2);

    assert(isSameTree(p2, q2) == false);
    printf("測試 2 通過: [1, 2] 與 [1, null, 2] 判定為不同 (false)\n");
    freeTree(p2);
    freeTree(q2);

    // 測試 3: 數值不同 [1, 2, 1] 與 [1, 1, 2] ➔ false
    struct TreeNode* p3 = createTreeNode(1);
    p3->left = createTreeNode(2);
    p3->right = createTreeNode(1);

    struct TreeNode* q3 = createTreeNode(1);
    q3->left = createTreeNode(1);
    q3->right = createTreeNode(2);

    assert(isSameTree(p3, q3) == false);
    printf("測試 3 通過: [1, 2, 1] 與 [1, 1, 2] 判定為不同 (false)\n");
    freeTree(p3);
    freeTree(q3);

    // 測試 4: 雙空樹 NULL 與 NULL ➔ true
    assert(isSameTree(NULL, NULL) == true);
    printf("測試 4 通過: NULL 與 NULL 判定為相同 (true)\n");

    printf("\n🎉 p2.c 相同的樹所有測試案例全數 100%% 通過！\n");
    return 0;
}
