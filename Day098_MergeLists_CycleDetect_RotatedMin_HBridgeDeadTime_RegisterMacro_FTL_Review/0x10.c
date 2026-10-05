#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
================================================================================
Day 98 - 一線 IC 設計廠（群聯、聯詠、慧榮）底層 C 語言與暫存器操作實戰
================================================================================
1. 小題 (A)：指標運算優先權與位移量驗證
2. 小題 (B)：32-bit 暫存器安全讀改寫 (Single Atomic Write) 與 MISRA-C 邊界防禦
================================================================================
*/

// ============================================================================
// 小題 (A) 驗證模組：指標運算子優先權
// ============================================================================
static void test_pointer_precedence(void) {
    printf("--- [Part A] 指標優先權與位移量驗證 ---\n");

    // Case 1: *p++
    {
        uint32_t buf[] = {0x11223344, 0x55667788, 0x99AABBCC};
        uint32_t *p = buf;
        uint32_t val = *p++;
        printf("1. *p++  : val = 0x%08X, *p = 0x%08X, offset = %zd bytes\n",
               val, *p, (uint8_t *)p - (uint8_t *)buf);
    }

    // Case 2: (*p)++
    {
        uint32_t buf[] = {0x11223344, 0x55667788, 0x99AABBCC};
        uint32_t *p = buf;
        uint32_t val = (*p)++;
        printf("2. (*p)++: val = 0x%08X, *p = 0x%08X, offset = %zd bytes\n",
               val, *p, (uint8_t *)p - (uint8_t *)buf);
    }

    // Case 3: *++p
    {
        uint32_t buf[] = {0x11223344, 0x55667788, 0x99AABBCC};
        uint32_t *p = buf;
        uint32_t val = *++p;
        printf("3. *++p  : val = 0x%08X, *p = 0x%08X, offset = %zd bytes\n",
               val, *p, (uint8_t *)p - (uint8_t *)buf);
    }

    // Case 4: ++*p
    {
        uint32_t buf[] = {0x11223344, 0x55667788, 0x99AABBCC};
        uint32_t *p = buf;
        uint32_t val = ++*p;
        printf("4. ++*p  : val = 0x%08X, *p = 0x%08X, offset = %zd bytes\n",
               val, *p, (uint8_t *)p - (uint8_t *)buf);
    }
    printf("\n");
}

// ============================================================================
// 小題 (B) 實作模組：暫存器位元欄位防禦操作 (SYS_CTRL_REG)
// ============================================================================

// 模擬實體硬體暫存器空間
static volatile uint32_t simulated_hardware_register = 0xAAAA55AA; // 預設測試值

// 暫存器存取指標定義
#define SYS_CTRL_REG (simulated_hardware_register)

#define FIELD_SHIFT         (8U)
#define FIELD_WIDTH         (4U)
#define FIELD_MASK          (((1U << FIELD_WIDTH) - 1U) << FIELD_SHIFT) // 0x00000F00

// 寫法 1: 現代韌體最高標準 static inline 函式 (型別安全、零毛刺、邊界截斷防護)
static inline void set_sys_ctrl_val_safe(uint32_t val) {
    uint32_t reg_val = SYS_CTRL_REG;              // 1. 單次讀取硬體
    reg_val &= ~FIELD_MASK;                       // 2. 在 CPU 內部清空目標欄位 [11:8]
    reg_val |= ((val & 0x0FUL) << FIELD_SHIFT);   // 3. 嚴格邊界遮罩截斷並填值
    SYS_CTRL_REG = reg_val;                       // 4. 單次原子寫回，杜絕雙重寫入毛刺！
}

// 寫法 2: 工業級防護巨集 (使用 do-while(0) 杜絕 dangling-else 語法錯誤)
#define SET_SYS_CTRL_VAL_MACRO(val) \
    do { \
        SYS_CTRL_REG = (SYS_CTRL_REG & ~FIELD_MASK) | (((uint32_t)(val) & 0x0FUL) << FIELD_SHIFT); \
    } while (0)

static void test_register_bitfield(void) {
    printf("--- [Part B] 暫存器位元欄位防禦測試 ---\n");

    // 初始值設定: Bit [15:12] = 0x5, Bit [11:8] = 0xA, Bit [7:0] = 0x55
    SYS_CTRL_REG = 0xFFFF5A55;
    printf("Initial REG Value:           0x%08X (Bit [11:8] is 0xA)\n", SYS_CTRL_REG);

    // 測試 1: 正常寫入 0x7
    set_sys_ctrl_val_safe(0x7);
    printf("After writing 0x7:           0x%08X (Bit [11:8] should be 0x7)\n", SYS_CTRL_REG);

    // 測試 2: 邊界溢位攻擊 (傳入 0x1F，超過 4 bits)
    // 預期行為: 0x1F & 0x0F = 0x0F，絕不能踩爛相鄰的 Bit [15:12]！
    set_sys_ctrl_val_safe(0x1F);
    printf("After writing 0x1F (Safety): 0x%08X (Bit [11:8] should be 0xF, [15:12] intact)\n", SYS_CTRL_REG);

    // 測試 3: 巨集版本測試 (寫入 0x3)
    SET_SYS_CTRL_VAL_MACRO(0x3);
    printf("After Macro writing 0x3:     0x%08X (Bit [11:8] should be 0x3)\n", SYS_CTRL_REG);
    printf("\n");
}

int main(void) {
    printf("===============================================================\n");
    printf("   Day 98 實戰: 一線 IC 設計廠底層 C 語言與暫存器操作測試\n");
    printf("===============================================================\n\n");

    test_pointer_precedence();
    test_register_bitfield();

    return 0;
}
