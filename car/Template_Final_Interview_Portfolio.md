# 🤖 【期末專題面試簡報範本】：基於 ROS 2 與異質雙核心之 Edge AI 自駕車系統
## Heterogeneous Dual-Core Edge AI Autonomous Mobile Robot (AMR) Platform

> **專案定位**：結合 Linux GPOS (Raspberry Pi 4 / ROS 2 / YOLOv8) 與硬即時微控制器之自駕巡檢平台  
> **核心技術**：C/C++、Python、Linux OS、ROS 2 節點拓撲、YOLOv8 物件偵測、POSIX `mmap` 零拷貝架構分析、UART 通訊協定與狀態機

---

## 📑 投影片架構設計（建議 5 頁 PPT）

---

### 🖼️ Slide 1: 異質雙核心系統架構與分工 (Heterogeneous Architecture)

* **設計哲學：高低階雙核心協同**：
  * **高階大腦 (High-Level GPOS - Raspberry Pi 4 / Ubuntu Linux)**：
    負責吃算力的神經網路推理 (YOLOv8)、相機影像串流採集、ROS 2 導航節點決策。
  * **底層小腦 (Low-Level Hard Real-Time MCU - ATmega328P / ARM)**：
    負責微秒級 PWM 馬達差速驅動、舵機轉向、硬即時緊急防撞煞停。
  * **跨板通訊 (Cross-Board Communication)**：全雙工 UART 序列傳輸。

```text
[Pi Camera] ──► [ RPi 4 (Linux / ROS 2 / YOLOv8) ]
                           │ (UART 序列指令)
                           ▼
                    [ Arduino MCU ] ──► [ L298N 馬達 / 舵機 / 感測器 ]
```

---

### 🖼️ Slide 2: 邊緣 AI 物件偵測與 ROS 2 節點設計 (Edge AI & ROS 2 Pipeline)

#### 1. YOLOv8 輕量化神經網路部署
* 部署輕量化 YOLOv8 模型於樹莓派，實現即時道路障礙物、交通標誌與行人偵測。
* 輸出目標物邊界框 (Bounding Box)、類別 (Class) 與置信度 (Confidence)。

#### 2. ROS 2 分散式節點架構 (Node Topology)
* `camera_publisher_node`：採集影像幀並發布影像話題。
* `yolo_detector_node`：訂閱影像，執行推論並發布偵測座標與障礙物距離。
* `navigation_controller_node`：整合視覺資訊，生成線速度與角速度指令 (`geometry_msgs/Twist`)。
* `serial_bridge_node`：將 ROS 2 Twist 指令轉換為底層二進位封包送至 MCU。

---

### 🖼️ Slide 3: 跨板通訊協定設計與可靠性機制 (UART Protocol & Reliability)

#### 1. 自定義二進位幀通訊協定 (Binary Frame Protocol)
為防止字串解析延遲與雜訊干擾，設計緊湊二進位封包：
* **`[0xAA 0x55]`**：2 Bytes 幀同步標頭 (Header)
* **`[Msg_Type]`**：1 Byte 訊息類型 (速度控制 / 避障狀態 / 心跳包)
* **`[Length]`**：1 Byte 資料長度
* **`[Payload]`**：$N$ Bytes 有效載荷 (左右輪目標 PWM / 舵機角度)
* **`[Checksum / CRC]`**：1 Byte 校驗碼 (保障資料完整性)

#### 2. 接收端有限狀態機 (FSM) 解析器
* MCU 端利用中斷配合 Ring Buffer 與狀態機解析，徹底解決串列通訊撕裂與黏包問題。

---

### 🖼️ Slide 4: 重大工程挑戰、除錯實戰與架構優化 (Challenges & Architecture Analysis)

#### 🛠️ 1. 【實際實作驗證】跨板通訊封包校驗與時延除錯 (Implementation & Debugging)
* **問題**：高頻發送控制命令時，偶爾因串口雜訊造成小車動作卡頓。
* **解決**：在封包中引入 Checksum 校驗碼，並加入「通訊超時安全保護機制 (Timeout Failsafe)」——若 500ms 內未收到樹莓派心跳包，MCU 自動強制煞停，確保自駕安全性。

#### 💡 2. 【架構分析與優化提案】POSIX `mmap` 零拷貝影像處理 (Zero-Copy Architecture Proposal)
* **效能瓶頸深入剖析**：
  * 分析傳統 Linux `read()` 系統呼叫在 User Space 與 Kernel Space 之間進行 3 次 `memcpy` 所造成的記憶體頻寬瓶頸（CPU 負載高達 80%）。
* **提出零拷貝架構優化方案**：
  * 提出利用 Linux POSIX **`mmap()` 共享記憶體機制**，將 V4L2 視訊緩衝區直接映射至推論引擎記憶體空間，理論上可達成 **Zero-Copy（零記憶體複製）**，大幅降低 CPU 開銷並提升推論幀率！

---

### 🖼️ Slide 5: 專題總結與技術技能樹 (Summary & Technical Skills)

* **量化成果**：
  * 成功整合 Linux 邊緣運算大腦與硬即時微控制器小腦。
  * 實踐端到端自駕巡檢：相機採集 ➔ YOLO 辨識 ➔ ROS 2 決策 ➔ MCU 馬達控制。
* **技術關鍵字 (Keyword Matrix)**：
  * `Linux / ROS 2`、`C / C++ / Python`、`YOLOv8 Edge AI`、`Cross-Board UART Protocol`、`POSIX mmap (Arch Design)`、`Hardware Real-Time Control`。
