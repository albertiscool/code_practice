/**
 * ============================================================================
 * 🔄 【二進位通訊封包狀態機解析器】 (Binary Protocol FSM Parser)
 * ============================================================================
 * 設計理念：
 *   1. 逐 Byte 解析 (Non-blocking): 完美適應中斷或 loop() 輪詢
 *   2. 狀態機嚴密防護: 若通訊受到 EMI 干擾中斷，自動回到等待 Header 狀態，永不死鎖
 *   3. 零動態配置 (Zero Malloc): 記憶體消耗嚴格固定，適合嵌入式微控制器
 * ============================================================================
 */

#ifndef FSM_PARSER_H
#define FSM_PARSER_H

#include "packet_protocol.h"

// 狀態機運作狀態
typedef enum {
    FSM_STATE_WAIT_HEADER1 = 0, // 等待第 1 個同步碼 0xAA
    FSM_STATE_WAIT_HEADER2,     // 等待第 2 個同步碼 0x55
    FSM_STATE_WAIT_MSG_ID,      // 等待指令編號 MsgID
    FSM_STATE_WAIT_LENGTH,      // 等待資料長度 Length
    FSM_STATE_WAIT_PAYLOAD,     // 接收 Payload 負載資料
    FSM_STATE_WAIT_CHECKSUM     // 校驗 Checksum
} FsmState_t;

// 解析器上下文結構體
typedef struct {
    FsmState_t state;
    uint8_t    msg_id;
    uint8_t    payload_len;
    uint8_t    payload_index;
    uint8_t    payload_buffer[PROTOCOL_MAX_PAYLOAD];
    uint8_t    received_checksum;
} FsmParser_t;

// 封包接收成功回呼函式原型 (Callback)
typedef void (*PacketCallback_t)(uint8_t msg_id, const uint8_t* payload, uint8_t len);

// 初始化解析器
void FsmParser_Init(FsmParser_t* parser);

// 核心處理：將序列埠收到的 1 個 Byte 餵入狀態機
// 若成功湊滿且校驗通過一個合法封包，回傳 true
bool FsmParser_FeedByte(FsmParser_t* parser, uint8_t byte, PacketCallback_t callback);

#endif // FSM_PARSER_H
