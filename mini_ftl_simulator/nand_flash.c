#include "nand_flash.h"
#include <stdio.h>
#include <string.h>

static NandBlock_t flash_blocks[BLOCK_NUM];
static NandStats_t flash_stats;

void nand_init(void) {
    memset(flash_blocks, 0, sizeof(flash_blocks));
    memset(&flash_stats, 0, sizeof(flash_stats));

    for (uint32_t b = 0; b < BLOCK_NUM; b++) {
        for (uint32_t p = 0; p < PAGES_PER_BLOCK; p++) {
            flash_blocks[b].pages[p].state = PAGE_FREE;
            flash_blocks[b].pages[p].lba = -1;
            memset(flash_blocks[b].pages[p].data, 0xFF, PAGE_DATA_SIZE); // NAND 抹除後預設為 0xFF
        }
        flash_blocks[b].erase_count = 0;
    }
}

bool nand_read_page(uint32_t block, uint32_t page, uint8_t *buffer_out) {
    if (block >= BLOCK_NUM || page >= PAGES_PER_BLOCK || buffer_out == NULL) {
        return false;
    }

    memcpy(buffer_out, flash_blocks[block].pages[page].data, PAGE_DATA_SIZE);
    flash_stats.read_count++;
    return true;
}

bool nand_program_page(uint32_t block, uint32_t page, const uint8_t *buffer_in, int32_t lba) {
    if (block >= BLOCK_NUM || page >= PAGES_PER_BLOCK || buffer_in == NULL) {
        return false;
    }

    // 💥 嚴格模擬 NAND 物理限制：寫入前必須先抹除 (Erase-before-Write)！
    if (flash_blocks[block].pages[page].state != PAGE_FREE) {
        printf("[NAND ERROR] 試圖覆寫非 FREE 狀態的 Page (Block %u, Page %u)！違反 NAND 物理特性！\n",
               block, page);
        return false;
    }

    memcpy(flash_blocks[block].pages[page].data, buffer_in, PAGE_DATA_SIZE);
    flash_blocks[block].pages[page].state = PAGE_VALID;
    flash_blocks[block].pages[page].lba = lba;
    flash_stats.program_count++;
    return true;
}

bool nand_erase_block(uint32_t block) {
    if (block >= BLOCK_NUM) {
        return false;
    }

    // 抹除只能以整個 Block 為單位進行
    for (uint32_t p = 0; p < PAGES_PER_BLOCK; p++) {
        flash_blocks[block].pages[p].state = PAGE_FREE;
        flash_blocks[block].pages[p].lba = -1;
        memset(flash_blocks[block].pages[p].data, 0xFF, PAGE_DATA_SIZE);
    }
    flash_blocks[block].erase_count++;
    flash_stats.erase_count++;
    return true;
}

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
