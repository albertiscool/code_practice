#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

// ============================================================================
// 情境題 1【中斷與暫存器 RMW 競爭危害 vs 硬體原子操作 (BSRR)】
// ============================================================================
typedef struct {
    uint32_t ODR;  // 輸出資料暫存器 (模擬傳統需 RMW 的暫存器)
    uint32_t BSRR; // 位元設定/清除暫存器 (模擬 Write-1-effective 原子暫存器)
} Mock_GPIO_t;

static Mock_GPIO_t g_gpio = { .ODR = 0, .BSRR = 0 };

// 模擬 ARM Cortex-M / STM32 硬體 BSRR 原子寫入邏輯
void write_BSRR(uint32_t bsrr_val) {
    // 低 16 位元：寫 1 則將對應 ODR 位元設為 1 (Set)
    uint16_t set_mask = (uint16_t)(bsrr_val & 0xFFFF);
    // 高 16 位元：寫 1 則將對應 ODR 位元清為 0 (Reset)
    uint16_t reset_mask = (uint16_t)((bsrr_val >> 16) & 0xFFFF);

    // 硬體在單一匯流排週期直接原子生效，不需經歷軟體 LDR -> Modify -> STR
    g_gpio.ODR |= set_mask;
    g_gpio.ODR &= ~((uint32_t)reset_mask);
}

// ============================================================================
// 情境題 2【跨晶片通訊結構體手動序列化 (Serialization) 與靜態斷言】
// ============================================================================
#pragma pack(push, 1)
typedef struct {
    uint8_t  cmd;
    uint32_t timestamp;
    uint16_t sensor_val;
} Packet_t;
#pragma pack(pop)

// 靜態斷言：編譯時期強制檢查封包大小，杜絕跨平台 ABI 對齊不一致！
static_assert(sizeof(Packet_t) == 7, "Error: Packet_t must be exactly 7 bytes!");

// 手動序列化：以固定 Big-Endian (網路位元組序) 打平成 Byte Array，徹底免疫 Padding 與大小端
void serialize_packet(uint8_t *buf, const Packet_t *pkt) {
    buf[0] = pkt->cmd;
    buf[1] = (uint8_t)((pkt->timestamp >> 24) & 0xFF);
    buf[2] = (uint8_t)((pkt->timestamp >> 16) & 0xFF);
    buf[3] = (uint8_t)((pkt->timestamp >> 8)  & 0xFF);
    buf[4] = (uint8_t)((pkt->timestamp)       & 0xFF);
    buf[5] = (uint8_t)((pkt->sensor_val >> 8) & 0xFF);
    buf[6] = (uint8_t)((pkt->sensor_val)      & 0xFF);
}

// 反序列化：還原封包
void deserialize_packet(Packet_t *pkt, const uint8_t *buf) {
    pkt->cmd = buf[0];
    pkt->timestamp = ((uint32_t)buf[1] << 24) |
                     ((uint32_t)buf[2] << 16) |
                     ((uint32_t)buf[3] << 8)  |
                     ((uint32_t)buf[4]);
    pkt->sensor_val = (uint16_t)(((uint16_t)buf[5] << 8) | ((uint16_t)buf[6]));
}

int main(void) {
    printf("=== Day 97: 0x10 戰場一級實戰驗證 ===\n\n");

    // 1. 驗證原子操作 (BSRR)
    printf("--- [情境 1] 模擬 BSRR 原子設定與清除 ---\n");
    // 設定 Pin 3 為 HIGH (寫入低 16 位之 Bit 3)
    write_BSRR(1U << 3);
    printf("BSRR Set Pin 3   -> ODR = 0x%08X (預期: 0x00000008)\n", g_gpio.ODR);
    // 清除 Pin 3 (寫入高 16 位之 Bit 3, 即 1U << (16 + 3))
    write_BSRR(1U << (16 + 3));
    printf("BSRR Reset Pin 3 -> ODR = 0x%08X (預期: 0x00000000)\n\n", g_gpio.ODR);

    // 2. 驗證跨平台序列化
    printf("--- [情境 2] 跨晶片通訊序列化與反序列化 ---\n");
    Packet_t tx_pkt = {
        .cmd = 0xA5,
        .timestamp = 0x12345678,
        .sensor_val = 0xABCD
    };
    uint8_t wire_buf[7];
    serialize_packet(wire_buf, &tx_pkt);

    printf("序列化 Byte 串流: ");
    for (int i = 0; i < 7; i++) printf("0x%02X ", wire_buf[i]);
    printf("\n預期 Byte 串流: 0xA5 0x12 0x34 0x56 0x78 0xAB 0xCD\n");

    Packet_t rx_pkt;
    deserialize_packet(&rx_pkt, wire_buf);
    printf("反序列化還原結果: cmd=0x%02X, timestamp=0x%08X, sensor_val=0x%04X\n",
           rx_pkt.cmd, rx_pkt.timestamp, rx_pkt.sensor_val);
    printf("驗證結果: %s\n", (memcmp(&tx_pkt, &rx_pkt, sizeof(Packet_t)) == 0) ? "100% MATCH (SUCCESS!)" : "MISMATCH");

    return 0;
}
