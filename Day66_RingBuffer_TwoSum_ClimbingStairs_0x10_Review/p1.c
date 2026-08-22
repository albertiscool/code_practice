#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

// ============================================================================
// 題目 1【設計環形緩衝區 (Design Circular Queue / Ring Buffer - LeetCode #622)】
// 
// 【背景】：
// Ring Buffer 是所有嵌入式韌體（UART 接收中斷、DMA 傳輸、音訊串流）最核心的資料結構！
// 
// 【結構體定義】：
// front: 指向隊頭元素
// rear: 指向下一個即將寫入的位置
// count: 目前存了幾個元素
// capacity: 佇列容量上限
// ============================================================================

typedef struct {
    int *buffer;
    int front;
    int rear;
    int count;
    int capacity;
} MyCircularQueue;

// 1. 初始化佇列，容量為 k
MyCircularQueue* myCircularQueueCreate(int k) {
    // 1. 配置結構體本體
    MyCircularQueue* obj = (MyCircularQueue*)malloc(sizeof(MyCircularQueue));
    if (obj == NULL) return NULL;

    // 2. 配置內部的整數陣列 (大小為 k)
    obj->buffer = (int*)malloc(k * sizeof(int));
    if (obj->buffer == NULL) {
        free(obj);
        return NULL;
    }

    // 3. 初始化所有狀態變數
    obj->front = 0;
    obj->rear = 0;
    obj->count = 0;     // 目前裝了 0 個元素
    obj->capacity = k;  // 容量上限為 k

    return obj;
}

// 2. 檢查佇列是否為空
bool myCircularQueueIsEmpty(MyCircularQueue* obj) {
    return obj->count == 0;
}

// 3. 檢查佇列是否已滿
bool myCircularQueueIsFull(MyCircularQueue* obj) {
    return obj->count == obj->capacity;
}

// 4. 向環形佇列插入一個元素。若成功插入回傳 true，若已滿回傳 false
bool myCircularQueueEnQueue(MyCircularQueue* obj, int value) {
    if (myCircularQueueIsFull(obj)) {
        return false; // 佇列已滿，拒絕寫入
    }

    obj->buffer[obj->rear] = value;               // 1. 寫入數值
    obj->rear = (obj->rear + 1) % obj->capacity;  // 2. 指標循環前進
    obj->count++;                                 // 3. 元素數量加 1

    return true;
}

// 5. 從環形佇列中刪除一個元素。若成功刪除回傳 true，若為空回傳 false
bool myCircularQueueDeQueue(MyCircularQueue* obj) {
    if (myCircularQueueIsEmpty(obj)) {
        return false; // 佇列已空，無法彈出
    }

    obj->front = (obj->front + 1) % obj->capacity; // 隊頭指針往前移一格
    obj->count--;                                  // 元素數量減 1

    return true;
}

// 6. 取得隊頭元素。若佇列為空回傳 -1
int myCircularQueueFront(MyCircularQueue* obj) {
    if (myCircularQueueIsEmpty(obj)) {
        return -1;
    }
    return obj->buffer[obj->front]; // 隊頭元素直接取 front
}

// 7. 取得隊尾元素。若佇列為空回傳 -1
int myCircularQueueRear(MyCircularQueue* obj) {
    if (myCircularQueueIsEmpty(obj)) {
        return -1;
    }
    // 🌟 因為 rear 指向的是「下一個即將寫入的位置」，所以最後一個剛寫入的元素在 rear - 1
    // 加上 capacity 避免 rear = 0 時 (0 - 1 = -1) 產生負數索引！
    int last_index = (obj->rear - 1 + obj->capacity) % obj->capacity;
    return obj->buffer[last_index];
}

// 8. 釋放記憶體
void myCircularQueueFree(MyCircularQueue* obj) {
    if (obj) {
        if (obj->buffer) free(obj->buffer);
        free(obj);
    }
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    MyCircularQueue* q = myCircularQueueCreate(3); // 容量為 3
    assert(myCircularQueueIsEmpty(q) == true);

    assert(myCircularQueueEnQueue(q, 1) == true); // 佇列: [1]
    assert(myCircularQueueEnQueue(q, 2) == true); // 佇列: [1, 2]
    assert(myCircularQueueEnQueue(q, 3) == true); // 佇列: [1, 2, 3]
    assert(myCircularQueueEnQueue(q, 4) == false); // 已滿，塞不進去！

    assert(myCircularQueueIsFull(q) == true);
    assert(myCircularQueueRear(q) == 3);
    assert(myCircularQueueFront(q) == 1);

    assert(myCircularQueueDeQueue(q) == true); // 彈出 1，佇列變為 [2, 3]
    assert(myCircularQueueEnQueue(q, 4) == true); // 環形寫入 4，佇列變為 [2, 3, 4]
    assert(myCircularQueueRear(q) == 4);
    assert(myCircularQueueFront(q) == 2);

    myCircularQueueFree(q);
    printf("🎉 p1.c Ring Buffer 所有測試案例全數 100%% 通過！\n");
    return 0;
}
