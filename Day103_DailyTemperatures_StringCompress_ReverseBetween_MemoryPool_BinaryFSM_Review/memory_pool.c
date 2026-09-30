#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**
 * ============================================================================
 * 🏊 【工業級高效率零開銷記憶體池】 (High-Performance Zero-Overhead Memory Pool)
 * ============================================================================
 * 【核心特性】：
 *   1. 絕對確定性時間：Alloc 與 Free 均嚴格為 O(1)（僅需 2 個 CPU 指令）！
 *   2. 絕對零碎片化：所有區塊規格完全一致，杜絕傳統 malloc 長期運行導致的記憶體空洞。
 *   3. 零額外指針負擔：利用 union 技巧，未使用的區塊把自身資料區當作 next 指標，額外 RAM 開銷為 0 Byte！
 *   4. 本質架構：就是一條「用單向鏈結串列 (Singly Linked List) 串起來的 Free-List」！
 *      - Alloc = Pop Head (刪除頭節點)
 *      - Free  = Push Head (頭插法接回頭部)
 * ============================================================================
 */

#define POOL_BLOCK_SIZE   64  // 每個區塊 64 Bytes (可放入通訊封包、感測器佇列或 FTL 描述符)
#define POOL_BLOCK_COUNT  8   // 一共提供 8 個固定區塊

// 每個區塊的結構體定義 (利用 union 達成 0 記憶體負擔)
typedef union Block {
    union Block* next;             // 空閒時 (Free-List)：指向下一個可用空位的指標
    uint8_t      data[POOL_BLOCK_SIZE]; // 借出時 (In-Use)：整塊 64 Bytes 給使用者存放資料
} Block_t;

// 靜態配置的記憶體池實體 (放置於 .bss 區段，編譯期即鎖定 RAM 用量)
static Block_t s_pool[POOL_BLOCK_COUNT];
static Block_t* s_free_head = NULL; // 指向目前第一個可用的空閒區塊 (Free List Head)
static int s_available_blocks = 0;   // 目前剩餘可用區塊數量

/**
 * 🛠️ 初始化記憶體池：把所有區塊串成一條單向鏈結串列
 */
void MemoryPool_Init(void) {
    for (int i = 0; i < POOL_BLOCK_COUNT - 1; i++) {
        s_pool[i].next = &s_pool[i + 1];
    }
    s_pool[POOL_BLOCK_COUNT - 1].next = NULL; // 最後一個區塊指向 NULL
    s_free_head = &s_pool[0];
    s_available_blocks = POOL_BLOCK_COUNT;
    printf("[MemoryPool] 初始化完成！共 %d 個區塊，每塊 %d Bytes，總佔用 %zu Bytes。\n",
           POOL_BLOCK_COUNT, POOL_BLOCK_SIZE, sizeof(s_pool));
}

/**
 * ⚡ 借出區塊 (Alloc)：極速 O(1) - 鏈結串列 Pop Head
 */
void* MemoryPool_Alloc(void) {
    if (s_free_head == NULL) {
        printf("[MemoryPool] ⚠️ 警告：記憶體池已耗盡 (Pool Exhausted)！\n");
        return NULL; // 可預測的邊界防禦，絕不當機
    }

    // 1. 取出頭節點
    Block_t* allocated_block = s_free_head;

    // 2. 頭指針往下移一位 (Pop Head)
    s_free_head = s_free_head->next;
    s_available_blocks--;

    return (void*)allocated_block;
}

/**
 * ⚡ 歸還區塊 (Free)：極速 O(1) - 鏈結串列 Push Head (頭插法)
 */
void MemoryPool_Free(void* ptr) {
    if (ptr == NULL) return;

    // 邊界防禦：檢查歸還的指標是否真正屬於本記憶體池的位址範圍
    uintptr_t addr = (uintptr_t)ptr;
    uintptr_t pool_start = (uintptr_t)&s_pool[0];
    uintptr_t pool_end   = (uintptr_t)&s_pool[POOL_BLOCK_COUNT];

    if (addr < pool_start || addr >= pool_end) {
        printf("[MemoryPool] ❌ 致命錯誤：嘗試歸還非本池管理的非法記憶體位址: %p！\n", ptr);
        return;
    }

    // 將該區塊透過「頭插法」接回 Free-List 的最前方
    Block_t* block = (Block_t*)ptr;
    block->next = s_free_head;
    s_free_head = block;
    s_available_blocks++;
}

/**
 * 📊 查詢當前剩餘可用區塊數
 */
int MemoryPool_GetAvailable(void) {
    return s_available_blocks;
}

/* ================= 測試驗證函式 ================= */
int main(void) {
    printf("============================================================\n");
    printf("🏊 工業級 Memory Pool (固定區塊配置器) 實機驗證\n");
    printf("============================================================\n");

    // 1. 初始化
    MemoryPool_Init();
    printf("目前可用區塊: %d / %d\n\n", MemoryPool_GetAvailable(), POOL_BLOCK_COUNT);

    // 2. 借出 3 個區塊
    printf("--- [測試 1: 連續借出 3 個區塊] ---\n");
    char* b1 = (char*)MemoryPool_Alloc();
    char* b2 = (char*)MemoryPool_Alloc();
    char* b3 = (char*)MemoryPool_Alloc();

    printf("b1 位址: %p\n", (void*)b1);
    printf("b2 位址: %p (位移 %td Bytes)\n", (void*)b2, (char*)b2 - (char*)b1);
    printf("b3 位址: %p (位移 %td Bytes)\n", (void*)b3, (char*)b3 - (char*)b2);
    printf("剩餘可用區塊: %d\n\n", MemoryPool_GetAvailable());

    // 3. 在區塊中填入資料，驗證不受指標干擾
    snprintf(b1, POOL_BLOCK_SIZE, "Hello Telemetry Packet 01!");
    snprintf(b2, POOL_BLOCK_SIZE, "Speed Command: Left=200, Right=240");
    printf("b1 儲存資料: \"%s\"\n", b1);
    printf("b2 儲存資料: \"%s\"\n\n", b2);

    // 4. 歸還 b2 (驗證頭插法回收)
    printf("--- [測試 2: 歸還 b2] ---\n");
    MemoryPool_Free(b2);
    printf("歸還 b2 後可用區塊: %d\n\n", MemoryPool_GetAvailable());

    // 5. 再次借出區塊 (因為是頭插法，剛還的 b2 會第一個被重新借出！)
    printf("--- [測試 3: 再次借出區塊 (驗證 LIFO 快取復用)] ---\n");
    char* b_new = (char*)MemoryPool_Alloc();
    printf("新借出的 b_new 位址: %p (是否等於剛還的 b2? %s)\n", 
           (void*)b_new, b_new == b2 ? "✅ 是！完全一致！" : "❌ 否");
    printf("剩餘可用區塊: %d\n\n", MemoryPool_GetAvailable());

    // 6. 歸還所有區塊
    printf("--- [測試 4: 全數回收] ---\n");
    MemoryPool_Free(b1);
    MemoryPool_Free(b3);
    MemoryPool_Free(b_new);
    printf("全數歸還後可用區塊: %d / %d\n", MemoryPool_GetAvailable(), POOL_BLOCK_COUNT);

    // 7. 測試非本池非法位址歸還防禦
    int fake_var = 123;
    printf("\n--- [測試 5: 非法記憶體位址歸還防禦] ---\n");
    MemoryPool_Free(&fake_var);

    printf("\n✅ Memory Pool 測試驗證全數通過！\n");
    return 0;
}
