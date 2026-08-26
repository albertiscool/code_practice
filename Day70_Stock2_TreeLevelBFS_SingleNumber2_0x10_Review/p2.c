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
// 題目 2【二元樹的層序走訪 / 廣度優先搜尋 (Binary Tree Level Order Traversal - LeetCode #102)】
// 難度：🟢 Easy / 🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：佇列廣度優先搜尋 (Queue-based BFS) - $O(N)$ 時間, $O(N)$ 空間
// ============================================================================
int* levelOrder(struct TreeNode* root, int* returnSize)
{
    // -------------------------------------------------------------
    // 第 1 步：防呆處理 (若樹是空的，直接回傳 NULL 與長度 0)
    // -------------------------------------------------------------
    if (root == NULL)
    {
        *returnSize = 0;
        return NULL;
    }

    // -------------------------------------------------------------
    // 第 2 步：準備「結果陣列 (result)」用來裝走訪出來的數字
    // -------------------------------------------------------------
    int* result = (int*)malloc(1000 * sizeof(int));
    int count = 0; // 紀錄目前存了幾個數字

    // -------------------------------------------------------------
    // 第 3 步：用「陣列」建立一個 Queue 佇列 (先進先出 FIFO)
    // head 指向隊頭 (準備出隊的人)，tail 指向隊尾 (準備排隊的位置)
    // -------------------------------------------------------------
    struct TreeNode* queue[1000];
    int head = 0;
    int tail = 0;

    // -------------------------------------------------------------
    // 第 4 步：將樹的根節點 (root) 推入佇列中開始排隊！
    // -------------------------------------------------------------
    queue[tail] = root;
    tail++;

    // -------------------------------------------------------------
    // 第 5 步：BFS 核心迴圈！
    // 只要隊伍裡還有節點 (head < tail)，就持續進行「出隊 ➔ 存值 ➔ 抓小孩排隊」
    // -------------------------------------------------------------
    while (head < tail)
    {
        // 1. 出隊：取出隊頭節點
        struct TreeNode* curr = queue[head];
        head++;

        // 2. 存值：將當前節點的數值加到 result 陣列中
        result[count] = curr->val;
        count++;

        // 3. 抓左小孩：如果左子樹存在，推進隊伍末尾排隊
        if (curr->left != NULL)
        {
            queue[tail] = curr->left;
            tail++;
        }

        // 4. 抓右小孩：如果右子樹存在，推進隊伍末尾排隊
        if (curr->right != NULL)
        {
            queue[tail] = curr->right;
            tail++;
        }
    }

    // -------------------------------------------------------------
    // 第 6 步：設定回傳陣列的長度，並回傳 result 指標
    // -------------------------------------------------------------
    *returnSize = count;
    return result;
}

// 輔助函式：建立新節點
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
    // 建立二元樹:     3
    //               /   \
    //              9     20
    //                   /  \
    //                  15   7
    struct TreeNode* root = createNode(3);
    root->left = createNode(9);
    root->right = createNode(20);
    root->right->left = createNode(15);
    root->right->right = createNode(7);

    int returnSize = 0;
    int* res = levelOrder(root, &returnSize);

    // 預期層序走訪結果: [3, 9, 20, 15, 7]
    assert(returnSize == 5);
    printf("測試 1 層序走訪結果: [%d, %d, %d, %d, %d]\n",
           res[0], res[1], res[2], res[3], res[4]);
    assert(res[0] == 3 && res[1] == 9 && res[2] == 20 && res[3] == 15 && res[4] == 7);

    free(res);
    freeTree(root);
    printf("\n🎉 p2.c 二元樹層序走訪 (BFS) 所有測試案例全數 100%% 通過！\n");
    return 0;
}
