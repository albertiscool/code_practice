#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
import serial
import time
from std_msgs.msg import String

class SerialBridgeNode:
    def __init__(self):
        rospy.init_node('serial_bridge_node', anonymous=True)
        
        # 1. 讀取 ROS 參數（可在 launch 檔靈活修改序列埠與鮑率）
        self.port = rospy.get_param('~port', '/dev/ttyACM0')
        self.baud = rospy.get_param('~baud', 9600)
        
        # 2. 初始化 Serial 連線
        try:
            self.ser = serial.Serial(self.port, self.baud, timeout=1)
            rospy.loginfo("🟢 成功連接 Arduino 序列埠: %s (Baud: %d)", self.port, self.baud)
        except serial.SerialException:
            try:
                self.port = '/dev/ttyUSB0'
                self.ser = serial.Serial(self.port, self.baud, timeout=1)
                rospy.loginfo("🟢 成功連接備用 Arduino 序列埠: %s (Baud: %d)", self.port, self.baud)
            except serial.SerialException:
                rospy.logerr("❌ 無法連接到 Arduino！請檢查 USB 線與權限（ls -l /dev/tty*）")
                self.ser = None

        # 3. 如果序列埠順利開啟，發送開機初始化指令 'X' (對應 Arduino 啟動自動模式)
        if self.ser:
            time.sleep(2)  # 等待 Arduino 重啟完成
            self.ser.write(b'X')
            rospy.loginfo("⚡ 已發送 'X' 指令，開啟 Arduino 自動模式")

        # 4. 訂閱 /car_cmd 決策指令 Topic
        rospy.Subscriber('/car_cmd', String, self.cmd_callback)
        
        # 5. 🛡️ 【工業級 Failsafe 心跳定時器 (Heartbeat Generator)】
        # 以 10 Hz (每 100ms) 定期發送心跳字元 'H' 給 Arduino，維持通訊看門狗存活
        # 一旦 ROS 節點卡死、當機或傳輸線斷裂，心跳即刻停止，Arduino 300ms 內觸發電磁急煞！
        self.heartbeat_timer = rospy.Timer(rospy.Duration(0.1), self.heartbeat_callback)
        
        rospy.loginfo("🚀 【ROS 序列埠橋接節點】已啟動，心跳看門狗協定已掛載 (10 Hz)...")

    def heartbeat_callback(self, event):
        """ 週期性發送心跳封包，維持 Arduino 端通訊看門狗存活 """
        if self.ser and self.ser.is_open:
            try:
                self.ser.write(b'H')
            except serial.SerialException as e:
                rospy.logerr_throttle(2, "❌ 發送心跳失敗，序列埠可能斷線: %s", str(e))

    def cmd_callback(self, msg):
        """ 當接收到 YOLO 節點發布的控制指令時，立刻轉傳給 Arduino """
        if self.ser and self.ser.is_open:
            cmd_str = msg.data
            
            # 將 Python 字串轉成 byte 格式並發送
            encoded_cmd = cmd_str.encode('utf-8')
            self.ser.write(encoded_cmd)
            
            rospy.logdebug("📥 ROS 轉傳給 Arduino: '%s'", cmd_str)
        else:
            rospy.logwarn_throttle(2, "⚠️ 序列埠未就緒，無法發送指令: '%s'", msg.data)

    def shutdown_hook(self):
        """ 當關閉 ROS 節點時，立即觸發動態煞車並安全釋放硬體 """
        rospy.loginfo("🔒 正在觸發 Failsafe 急煞並安全關閉序列埠...")
        if self.ser and self.ser.is_open:
            try:
                self.ser.write(b'S')  # 發送主動動態急煞指令
                time.sleep(0.05)
                self.ser.write(b'x')  # 關閉自動模式
                time.sleep(0.05)
                self.ser.close()
            except Exception as e:
                rospy.logwarn("關閉序列埠時發生異常: %s", str(e))
        rospy.loginfo("🔒 序列埠已安全關閉，車體已鎖定煞停")

if __name__ == '__main__':
    try:
        bridge = SerialBridgeNode()
        # 註冊關閉時執行的功能
        rospy.on_shutdown(bridge.shutdown_hook)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass