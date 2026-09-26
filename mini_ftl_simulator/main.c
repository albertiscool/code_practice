#include "nand_flash.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    printf("====================================================\n");
    printf("  【Mini FTL 專案】第一關：NAND Flash 物理層單元測試\n");
    printf("====================================================\n\n");

    // 1. 測試初始化
    printf("1. 執行 nand_init()...\n");
    nand_init();
    printf("初始 NAND 佈局 (預期全部為 [  F  ] 且 Erase 為 0)：\n");
    nand_print_layout();
    printf("\n");

    // 2. 測試寫入 (Program)
    printf("2. 寫入資料到 Block 0, Page 0 (LBA 0)...\n");
    uint8_t write_data[PAGE_DATA_SIZE] = "Hello_NAND_Flash_Simulation!";
    bool prog_ok = nand_program_page(0, 0, write_data, 0);
    printf("寫入結果: %s\n", prog_ok ? "PASS (成功)" : "FAIL (失敗)");

    printf("寫入後佈局 (預期 Block 0 Page 0 為 [V:L00])：\n");
    nand_print_layout();
    printf("\n");

    // 3. 測試讀取 (Read)
    printf("3. 讀取 Block 0, Page 0...\n");
    uint8_t read_buf[PAGE_DATA_SIZE] = {0};
    bool read_ok = nand_read_page(0, 0, read_buf);
    if (read_ok && strcmp((char *)read_buf, (char *)write_data) == 0) {
        printf("讀取結果: PASS (內容相符 -> \"%s\")\n", (char *)read_buf);
    } else {
        printf("讀取結果: FAIL (內容不符或讀取失敗)\n");
    }
    printf("\n");

    // 4. 測試 NAND 核心物理限制：未抹除前禁止覆寫 (Erase-before-Write)
    printf("4. 測試物理鐵律：試圖覆寫非 FREE 狀態的 Block 0, Page 0...\n");
    uint8_t overwrite_data[PAGE_DATA_SIZE] = "Illegal_Overwrite!";
    bool illegal_prog = nand_program_page(0, 0, overwrite_data, 1);
    printf("非法覆寫防禦測試: %s (預期 FAIL 拒絕覆寫)\n\n",
           !illegal_prog ? "PASS (成功攔截非法覆寫！)" : "FAIL (未遵循 NAND 特性！)");

    // 5. 測試區塊抹除 (Erase Block)
    printf("5. 抹除 Block 0...\n");
    bool erase_ok = nand_erase_block(0);
    printf("抹除結果: %s\n", erase_ok ? "PASS" : "FAIL");
    printf("抹除後佈局 (預期 Block 0 重新變回 [  F  ]，Erase 變為 1)：\n");
    nand_print_layout();
    printf("\n");

    printf("====================================================\n");
    printf("第一關測試完畢！若全數 PASS，代表物理層已完全達標！\n");
    printf("====================================================\n");

    return 0;
}
