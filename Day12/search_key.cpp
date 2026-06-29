TreeNode_t* search(TreeNode_t* root, int key)
{
    if(key == root->data)
    {
        return root;
    }
    if(key < root->data)
    {
        search(root->left,key);
    }
    else if(key > root->data)
    {
        search(root->right,key);
    }
}