#include "ftl.h"
#include <stdio.h>
#include <string.h>

static Pba_t l2p_table[LOGICAL_LBA_NUM];
static FtlStats_t ftl_stats;

// 目前正在寫入的活躍區塊 (Active Block) 與頁 (Active Page)
static int32_t active_block = -1;
static int32_t active_page = 0;

// 輔助函式：尋找一個完全空白的 Block (所有 Page 都是 FREE)
static int32_t find_free_block(void) {
    for (uint32_t b = 0; b < BLOCK_NUM; b++) {
        bool all_free = true;
        for (uint32_t p = 0; p < PAGES_PER_BLOCK; p++) {
            if (nand_get_page_state(b, p) != PAGE_FREE) {
                all_free = false;
                break;
            }
        }
        if (all_free) {
            return (int32_t)b;
        }
    }
    return -1;
}

// 輔助函式：統計剩餘完全空白的 Block 數量
static uint32_t count_free_blocks(void) {
    uint32_t free_cnt = 0;
    for (uint32_t b = 0; b < BLOCK_NUM; b++) {
        bool all_free = true;
        for (uint32_t p = 0; p < PAGES_PER_BLOCK; p++) {
            if (nand_get_page_state(b, p) != PAGE_FREE) {
                all_free = false;
                break;
            }
        }
        if (all_free) free_cnt++;
    }
    return free_cnt;
}

void ftl_init(void) {
    nand_init();
    memset(&ftl_stats, 0, sizeof(ftl_stats));

    for (uint32_t i = 0; i < LOGICAL_LBA_NUM; i++) {
        l2p_table[i].block = -1;
        l2p_table[i].page = -1;
    }

    active_block = find_free_block();
    active_page = 0;
}

// 核心演算法：Greedy 貪婪垃圾回收 (Garbage Collection)
bool ftl_trigger_gc(void) {
    ftl_stats.gc_trigger_count++;
    printf("\n>>> [FTL GC] 觸發垃圾回收 (Garbage Collection)！開始搜尋受害者區塊...\n");

    // 1. 貪婪策略 (Greedy Policy)：找出無效頁 (Invalid Pages) 最多的區塊
    int32_t victim_block = -1;
    uint32_t max_invalid_count = 0;

    for (uint32_t b = 0; b < BLOCK_NUM; b++) {
        if ((int32_t)b == active_block) continue; // 不回收目前正寫入的活躍區塊

        uint32_t invalid_count = 0;
        for (uint32_t p = 0; p < PAGES_PER_BLOCK; p++) {
            if (nand_get_page_state(b, p) == PAGE_INVALID) {
                invalid_count++;
            }
        }

        if (invalid_count > max_invalid_count) {
            max_invalid_count = invalid_count;
            victim_block = (int32_t)b;
        }
    }

    if (victim_block == -1) {
        printf("[FTL GC WARNING] 無法找到具有無效頁的區塊！\n");
        return false;
    }

    printf(">>> [FTL GC] 鎖定受害者 Block %d (內含 %u 個無效垃圾頁)！\n",
           victim_block, max_invalid_count);

    // 2. 搬移有效頁 (Valid Page Relocation) -> 寫入放大 (WAF) 的真正來源！
    for (uint32_t p = 0; p < PAGES_PER_BLOCK; p++) {
        if (nand_get_page_state((uint32_t)victim_block, p) == PAGE_VALID) {
            int32_t lba = nand_get_page_lba((uint32_t)victim_block, p);
            uint8_t temp_buf[PAGE_DATA_SIZE];
            nand_read_page((uint32_t)victim_block, p, temp_buf);

            // 確保有足夠的寫入位置
            if (active_page >= (int32_t)PAGES_PER_BLOCK) {
                active_block = find_free_block();
                active_page = 0;
            }

            printf("    [GC 搬遷] LBA %02d: 從舊位置 (Block %d, Page %u) 搬遷至新位置 (Block %d, Page %d)\n",
                   lba, victim_block, p, active_block, active_page);

            nand_program_page((uint32_t)active_block, (uint32_t)active_page, temp_buf, lba);

            // 更新 L2P 位址對映表！
            l2p_table[lba].block = active_block;
            l2p_table[lba].page = active_page;

            active_page++;
        }
    }

    // 3. 整塊抹除受害者區塊 (Erase Victim Block)
    printf(">>> [FTL GC] 抹除受害者 Block %d，轉化為全新 Free Block！\n", victim_block);
    nand_erase_block((uint32_t)victim_block);

    return true;
}

bool ftl_write(uint32_t lba, const uint8_t *data) {
    if (lba >= LOGICAL_LBA_NUM || data == NULL) {
        return false;
    }

    ftl_stats.host_write_count++;

    // 1. 異地更新 (Out-of-Place Update) 判定：
    // 若此 LBA 原本已有映射，將舊的實體 Page 標記為 INVALID
    if (l2p_table[lba].block != -1) {
        uint32_t old_b = (uint32_t)l2p_table[lba].block;
        uint32_t old_p = (uint32_t)l2p_table[lba].page;
        nand_set_page_state(old_b, old_p, PAGE_INVALID);
    }

    // 2. 檢查活躍區塊容量與預留空間 (OP / GC 觸發門檻)
    if (active_block == -1 || active_page >= (int32_t)PAGES_PER_BLOCK) {
        // 若剩餘空白區塊不足 (<= 1 個 Free Block)，提前觸發 GC 釋放空間！
        if (count_free_blocks() <= 1) {
            ftl_trigger_gc();
        }

        active_block = find_free_block();
        active_page = 0;
    }

    // 3. 寫入全新實體 Page
    nand_program_page((uint32_t)active_block, (uint32_t)active_page, data, (int32_t)lba);

    // 4. 更新 L2P Mapping Table
    l2p_table[lba].block = active_block;
    l2p_table[lba].page = active_page;

    active_page++;
    return true;
}

bool ftl_read(uint32_t lba, uint8_t *data_out) {
    if (lba >= LOGICAL_LBA_NUM || data_out == NULL) {
        return false;
    }

    ftl_stats.host_read_count++;

    if (l2p_table[lba].block == -1) {
        memset(data_out, 0xFF, PAGE_DATA_SIZE);
        return true;
    }

    return nand_read_page((uint32_t)l2p_table[lba].block,
                           (uint32_t)l2p_table[lba].page,
                           data_out);
}

bool ftl_trim(uint32_t lba) {
    if (lba >= LOGICAL_LBA_NUM) {
        return false;
    }

    if (l2p_table[lba].block != -1) {
        nand_set_page_state((uint32_t)l2p_table[lba].block,
                            (uint32_t)l2p_table[lba].page,
                            PAGE_INVALID);
        l2p_table[lba].block = -1;
        l2p_table[lba].page = -1;
        printf("[FTL TRIM] LBA %02d 標記刪除，實體 Page 轉為 INVALID！\n", lba);
        return true;
    }
    return false;
}

double ftl_get_waf(void) {
    if (ftl_stats.host_write_count == 0) return 1.0;
    NandStats_t nstats = nand_get_stats();
    return (double)nstats.program_count / (double)ftl_stats.host_write_count;
}

void ftl_print_l2p_table(void) {
    printf("+-------+-------------------------+\n");
    printf("|  LBA  | Mapped PBA (Block, Page)|\n");
    printf("+-------+-------------------------+\n");
    for (uint32_t i = 0; i < LOGICAL_LBA_NUM; i++) {
        if (l2p_table[i].block == -1) {
            printf("| LBA%02d |         Unmapped        |\n", i);
        } else {
            printf("| LBA%02d |     Block %d, Page %d     |\n",
                   i, l2p_table[i].block, l2p_table[i].page);
        }
    }
    printf("+-------+-------------------------+\n");
}

void ftl_print_metrics(void) {
    NandStats_t nstats = nand_get_stats();
    printf("====================================================\n");
    printf("              FTL 性能與寫入放大統計 (WAF)          \n");
    printf("====================================================\n");
    printf("Host 請求寫入次數 (Host Writes) : %u Pages\n", ftl_stats.host_write_count);
    printf("NAND 實體寫入次數 (NAND Programs): %u Pages\n", nstats.program_count);
    printf("NAND 實體抹除次數 (NAND Erases)  : %u Blocks\n", nstats.erase_count);
    printf("GC 觸發次數       (GC Triggers)  : %u 次\n", ftl_stats.gc_trigger_count);
    printf("----------------------------------------------------\n");
    printf("寫入放大率 (WAF = NAND / Host)   : %.2f\n", ftl_get_waf());
    printf("====================================================\n");
}
