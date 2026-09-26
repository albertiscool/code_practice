#include "nand_flash.h"
#include <stdio.h>
#include <string.h>

// 實體 NAND 晶片儲存陣列 (4 個 Blocks，每個 Block 4 個 Pages)
static NandBlock_t flash_blocks[BLOCK_NUM];
static NandStats_t flash_stats;

/*
 * 【任務 1.1】初始化 NAND Flash
 * 說明：
 * 1. 將所有 Block 的所有 Page 狀態設為 PAGE_FREE。
 * 2. 將所有 Page 的 lba 設為 -1 (代表未存放任何邏輯資料)。
 * 3. 將所有 Page 的 data 填入 0xFF (真實 NAND 抹除後的預設位元為全 1)。
 * 4. 將 erase_count 與 flash_stats 全數歸零。
 */
void nand_init(void) {
    // === 請在此實作你的代碼 ===
}

/*
 * 【任務 1.2】讀取單一 Page (64 Bytes)
 * 說明：
 * 1. 檢查邊界條件：block 是否合法？page 是否合法？buffer_out 是否為 NULL？
 * 2. 使用 memcpy 將該 Page 的 data 複製到 buffer_out。
 * 3. 累加 flash_stats.read_count。
 * 4. 成功回傳 true，失敗回傳 false。
 */
bool nand_read_page(uint32_t block, uint32_t page, uint8_t *buffer_out) {
    // === 請在此實作你的代碼 ===
    return false;
}

/*
 * 【任務 1.3】寫入 (Program) 單一 Page
 * 說明：
 * 1. 檢查邊界條件：block, page 是否合法？buffer_in 是否為 NULL？
 * 2. ★ 模擬 NAND 物理鐵律 (Erase-before-Write)：
 *    如果目標 Page 的狀態「不是 PAGE_FREE」，代表在未抹除前試圖覆寫！
 *    印出錯誤訊息並回傳 false！
 * 3. 寫入資料：
 *    - 將 buffer_in 內容複製到該 Page 的 data。
 *    - 將該 Page 的狀態改為 PAGE_VALID。
 *    - 將該 Page 的 lba 欄位記錄傳入的 lba。
 * 4. 累加 flash_stats.program_count，回傳 true。
 */
bool nand_program_page(uint32_t block, uint32_t page, const uint8_t *buffer_in, int32_t lba) {
    // === 請在此實作你的代碼 ===
    return false;
}

/*
 * 【任務 1.4】抹除整個 Block (Erase)
 * 說明：
 * 1. 檢查 block 是否合法？
 * 2. ★ 模擬 NAND 物理抹除特性：
 *    抹除只能對「整個 Block」操作！
 *    遍歷該 Block 內的所有 Page，將狀態全部重設為 PAGE_FREE，
 *    lba 重設為 -1，data 重新刷成 0xFF。
 * 3. 該 Block 的 erase_count 累加 1。
 * 4. 全域統計 flash_stats.erase_count 累加 1，回傳 true。
 */
bool nand_erase_block(uint32_t block) {
    // === 請在此實作你的代碼 ===
    return false;
}

// ============================================================================
// 底層輔助函式 (已為你寫好，提供 FTL 查詢與除錯使用)
// ============================================================================
void nand_set_page_state(uint32_t block, uint32_t page, PageState_t state) {
    if (block < BLOCK_NUM && page < PAGES_PER_BLOCK) {
        flash_blocks[block].pages[page].state = state;
    }
}

PageState_t nand_get_page_state(uint32_t block, uint32_t page) {
    if (block < BLOCK_NUM && page < PAGES_PER_BLOCK) {
        return flash_blocks[block].pages[page].state;
    }
    return PAGE_INVALID;
}

int32_t nand_get_page_lba(uint32_t block, uint32_t page) {
    if (block < BLOCK_NUM && page < PAGES_PER_BLOCK) {
        return flash_blocks[block].pages[page].lba;
    }
    return -1;
}

NandStats_t nand_get_stats(void) {
    return flash_stats;
}

void nand_print_layout(void) {
    printf("+-------+------------------------------------------+-------+\n");
    printf("| Block | Pages (State: V=Valid, I=Invalid, F=Free)| Erase |\n");
    printf("+-------+------------------------------------------+-------+\n");
    for (uint32_t b = 0; b < BLOCK_NUM; b++) {
        printf("|   %u   | ", b);
        for (uint32_t p = 0; p < PAGES_PER_BLOCK; p++) {
            PageState_t st = flash_blocks[b].pages[p].state;
            int32_t lba = flash_blocks[b].pages[p].lba;
            if (st == PAGE_VALID) {
                printf("[V:L%02d] ", lba);
            } else if (st == PAGE_INVALID) {
                printf("[I:L%02d] ", lba);
            } else {
                printf("[  F  ] ");
            }
        }
        printf("|  %3u  |\n", flash_blocks[b].erase_count);
    }
    printf("+-------+------------------------------------------+-------+\n");
}
