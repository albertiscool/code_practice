#ifndef FTL_H
#define FTL_H

#include "nand_flash.h"
#include <stdint.h>
#include <stdbool.h>

// 邏輯位址總量 (Host 可見的 LBA 數量)
#define LOGICAL_LBA_NUM   8U

// 實體位址定義 (PBA)
typedef struct {
    int32_t block; // -1 代表尚未映射 (Unmapped)
    int32_t page;
} Pba_t;

// FTL 核心統計
typedef struct {
    uint32_t host_write_count; // 主機要求寫入的 Page 數
    uint32_t host_read_count;  // 主機要求讀取的 Page 數
    uint32_t gc_trigger_count; // GC 觸發次數
} FtlStats_t;

// FTL 核心 API
void ftl_init(void);
bool ftl_write(uint32_t lba, const uint8_t *data);
bool ftl_read(uint32_t lba, uint8_t *data_out);
bool ftl_trim(uint32_t lba);
bool ftl_trigger_gc(void);
double ftl_get_waf(void);
void ftl_print_l2p_table(void);
void ftl_print_metrics(void);

#endif // FTL_H
