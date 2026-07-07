#include <stdio.h>
#include <stdlib.h>

void create_buffer(int *ptr, int size) {
    ptr = (int*)malloc(size * sizeof(int));
    for(int i = 0; i < size; i++) {
        *(ptr+i) = i * 10;
    }
}

int main() {
    int *my_buf = NULL;
    create_buffer(my_buf, 5);
    
    // 預期印出 20 (my_buf[2])
    if(my_buf != NULL) {
        printf("%d\n", &(my_buf+2));
        free(my_buf);
    }
    return 0;
}