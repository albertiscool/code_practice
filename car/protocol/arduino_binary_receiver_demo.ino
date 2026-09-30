/**
 * ============================================================================
 * 🤖 【Arduino 端的工業級二進位協議接收範例】 (Binary Receiver Demo)
 * ============================================================================
 * 示範亮點：
 *   1. 徹底取代 'F', 'S' 或 "SPEED:100" 的字串寫法
 *   2. 透過 FsmParser 逐 Byte 解包，完全不需等待結尾換行符號
 *   3. 零動態記憶體 (0 malloc)，抗雜訊與重置對齊能力極強
 * ============================================================================
 */

#include "packet_protocol.h"
#include "fsm_parser.h"

// 全域狀態機解析器上下文
static FsmParser_t rx_parser;

// 模擬馬達控制腳位
const byte LEFT_PWM = 10;
const byte RIGHT_PWM = 5;
const int ledPin = A5;

// 通訊看門狗時間戳
unsigned long lastHeartbeat = 0;

/**
 * 📦 業務回呼函式：當狀態機成功校驗完一個合法封包時觸發
 */
void HandlePacket(uint8_t msg_id, const uint8_t* payload, uint8_t len) {
    // 收到合法封包，立即餵狗！
    lastHeartbeat = millis();

    switch (msg_id) {
        case MSG_ID_HEARTBEAT:
            // 收到心跳包，維持存活
            digitalWrite(ledPin, HIGH);
            break;

        case MSG_ID_MOTION_CMD:
            if (len == sizeof(MotionPayload_t)) {
                // 直接將二進位記憶體映射為結構體指標，零拷貝且無須 atoi 解析！
                const MotionPayload_t* motion = (const MotionPayload_t*)payload;
                
                // 驅動左右馬達 PWM
                // (注意：實際使用時需根據正負號控制方向腳位，此處簡化示意)
                analogWrite(LEFT_PWM, constrain(abs(motion->left_speed), 0, 255));
                analogWrite(RIGHT_PWM, constrain(abs(motion->right_speed), 0, 255));
            }
            break;

        case MSG_ID_ATTACK_CMD:
            if (len == sizeof(AttackPayload_t)) {
                const AttackPayload_t* atk = (const AttackPayload_t*)payload;
                // 控制伺服馬達揮刀攻擊
                // writeServoAngle(atk->servo_angle);
            }
            break;

        case MSG_ID_EMERGENCY:
            // 緊急主動動態急煞
            analogWrite(LEFT_PWM, 0);
            analogWrite(RIGHT_PWM, 0);
            break;

        default:
            break;
    }
}

void setup() {
    Serial.begin(115200); // 採用高速 115200 鮑率
    pinMode(LEFT_PWM, OUTPUT);
    pinMode(RIGHT_PWM, OUTPUT);
    pinMode(ledPin, OUTPUT);

    // 初始化二進位狀態機解析器
    FsmParser_Init(&rx_parser);
    lastHeartbeat = millis();
}

void loop() {
    // 1. 🛡️ 通訊看門狗監控 (300ms 逾時防暴衝)
    if (millis() - lastHeartbeat > 300) {
        analogWrite(LEFT_PWM, 0);
        analogWrite(RIGHT_PWM, 0);
        digitalWrite(ledPin, LOW);
    }

    // 2. 🔄 非阻塞式讀取序列埠，逐 Byte 餵給狀態機
    while (Serial.available() > 0) {
        uint8_t incoming_byte = (uint8_t)Serial.read();
        FsmParser_FeedByte(&rx_parser, incoming_byte, HandlePacket);
    }
}
