#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// 主題【C 語言底層記憶體：大小端 (Endianness) 探測與 32-bit 位元反轉巨集】
// ============================================================================

/* -------------------------------------------------------------
 * 1. 使用 union 判斷大小端 (你所設計的架構)
 * ------------------------------------------------------------- */
union EndianTest {
    uint32_t b;
    uint8_t c;
};

bool isLittleEndian_Union(void) {
    union EndianTest A;
    A.b = 0x10; // 0x00000010

    // union 共用起始位址 (Offset 0)
    // 小端序 (Little-Endian): 低位元組 (0x10) 放在低位址 (Offset 0)
    // 大端序 (Big-Endian):    高位元組 (0x00) 放在低位址 (Offset 0)
    if (A.c == 0x10) {
        return true;  // 是 Little-Endian
    } else {
        return false; // 是 Big-Endian
    }
}

/* -------------------------------------------------------------
 * 2. 使用指標轉型 (Pointer Casting) 判斷大小端 (對照寫法)
 * ------------------------------------------------------------- */
bool isLittleEndian_Pointer(void) {
    uint32_t x = 0x01;
    uint8_t *p = (uint8_t *)&x;
    return (*p == 0x01); // 指向第一個 Byte，若為 1 則是小端序
}

/* -------------------------------------------------------------
 * 3. 純位元操作之 32-bit 大小端反轉巨集 (Macro)
 * ------------------------------------------------------------- */
#define SWAP32(n) ( (((uint32_t)(n) & 0x000000FFU) << 24) | \
                    (((uint32_t)(n) & 0x0000FF00U) <<  8) | \
                    (((uint32_t)(n) & 0x00FF0000U) >>  8) | \
                    (((uint32_t)(n) & 0xFF000000U) >> 24) )

/*
 * ============================================================================
 * 【原始實作紀錄與盲點備忘】
 * 原始想法：
 *   union a { uint32_t b; uint8_t c; };
 *   union a A;
 *   A.b = 0x10;
 *   if (A.c == 0x10) { // is big_endian } else { // is little_endian }
 *
 * 盲點分析：
 *   剛好記反了！
 *   記憶口訣：「小端在低位」。
 *   A.c 讀取的是最低位址 (Offset 0)。
 *   當讀出 0x10 (較小的低位元組) 時，代表它把小的放在低位，因此是【小端序 Little-Endian】！
 * ============================================================================
 */

int main(void) {
    printf("===== 1. 執行時期大小端 (Endianness) 探測 =====\n");
    if (isLittleEndian_Union()) {
        printf("[Union 檢測] 當前 CPU 架構為: Little-Endian (小端序 - x86/ARM預設)\n");
    } else {
        printf("[Union 檢測] 當前 CPU 架構為: Big-Endian (大端序)\n");
    }

    if (isLittleEndian_Pointer()) {
        printf("[Pointer 檢測] 當前 CPU 架構為: Little-Endian (小端序)\n");
    } else {
        printf("[Pointer 檢測] 當前 CPU 架構為: Big-Endian (大端序)\n");
    }

    printf("\n===== 2. 32-bit 整數大小端反轉 (SWAP32) 測試 =====\n");
    uint32_t original = 0x12345678;
    uint32_t swapped  = SWAP32(original);
    uint32_t restored = SWAP32(swapped);

    printf("原始數值: 0x%08X\n", original);
    printf("反轉數值: 0x%08X (預期: 0x78563412)\n", swapped);
    printf("再次反轉: 0x%08X (恢復原值)\n", restored);

    return 0;
}
