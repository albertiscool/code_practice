// 1. 參數改成 int **ptr (收下一把雙重鑰匙)
void create_buffer(int **ptr, int size) {
    // 2. 用 *ptr 提領，也就是直接對準外面的 my_buf，把 malloc 的新地址塞進去！
    *ptr = (int*)malloc(size * sizeof(int));
    
    for(int i = 0; i < size; i++) {
        // (*ptr) 就是 my_buf，再加上陣列的 [i] 操作
        (*ptr)[i] = i * 10;
    }
}

int main() {
    int *my_buf = NULL;
    
    // 3. 傳遞 my_buf 的記憶體位址進去！
    create_buffer(&my_buf, 5);
    
    if(my_buf != NULL) {
        printf("%d\n", my_buf[2]); // 完美印出 20
        free(my_buf);              // 安全釋放
    }
    return 0;
}