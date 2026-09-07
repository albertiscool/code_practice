#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
import cv2
import time
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from ultralytics import YOLO

class BalloonTrackerNode:
    def __init__(self):
        rospy.init_node('balloon_tracker_node', anonymous=True)
        
        # 1. 讀取 ROS 參數 (可從 launch 檔修改模型路徑與相機 Topic)
        model_path = rospy.get_param('~model_path', 'best.onnx')
        camera_topic = rospy.get_param('~camera_topic', '/usb_cam/image_raw')
        
        # 2. 初始化 YOLO 與 CvBridge
        self.model = YOLO(model_path)
        self.bridge = CvBridge()
        self.RED_BALLOON_CLS = 1
        
        # 3. 建立 Publisher (將控制指令發布到 /car_cmd Topic)
        self.cmd_pub = rospy.Publisher('/car_cmd', String, queue_size=10)
        
        # 4. 建立 Subscriber (訂閱相機影像)
        self.image_sub = rospy.Subscriber(camera_topic, Image, self.image_callback)
        
        # 控制發送頻率的計時器 (限制最快 0.5 秒處理一次，避免塞爆 Serial)
        self.last_proc_time = time.time()
        self.proc_interval = 0.5 

        rospy.loginfo("🚀 【YOLO ONNX ROS 節點】已成功啟動，監聽 Topic: %s", camera_topic)

    def image_callback(self, data):
        current_time = time.time()
        # 限制處理頻率，達到 time.sleep(0.5) 的效果，同時不卡死 ROS 接收
        if current_time - self.last_proc_time < self.proc_interval:
            return
            
        try:
            # 將 ROS Image 訊息轉換為 OpenCV 格式
            frame = self.bridge.imgmsg_to_cv2(data, "bgr8")
        except CvBridgeError as e:
            rospy.logerr("CvBridge 轉換失敗: %s", e)
            return

        self.last_proc_time = current_time

        # 🎯 強制縮小畫面至 320x240
        frame = cv2.resize(frame, (320, 240))
        
        # ⚡ 執行 YOLO 推論
        results = self.model(frame, imgsz=320, stream=True, verbose=False)
        
        red_targets = []    
        other_targets = []  
        
        for r in results:
            for box in r.boxes:
                cls_id = int(box.cls[0])
                x1, y1, x2, y2 = map(int, box.xyxy[0])
                area = (x2 - x1) * (y2 - y1)
                
                target_info = {
                    'box': (x1, y1, x2, y2),
                    'area': area,
                    'center_x': int((x1 + x2) / 2),
                    'center_y': int((y1 + y2) / 2)
                }
                
                if cls_id == self.RED_BALLOON_CLS:
                    red_targets.append(target_info)
                else:
                    other_targets.append(target_info)

        final_target = None
        is_red = False
        if len(red_targets) > 0:
            final_target = max(red_targets, key=lambda x: x['area'])
            is_red = True
        elif len(other_targets) > 0:
            final_target = max(other_targets, key=lambda x: x['area'])

        # 做出決策並透過 ROS Topic 發布
        cmd_msg = String()
        
        if final_target is not None:
            bx = final_target['center_x']
            largest_area = final_target['area']
            
            if bx < 50:
                cmd_msg.data = 'L'
                rospy.loginfo("⬅️ 決策：傳送 'L'")
            elif bx > 270:
                cmd_msg.data = 'R'
                rospy.loginfo("➡️ 決策：傳送 'R'")
            else:
                if largest_area > 37500:  
                    cmd_msg.data = 'A'
                    rospy.loginfo("⚔️ 決策：鎖定目標 -> 傳送 'A' 斬殺！")
                else:
                    cmd_msg.data = 'F'
                    rospy.loginfo("⬆️ 決策：傳送 'F'")
        else:
            cmd_msg.data = 'W'
            rospy.loginfo("🔍 決策：沒看到氣球 -> 傳送 'W' 搜尋")

        # 發布指令
        self.cmd_pub.publish(cmd_msg)

if __name__ == '__main__':
    try:
        node = BalloonTrackerNode()
        rospy.spin() # 保持節點運行
    except rospy.ROSInterruptException:
        pass