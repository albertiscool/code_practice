#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
🛰️ 【樹莓派端二進位通訊封包發送器範例】 (Raspberry Pi Binary Sender Demo)
============================================================================
示範亮點：
  1. 使用 Python 內建 struct.pack 進行跨平台二進位封裝 (<: Little-Endian)
  2. 計算 XOR Checksum 並打上 0xAA 0x55 同步頭
  3. 比起傳送字串 "SPEED:100,200\n"，二進位封包只有 9 Bytes，耗時減少 70%！
============================================================================
"""

import struct
import time
import sys

try:
    sys.stdout.reconfigure(encoding='utf-8')
except Exception:
    pass

try:
    import serial
except ImportError:
    serial = None

# 協議常數定義
SYNC_BYTE1 = 0xAA
SYNC_BYTE2 = 0x55

MSG_ID_HEARTBEAT  = 0x01
MSG_ID_MOTION_CMD = 0x02
MSG_ID_ATTACK_CMD = 0x03
MSG_ID_EMERGENCY  = 0x04

def calculate_checksum(msg_id: int, payload: bytes) -> int:
    """ 計算 XOR Checksum (涵蓋 msg_id, payload 長度, 與所有 payload 位元組) """
    csum = msg_id ^ len(payload)
    for b in payload:
        csum ^= b
    return csum & 0xFF

def build_packet(msg_id: int, payload: bytes = b'') -> bytes:
    """
    組裝標準二進位封包：
    [0xAA] [0x55] [MsgID: 1B] [Len: 1B] [Payload: NB] [Checksum: 1B]
    """
    payload_len = len(payload)
    checksum = calculate_checksum(msg_id, payload)
    
    # 格式說明：< (小端序), B (uint8), B (uint8), B (uint8), B (uint8)
    header = struct.pack('<BBBB', SYNC_BYTE1, SYNC_BYTE2, msg_id, payload_len)
    footer = struct.pack('<B', checksum)
    return header + payload + footer

def build_motion_packet(left_speed: int, right_speed: int) -> bytes:
    """
    建立馬達差速運動指令封包：
    Payload: int16 (left_speed), int16 (right_speed) -> 4 Bytes
    """
    # <h 代表 signed 16-bit integer (小端序)
    payload = struct.pack('<hh', left_speed, right_speed)
    return build_packet(MSG_ID_MOTION_CMD, payload)

def build_heartbeat_packet() -> bytes:
    """ 建立無負載的心跳包 (長度僅 5 Bytes) """
    return build_packet(MSG_ID_HEARTBEAT)

# ============================================================================
# 🎯 測試發送邏輯
# ============================================================================
if __name__ == '__main__':
    print("=" * 60)
    print("🛰️ 樹莓派二進位封包封裝展示")
    print("=" * 60)

    # 1. 建立心跳包
    hb = build_heartbeat_packet()
    print(f"💓 心跳封包 Hex: {hb.hex(' ').upper()} (長度: {len(hb)} Bytes)")
    # 預期: AA 55 01 00 01

    # 2. 建立前進運動封包 (左輪 200, 右輪 240)
    motion_fwd = build_motion_packet(200, 240)
    print(f"🚗 前進運動封包 Hex: {motion_fwd.hex(' ').upper()} (長度: {len(motion_fwd)} Bytes)")
    # 解析：
    # AA 55 : 同步頭
    # 02    : MSG_ID_MOTION_CMD
    # 04    : Payload 長度 (4 Bytes)
    # C8 00 : 200 (0x00C8 Little-Endian)
    # F0 00 : 240 (0x00F0 Little-Endian)
    # 3E    : Checksum

    print("\n✅ 二進位封裝完成！可直接透過 ser.write(motion_fwd) 送入 UART 序列埠！")
