#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE  8U

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    uint32_t head; // 寫入位置 (Write index / Producer)
    uint32_t tail; // 讀取位置 (Read index / Consumer)
    uint32_t count;
} RingBuffer_t;

void ring_buffer_init(RingBuffer_t *rb) {
    if (rb == NULL) {
        return;
    }
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

bool ring_buffer_is_full(const RingBuffer_t *rb) {
    if (rb == NULL) {
        return false;
    }
    return (rb->count == BUFFER_SIZE);
}

bool ring_buffer_is_empty(const RingBuffer_t *rb) {
    if (rb == NULL) {
        return true;
    }
    return (rb->count == 0);
}

bool ring_buffer_push(RingBuffer_t *rb, uint8_t data) {
    // 防禦性檢查：空指標或佇列已滿
    if (rb == NULL || ring_buffer_is_full(rb)) {
        return false;
    }

    // 寫入資料到 head 位置，並用 & (BUFFER_SIZE - 1) 取代 % 模運算回繞
    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1U) & (BUFFER_SIZE - 1U);
    rb->count++;
    return true;
}

bool ring_buffer_pop(RingBuffer_t *rb, uint8_t *data) {
    // 防禦性檢查：空指標或佇列已空
    if (rb == NULL || data == NULL || ring_buffer_is_empty(rb)) {
        return false;
    }

    // 從 tail 位置讀取資料，並用 & (BUFFER_SIZE - 1) 取代 % 模運算回繞
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1U) & (BUFFER_SIZE - 1U);
    rb->count--;
    return true;
}

/*
================================================================================
【原始盲點剖析與使用者程式碼存檔】
================================================================================
使用者原始實作：
bool ring_buffer_push(RingBuffer_t *rb, uint8_t data); // ❌ 多了分號
{
    if(ring_buffer_is_full) // ❌ 缺少函式呼叫括號與引數 (rb)
    {
        return false;
    }

    rb->buffer[rb->tail] = data; // ⚠️ 與 prompt 定義的 head (寫入) / tail (讀取) 顛倒
    rb->tail = (rb->tail + 1) & (BUFFER_SIZE - 1);
    rb->count += 1;
    return true;
};

bool ring_buffer_pop(RingBuffer_t *rb, uint8_t *data)
{
    if(ring_buffer_is_empty) // ❌ 缺少函式呼叫括號與引數 (rb)
    {
        return false;
    }

    *data = rb->buffer[rb->head]; // ⚠️ 與 prompt 定義的 head (寫入) / tail (讀取) 顛倒
    rb->head = (rb->head + 1) & (BUFFER_SIZE - 1);
    rb->count -= 1;
    return true; 
};

盲點 1：函式定義原型多了分號 (語法錯誤 C2449)
- 在 `bool ring_buffer_push(RingBuffer_t *rb, uint8_t data);` 行尾多了分號 `;`，
  導致編譯器將其判定為函式宣告原型，後面的 `{ ... }` 變成孤立區塊，引發 MSVC 編譯錯誤。

盲點 2：函式呼叫遺漏引數與括號 (Function Pointer 陷阱)
- 寫成 `if(ring_buffer_is_full)` 與 `if(ring_buffer_is_empty)`，在 C 語言中這不是呼叫函式，
  而是「取函式指針的記憶體位址」！因為函式指針必不為 NULL，導致這個 if 判斷永遠為真 (Always True)，
  造成永遠無法 push 也永遠無法 pop！
- 修正：必須帶入引數進行呼叫：`if (ring_buffer_is_full(rb))`。

盲點 3：Head 與 Tail 的職責慣例顛倒
- 依照定義：
  - `head`：佇列頭部，代表生產者（Push）寫入的位置。
  - `tail`：佇列尾部，代表消費者（Pop）讀取的位置。
  - 原始程式碼將 `push` 寫入 `tail`，`pop` 讀取 `head`，剛好語義顛倒。

優點亮點：
- 成功吸收並正確運用了位元遮罩 `& (BUFFER_SIZE - 1)` 替代代價高昂的模運算 `%`！
================================================================================
*/

int main(void) {
    printf("=== Day 99 - 0x10 儲存核心: Circular Buffer 單元測試 ===\n\n");

    RingBuffer_t rb;
    ring_buffer_init(&rb);

    printf("1. 初始化後狀態: Empty=%s, Full=%s, Count=%u\n",
           ring_buffer_is_empty(&rb) ? "true" : "false",
           ring_buffer_is_full(&rb) ? "true" : "false",
           rb.count);

    // 測試寫入 8 個 byte (剛好填滿)
    printf("2. 連續寫入 8 筆資料 (0x10 ~ 0x17):\n");
    for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
        bool ok = ring_buffer_push(&rb, (uint8_t)(0x10 + i));
        printf("   Push 0x%02X -> %s (Count=%u, Head=%u, Tail=%u)\n",
               0x10 + i, ok ? "OK" : "FAIL", rb.count, rb.head, rb.tail);
    }

    printf("3. 填滿後狀態: Empty=%s, Full=%s (預期 Full=true)\n",
           ring_buffer_is_empty(&rb) ? "true" : "false",
           ring_buffer_is_full(&rb) ? "true" : "false");

    // 測試溢位寫入 (滿了不可覆蓋)
    bool overflow_ok = ring_buffer_push(&rb, 0xFF);
    printf("4. 滿佇列嘗試 Push 0xFF: %s (預期 FAIL)\n\n", overflow_ok ? "OK" : "FAIL (正確防禦)");

    // 測試讀出 4 筆資料
    printf("5. 讀出 4 筆資料:\n");
    for (int i = 0; i < 4; i++) {
        uint8_t val = 0;
        bool ok = ring_buffer_pop(&rb, &val);
        printf("   Pop -> 0x%02X (%s, Count=%u, Tail=%u)\n", val, ok ? "OK" : "FAIL", rb.count, rb.tail);
    }

    // 測試環狀回繞 (Wrap-around): 再寫入 3 筆資料
    printf("6. 再寫入 3 筆資料 (0xA1, 0xA2, 0xA3) 驗證環狀回繞:\n");
    for (uint8_t i = 1; i <= 3; i++) {
        bool ok = ring_buffer_push(&rb, (uint8_t)(0xA0 + i));
        printf("   Push 0x%02X -> %s (Count=%u, Head=%u)\n", 0xA0 + i, ok ? "OK" : "FAIL", rb.count, rb.head);
    }

    // 清空剩餘所有資料
    printf("7. 讀出所有剩餘資料:\n");
    while (!ring_buffer_is_empty(&rb)) {
        uint8_t val = 0;
        ring_buffer_pop(&rb, &val);
        printf("   Pop Remaining -> 0x%02X\n", val);
    }

    printf("8. 清空後狀態: Empty=%s, Count=%u (預期 Empty=true)\n",
           ring_buffer_is_empty(&rb) ? "true" : "false", rb.count);

    return 0;
}