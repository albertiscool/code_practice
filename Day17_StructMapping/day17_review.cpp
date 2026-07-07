void allocate_buffer(int *ptr) {
    ptr = (int*)malloc(5 * sizeof(int));
}
int main() {
    int *my_ptr = NULL;
    allocate_buffer(my_ptr);
    // 接下來使用 my_ptr ...
}