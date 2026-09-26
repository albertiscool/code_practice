#ifndef NAND_FLASH_H
#define NAND_FLASH_H

#include <stdint.h>
#include <stdbool.h>

// NAND Flash 規格參數 (為便於觀察與測試，以微縮規格設計)
#define BLOCK_NUM         4U   // 共有 4 個物理 Block
#define PAGES_PER_BLOCK   4U   // 每個 Block 有 4 個 Page
#define TOTAL_PAGES       (BLOCK_NUM * PAGES_PER_BLOCK) // 總共 16 個物理 Page
#define PAGE_DATA_SIZE    64U  // 每個 Page 存放 64 Bytes 資料

// 物理 Page 狀態定義
typedef enum {
    PAGE_FREE = 0, // 空白頁 (已抹除，可寫入)
    PAGE_VALID,    // 有效頁 (存放最新有效資料)
    PAGE_INVALID   // 無效頁 (被覆寫後的垃圾資料，等待 GC 抹除)
} PageState_t;

// 單一物理 Page 結構
typedef struct {
    uint8_t data[PAGE_DATA_SIZE];
    PageState_t state;
    int32_t lba; // 反向記錄該 Page 對應的邏輯 LBA (GC 搬移時更新 L2P 必備)
} NandPage_t;

// 單一物理 Block 結構
typedef struct {
    NandPage_t pages[PAGES_PER_BLOCK];
    uint32_t erase_count; // 該 Block 的抹寫次數 (可用於 Wear Leveling 擴充)
} NandBlock_t;

// 硬體統計指標
typedef struct {
    uint32_t read_count;
    uint32_t program_count;
    uint32_t erase_count;
} NandStats_t;

// 物理層操作 API
void nand_init(void);
bool nand_read_page(uint32_t block, uint32_t page, uint8_t *buffer_out);
bool nand_program_page(uint32_t block, uint32_t page, const uint8_t *buffer_in, int32_t lba);
bool nand_erase_block(uint32_t block);
void nand_set_page_state(uint32_t block, uint32_t page, PageState_t state);
PageState_t nand_get_page_state(uint32_t block, uint32_t page);
int32_t nand_get_page_lba(uint32_t block, uint32_t page);
NandStats_t nand_get_stats(void);
void nand_print_layout(void);

#endif // NAND_FLASH_H
