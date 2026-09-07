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
        
        rospy.loginfo("🚀 【ROS 序列埠橋接節點】已啟動，等待轉傳指令...")

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
        """ 當關閉 ROS 節點時，安全停車並關閉序列埠 """
        rospy.loginfo("🔒 正在安全關閉序列埠...")
        if self.ser and self.ser.is_open:
            self.ser.write(b's')  # 發送停車解鎖指令給 Arduino
            self.ser.close()
        rospy.loginfo("🔒 序列埠已安全關閉")

if __name__ == '__main__':
    try:
        bridge = SerialBridgeNode()
        # 註冊關閉時執行的功能
        rospy.on_shutdown(bridge.shutdown_hook)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass