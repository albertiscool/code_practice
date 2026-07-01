TreeNode_t* search(TreeNode_t* root, int key) {
    // 1. 終止條件：走到死路沒找到，或是剛好找到了！
    if (root == NULL || root->data == key) {
        return root; 
    }
    
    // 2. 叫左邊小弟去找，並記得把結果「往上交差 (return)」
    if (key < root->data) {
        return search(root->left, key);
    }
    // 3. 叫右邊小弟去找，並記得把結果「往上交差 (return)」
    else {
        return search(root->right, key);
    }
}