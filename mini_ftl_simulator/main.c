#include "nand_flash.h"
#include "ftl.h"
#include <stdio.h>
#include <string.h>

static void print_separator(const char *title) {
    printf("\n====================================================\n");
    printf("  %s\n", title);
    printf("====================================================\n");
}

int main(void) {
    print_separator("【Mini FTL 固態硬碟主控模擬器】啟動");
    printf("模擬規格：4 Blocks x 4 Pages/Block (共 16 物理頁), 8 個邏輯 LBA\n");

    // 初始化 FTL 與 NAND Flash
    ftl_init();

    // ------------------------------------------------------------------------
    // 階段 1: 循序寫入 LBA 0 ~ 7 (填滿 8 個邏輯空間)
    // ------------------------------------------------------------------------
    print_separator("階段 1: 循序寫入 LBA 0 ~ 7");
    for (uint32_t i = 0; i < LOGICAL_LBA_NUM; i++) {
        uint8_t payload[PAGE_DATA_SIZE];
        snprintf((char *)payload, PAGE_DATA_SIZE, "Data_v1_for_LBA_%u", i);
        ftl_write(i, payload);
    }

    printf("\n[NAND 佈局 - 寫入 8 筆後]：\n");
    nand_print_layout();
    printf("\n[L2P 位址對映表]：\n");
    ftl_print_l2p_table();
    ftl_print_metrics();

    // ------------------------------------------------------------------------
    // 階段 2: 模擬隨機覆寫 (Out-of-Place Update 產生無效垃圾頁)
    // ------------------------------------------------------------------------
    print_separator("階段 2: 覆寫 LBA 0, 1, 2 (產生 Invalid 頁)");
    printf("覆寫 LBA 0, 1, 2，觀察原本舊頁面是否被自動標記為 INVALID...\n");

    for (uint32_t i = 0; i < 3; i++) {
        uint8_t payload[PAGE_DATA_SIZE];
        snprintf((char *)payload, PAGE_DATA_SIZE, "Data_v2_NEW_LBA_%u", i);
        ftl_write(i, payload);
    }

    printf("\n[NAND 佈局 - 覆寫後] (注意觀察 Block 0 的舊頁變為 I:L00~02)：\n");
    nand_print_layout();
    ftl_print_l2p_table();

    // ------------------------------------------------------------------------
    // 階段 3: 空間耗盡，自動引發 Greedy 垃圾回收 (GC)
    // ------------------------------------------------------------------------
    print_separator("階段 3: 持續寫入引發空間不足，自動觸發 GC");
    printf("持續寫入 LBA 3, 4, 5, 6, 7，耗盡剩餘 Free 區塊...\n");

    for (uint32_t i = 3; i < LOGICAL_LBA_NUM; i++) {
        uint8_t payload[PAGE_DATA_SIZE];
        snprintf((char *)payload, PAGE_DATA_SIZE, "Data_v2_NEW_LBA_%u", i);
        ftl_write(i, payload);
    }

    printf("\n[NAND 佈局 - GC 搬遷與抹除後]：\n");
    nand_print_layout();
    ftl_print_l2p_table();
    ftl_print_metrics();

    // ------------------------------------------------------------------------
    // 階段 4: 資料完整性驗證 (Read Verification)
    // ------------------------------------------------------------------------
    print_separator("階段 4: 資料完整性校驗 (Read Verification)");
    printf("讀取 LBA 0 ~ 7，驗證歷經 GC 搬遷後資料依然完全正確：\n");
    bool all_passed = true;
    for (uint32_t i = 0; i < LOGICAL_LBA_NUM; i++) {
        uint8_t read_buf[PAGE_DATA_SIZE];
        char expected[PAGE_DATA_SIZE];
        snprintf(expected, PAGE_DATA_SIZE, "Data_v2_NEW_LBA_%u", i);

        ftl_read(i, read_buf);
        if (strcmp((char *)read_buf, expected) == 0) {
            printf("  [PASS] LBA %02d 讀取成功 -> \"%s\"\n", i, (char *)read_buf);
        } else {
            printf("  [FAIL] LBA %02d 資料不符！預期 \"%s\", 實得 \"%s\"\n",
                   i, expected, (char *)read_buf);
            all_passed = false;
        }
    }

    if (all_passed) {
        printf("\n>>> 恭喜！所有 LBA 資料在 GC 搬遷與抹除後 100%% 保持完整！<<<\n");
    }

    // ------------------------------------------------------------------------
    // 階段 5: TRIM 指令演示
    // ------------------------------------------------------------------------
    print_separator("階段 5: TRIM 指令演示 (主機通知刪除檔案)");
    printf("Host 刪除檔案，發送 TRIM 命令釋放 LBA 6 與 LBA 7...\n");
    ftl_trim(6);
    ftl_trim(7);

    printf("\n[NAND 佈局 - TRIM 執行後] (LBA 6, 7 的實體 Page 直接轉為 INVALID)：\n");
    nand_print_layout();
    ftl_print_l2p_table();

    print_separator("【Mini FTL 固態硬碟模擬器】展示圓滿完成！");
    ftl_print_metrics();

    return 0;
}
