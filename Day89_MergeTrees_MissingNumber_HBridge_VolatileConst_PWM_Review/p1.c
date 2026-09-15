#include <stdio.h>
#include <stdlib.h>

/**
 * Definition for a binary tree node.
 */
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// ============================================================================
// 題目 1【合併二元樹 (Merge Two Binary Trees - LeetCode #617)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (聯發科、瑞昱、群聯二元樹遞迴經典題)
// 題目說明：給定兩棵二元樹 root1 和 root2。
//           想像一下，當你將其中一棵覆蓋在另一棵之上時，兩棵樹的某些節點便會重疊。
//           你需要將兩棵樹合併為一棵新的二元樹。
//           合併的規則是：如果兩個節點重疊，那麼將這兩個節點的值相加作為合併後節點的新值；
//           否則，不為 NULL 的節點將直接作為新二元樹的節點。
//           返回合併後的二元樹。
//           注意：合併過程必須從兩個樹的根節點開始。
// 範例 1：
//    輸入：root1 = [1,3,2,5], root2 = [2,1,3,null,4,null,7]
//          Tree 1               Tree 2                  Merged Tree
//            1                    2                         3
//           / \                  / \                       / \
//          3   2                1   3                     4   5
//         /                      \   \                   / \   \
//        5                        4   7                 5   4   7
//    輸出：[3,4,5,5,4,null,7]
// 範例 2：
//    輸入：root1 = [1], root2 = [1,2]
//    輸出：[2,2]
// 限制條件：
//    - 兩棵樹的節點數目在範圍 [0, 2000] 內。
//    - -10^4 <= Node.val <= 10^4
// ============================================================================

struct TreeNode* mergeTrees(struct TreeNode* root1, struct TreeNode* root2) {
    // 1. 若其中一棵樹為空，直接回傳另一棵樹的節點 (整枝子樹直接嫁接，不需額外建立)
    if (root1 == NULL) {
        return root2;
    }
    if (root2 == NULL) {
        return root1;
    }

    // 2. 兩者皆非空：將 root2 的數值累加至 root1 上 (原地覆寫省空間)
    root1->val += root2->val;

    // 3. 遞迴合併左右子樹，並將回傳結果重新掛載到 root1 的左右指標上
    root1->left = mergeTrees(root1->left, root2->left);
    root1->right = mergeTrees(root1->right, root2->right);

    return root1;
}

/*
 * ============================================================================
 * 【原始錯誤實作紀錄與盲點分析】
 * 錯誤原因：
 * 1. 致命空指標取值 (NULL Pointer Dereference / Segfault 崩潰)：
 *    只檢查了 `if (root1 == NULL && root2 == NULL)`。
 *    若遇到「其中一棵為 NULL、另一棵非 NULL」時 (例如 root1 為 NULL)，
 *    進入 else 執行 `root1->val + root2->val` 會直接對 NULL 取 val 引發當機！
 * 2. 缺少指標嫁接賦值：
 *    遞迴呼叫時寫 `mergeTrees(root1->left, root2->left);` 但沒有接住回傳值！
 *    當 root1->left 原本為 NULL 而 root2->left 有節點時，
 *    Tree 2 的子節點永遠無法被掛載到 Tree 1 上。
 *
 * 正確思維：
 * - 當 root1 == NULL 時直接 return root2（把整根 Tree 2 嫁接過去）。
 * - 當 root2 == NULL 時直接 return root1。
 * - 遞迴回傳值必須賦值：`root1->left = mergeTrees(...)`。
 * ============================================================================
struct TreeNode* mergeTrees_wrong(struct TreeNode* root1, struct TreeNode* root2) {
    if(root1 == NULL && root2 == NULL)
    {
        return NULL;
    }
    else
    {
        root1->val = root1->val + root2->val; // ❌ 盲點 1：若任一為 NULL，直接 Segfault 崩潰
        mergeTrees(root1->left,root2->left);  // ❌ 盲點 2：未接住回傳值賦給 root1->left，無法嫁接
        mergeTrees(root1->right,root2->right);
        return root1;
    }
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

/* 輔助函式：前序走訪印出樹結構 */
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
    /* 建立 Tree 1: [1, 3, 2, 5] */
    struct TreeNode* t1 = createNode(1);
    t1->left = createNode(3);
    t1->right = createNode(2);
    t1->left->left = createNode(5);

    /* 建立 Tree 2: [2, 1, 3, null, 4, null, 7] */
    struct TreeNode* t2 = createNode(2);
    t2->left = createNode(1);
    t2->right = createNode(3);
    t2->left->right = createNode(4);
    t2->right->right = createNode(7);

    printf("Tree 1 前序: ");
    printPreorder(t1);
    printf("\nTree 2 前序: ");
    printPreorder(t2);
    printf("\n");

    struct TreeNode* merged = mergeTrees(t1, t2);

    printf("合併後前序 : ");
    printPreorder(merged);
    printf("\n預期合併前序: 3 4 5 4 5 null 7 \n");

    freeTree(merged);
    return 0;
}
