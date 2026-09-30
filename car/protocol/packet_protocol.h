/**
 * ============================================================================
 * 🛰️ 【雙核心跨平台二進位通訊協議定義】 (Binary Packet Protocol Definition)
 * ============================================================================
 * 適用平台：
 *   - 上層發送端：樹莓派 4B (64-bit ARM Linux / Python / C++)
 *   - 下層接收端：Arduino Uno/Nano (8-bit AVR ATmega328P)
 * 
 * 核心特性：
 *   1. 同步特徵碼 (Sync Header): 0xAA 0x55 (徹底抗雜訊、防封包撕裂)
 *   2. 嚴格 1-Byte 對齊: 杜絕 64-bit 與 8-bit 之間的 struct padding 記憶體陷阱
 *   3. 零字串解析開銷: 速度比 atoi/sscanf 快 10 倍以上
 * ============================================================================
 */

#ifndef PACKET_PROTOCOL_H
#define PACKET_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

#define PROTOCOL_SYNC_BYTE1   0xAA
#define PROTOCOL_SYNC_BYTE2   0x55
#define PROTOCOL_MAX_PAYLOAD  32

// 封包指令類型 (Message ID)
typedef enum {
    MSG_ID_HEARTBEAT   = 0x01, // 💓 心跳保活 (無 Payload)
    MSG_ID_MOTION_CMD  = 0x02, // 🚗 差速運動控制 (Payload: MotionPayload_t)
    MSG_ID_ATTACK_CMD  = 0x03, // ⚔️ 伺服刺氣球攻擊 (Payload: AttackPayload_t)
    MSG_ID_EMERGENCY   = 0x04, // 🛑 緊急動態煞車 (無 Payload)
    MSG_ID_TELEMETRY   = 0x81  // 📊 Arduino 回傳遙測狀態 (Payload: TelemetryPayload_t)
} MsgId_t;

// ============================================================================
// ⚠️ 關鍵防護：強制編譯器以 1-Byte 對齊 (Packed Struct)
// 徹底消除 64-bit ARM 與 8-bit AVR 之間的 Padding 差異！
// ============================================================================
#pragma pack(push, 1)

// 🚗 運動控制負載 (4 Bytes)
typedef struct {
    int16_t left_speed;   // 左輪 PWM 速度 (-255 ~ +255，負數為倒車)
    int16_t right_speed;  // 右輪 PWM 速度 (-255 ~ +255)
} MotionPayload_t;

// ⚔️ 攻擊控制負載 (2 Bytes)
typedef struct {
    uint8_t attack_mode;  // 1: 單次揮刀, 2: 倒退補刀
    uint8_t servo_angle;  // 目標揮刀角度 (0~180 度)
} AttackPayload_t;

// 📊 Arduino 回報遙測負載 (6 Bytes)
typedef struct {
    uint16_t battery_mv;  // 電池電壓 (mV)
    uint8_t  failsafe_on; // 0: 正常, 1: 處於 Failsafe 急煞狀態
    uint8_t  reserved;    // 保留位元
    int16_t  current_speed; // 當前估計速度
} TelemetryPayload_t;

// 📦 完整封包結構體框架
typedef struct {
    uint8_t header1;     // 固定 0xAA
    uint8_t header2;     // 固定 0x55
    uint8_t msg_id;      // 指令編號 (MsgId_t)
    uint8_t payload_len; // 負載長度 (0 ~ 32)
    uint8_t payload[PROTOCOL_MAX_PAYLOAD]; // 實際資料
    uint8_t checksum;    // 累積校驗和 (XOR Checksum)
} Packet_t;

#pragma pack(pop)

/**
 * 簡易快速校驗和計算 (XOR Checksum)
 * 涵蓋 msg_id, payload_len 以及所有 payload bytes
 */
static inline uint8_t CalculateChecksum(uint8_t msg_id, uint8_t len, const uint8_t* payload) {
    uint8_t csum = msg_id ^ len;
    for (uint8_t i = 0; i < len; i++) {
        csum ^= payload[i];
    }
    return csum;
}

#endif // PACKET_PROTOCOL_H
