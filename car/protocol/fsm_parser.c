#include "fsm_parser.h"
#include <string.h>

void FsmParser_Init(FsmParser_t* parser) {
    if (parser == NULL) return;
    parser->state = FSM_STATE_WAIT_HEADER1;
    parser->msg_id = 0;
    parser->payload_len = 0;
    parser->payload_index = 0;
    parser->received_checksum = 0;
    memset(parser->payload_buffer, 0, sizeof(parser->payload_buffer));
}

bool FsmParser_FeedByte(FsmParser_t* parser, uint8_t byte, PacketCallback_t callback) {
    if (parser == NULL) return false;

    switch (parser->state) {
        case FSM_STATE_WAIT_HEADER1:
            if (byte == PROTOCOL_SYNC_BYTE1) { // 命中 0xAA
                parser->state = FSM_STATE_WAIT_HEADER2;
            }
            break;

        case FSM_STATE_WAIT_HEADER2:
            if (byte == PROTOCOL_SYNC_BYTE2) { // 命中 0x55
                parser->state = FSM_STATE_WAIT_MSG_ID;
            } else if (byte == PROTOCOL_SYNC_BYTE1) {
                // 若連收兩個 0xAA，維持在等待 HEADER2
                parser->state = FSM_STATE_WAIT_HEADER2;
            } else {
                // 雜訊干擾，退回初始狀態
                parser->state = FSM_STATE_WAIT_HEADER1;
            }
            break;

        case FSM_STATE_WAIT_MSG_ID:
            parser->msg_id = byte;
            parser->state = FSM_STATE_WAIT_LENGTH;
            break;

        case FSM_STATE_WAIT_LENGTH:
            if (byte > PROTOCOL_MAX_PAYLOAD) {
                // 異常長度，防止 Buffer Overflow 漏洞，直接丟棄！
                parser->state = FSM_STATE_WAIT_HEADER1;
            } else {
                parser->payload_len = byte;
                parser->payload_index = 0;
                if (parser->payload_len == 0) {
                    // 無資料負載 (如純心跳)，直接進入 Checksum 校驗
                    parser->state = FSM_STATE_WAIT_CHECKSUM;
                } else {
                    parser->state = FSM_STATE_WAIT_PAYLOAD;
                }
            }
            break;

        case FSM_STATE_WAIT_PAYLOAD:
            parser->payload_buffer[parser->payload_index++] = byte;
            if (parser->payload_index >= parser->payload_len) {
                // 負載接收完畢，進入校驗和驗證
                parser->state = FSM_STATE_WAIT_CHECKSUM;
            }
            break;

        case FSM_STATE_WAIT_CHECKSUM:
            parser->received_checksum = byte;
            // 計算本地校驗和進行比對
            uint8_t expected_csum = CalculateChecksum(
                parser->msg_id, 
                parser->payload_len, 
                parser->payload_buffer
            );

            bool packet_valid = false;
            if (parser->received_checksum == expected_csum) {
                // 封包校驗成功！呼叫業務層回呼函式執行動作
                if (callback != NULL) {
                    callback(parser->msg_id, parser->payload_buffer, parser->payload_len);
                }
                packet_valid = true;
            }

            // 無論校驗成功或失敗，狀態機都自動重置，準備接收下一個封包
            parser->state = FSM_STATE_WAIT_HEADER1;
            return packet_valid;

        default:
            parser->state = FSM_STATE_WAIT_HEADER1;
            break;
    }

    return false;
}
