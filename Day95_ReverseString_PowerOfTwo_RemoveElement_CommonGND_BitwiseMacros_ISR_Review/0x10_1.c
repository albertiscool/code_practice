#include <stdio.h>
#include <stdint.h>

#define SET_BIT(reg, n)    ((reg) |= (1U << (n)))
#define CLEAR_BIT(reg, n)  ((reg) &= ~(1U << (n)))
#define TOGGLE_BIT(reg, n) ((reg) ^= (1U << (n)))
#define CHECK_BIT(reg, n)  (((reg) >> (n)) & (1U))

int main(void) {
    uint32_t reg = 0x00000000;

    // 1. 測試 SET_BIT
    SET_BIT(reg, 3);
    printf("SET_BIT(reg, 3)    -> 0x%08X (預期: 0x00000008)\n", reg);

    // 2. 測試 CHECK_BIT
    printf("CHECK_BIT(reg, 3)  -> %u (預期: 1)\n", CHECK_BIT(reg, 3));
    printf("CHECK_BIT(reg, 2)  -> %u (預期: 0)\n", CHECK_BIT(reg, 2));

    // 3. 測試 TOGGLE_BIT
    TOGGLE_BIT(reg, 3);
    printf("TOGGLE_BIT(reg, 3) -> 0x%08X (預期: 0x00000000, 翻轉回 0)\n", reg);

    // 4. 測試 CLEAR_BIT
    reg = 0xFFFFFFFF;
    CLEAR_BIT(reg, 4);
    printf("CLEAR_BIT(reg, 4)  -> 0x%08X (預期: 0xFFFFFFEF)\n", reg);

    // 5. 測試第 31 位元 (MSB 邊界與 1U 驗證)
    reg = 0;
    SET_BIT(reg, 31);
    printf("SET_BIT(reg, 31)   -> 0x%08X (預期: 0x80000000)\n", reg);
    printf("CHECK_BIT(reg, 31) -> %u (預期: 1)\n", CHECK_BIT(reg, 31));

    return 0;
}