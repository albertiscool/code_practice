# 🚗 科技業技術面試專用簡報：Edge AI 異質雙核心自駕小車平台
## Heterogeneous Dual-Core Autonomous Mobile Robot (AMR) Platform

> **報告人**：楊毅晨 (National Chung Hsing University, EE Master)  
> **專案定位**：結合 Linux GPOS (Edge AI / ROS 2) 與硬即時微控制器 (Hard Real-Time Motion Control) 之邊緣自駕巡檢車平台

---

## 📑 投影片目錄 (Slide Outline)

1. **Slide 1: 專案背景與系統總體架構 (System Architecture)**
2. **Slide 2: 邊緣 AI 視覺與 POSIX `mmap` 零拷貝優化 (Edge AI & Zero-Copy IPC)**
3. **Slide 3: 硬即時運動控制與反電動勢動態制動 (Motion Control & Back-EMF Braking)**
4. **Slide 4: 重大工程挑戰與 Root-Cause 除錯實戰 (Engineering Challenges & Debugging)**
5. **Slide 5: 量化成果指標與核心技術技能樹 (Key Metrics & Technical Skills)**

---

## 🖼️ Slide 1: 專案定位與異質雙核心架構 (System Architecture)

### 💡 設計思維：為什麼採用「異質雙核心 (Heterogeneous Dual-Core)」？
自駕系統面臨兩大矛盾需求：
* **大量運算需求 (Heavy Compute)**：YOLOv8 深度學習物件辨識與路徑規劃（需要 Linux OS 與 GPU/NPU 算力）。
* **硬即時性需求 (Hard Real-Time)**：微秒級 PWM 馬達差速、急煞防撞、感測器採樣（不能容忍 Linux 排程 Jitter）。
* **架構決策**：**RPi 4 (Linux GPOS) 負責大腦決策；MCU (ATmega328P/ARM) 負責小腦運動！**

```mermaid
graph TD
    subgraph "高階大腦運算層 (High-Level GPOS: Raspberry Pi 4 / Linux)"
        Cam[Pi Camera V4L2] -->|POSIX mmap 零拷貝| YOLO[YOLOv8 輕量化神經網路]
        YOLO -->|Bounding Box & Class| Nav[ROS 2 導航與行為狀態機]
        Nav -->|Motion Command / Twist| Protocol[自定義 UART 二進位封包編碼器]
    end

    Protocol <===>|全雙工 UART 115200 (帶 CRC 校驗)| MCU_Bridge[UART 中斷接收狀態機]

    subgraph "底層小腦控制層 (Low-Level Hard Real-Time MCU: ATmega328P)"
        MCU_Bridge --> FSM[底層即時運動控制器]
        IR[5路紅外線循線感測器] -->|ADC / GPIO| PID[閉迴路 PID 差速調速演算法]
        US[HC-SR04 超音波感測器] -->|Timer 輸入捕獲| Safety[硬體級防撞中斷]
        
        PID --> PWM[硬體 Timer PWM 產生器]
        Safety -->|急煞觸發| Brake[反電動勢動態煞車控制器]
        
        PWM --> Driver[L298N 雙 H 橋馬達驅動器]
        Brake --> Driver
        Driver --> Motors[雙輪直流減速馬達 + 差速輪]
    end
```

---

## 🖼️ Slide 2: 邊緣 AI 視覺與 POSIX `mmap` 零拷貝優化 (Edge AI & IPC)

### 💥 核心工程痛點：
邊緣端嵌入式晶片 (RPi 4) 記憶體頻寬有限。傳統相機讀取透過 `read()` 系統呼叫，影像資料需在 **Kernel Space ➔ User Space ➔ OpenCV Buffer ➔ YOLO Input Tensor** 之間經歷三次 `memcpy`，導致 CPU 佔用率飆高至 85%，影像延遲嚴重（僅 12 FPS）。

```text
【傳統方案：三次 memcpy (頻寬瓶頸)】
Camera Driver (Kernel) ──memcpy──► User Buffer ──memcpy──► OpenCV Mat ──memcpy──► YOLO Tensor (FPS: ~12)

【本專案優化：POSIX mmap 零拷貝映射 (Zero-Copy)】
Camera Frame Buffer (DMA 實體記憶體) 
       │
       ▼ (mmap 直通映射)
YOLOv8 Inference Pipeline (共享虛擬位址，零記憶體複製！) (FPS: 30+ 🚀)
```

### 🚀 具體工程優化效益：
1. **零拷貝 (Zero-Copy)**：利用 Linux POSIX `mmap()` 將 V4L2 視訊緩衝區直接映射至使用者進程位址空間。
2. **模型輕量化 (Model Quantization)**：YOLOv8 Nano 模型匯出為 INT8 量化格式，參數量壓縮 75%。
3. **成果**：CPU 負載降低 **60%**，端到端推論幀率由 **12 FPS 提升至 30+ FPS (即時 33ms/幀)**！

---

## 🖼️ Slide 3: 硬即時運動控制與反電動勢動態制動 (Motion Control)

### 1. 閉迴路 PID 循線差速轉彎模型 (Differential Drive Kinematics)
* **差速運動學**：利用左右輪轉速差產生角速度 $\omega = \frac{V_R - V_L}{L}$ 實現靈活轉向。
* **PID 軌跡跟隨**：根據 5 路 IR 循線感測器偏差值 $e(t)$ 即時計算補償輸出：
  $$u(t) = K_p e(t) + K_i \int e(t)dt + K_d \frac{de(t)}{dt}$$
  達成高速過 S 彎平滑過渡、零劇烈擺動。

### 2. L298N H 橋「反電動勢動態急煞 (Active Dynamic Braking)」機制
在避障硬即時防撞場景中，徹底解決傳統小車慣性衝撞問題：

| 控制模式 | 腳位設定 (IN1, IN2, ENA) | 電機物理狀態 | 煞車距離與效果 |
| :--- | :--- | :--- | :--- |
| **慣性滑行 (Coasting)** | `ENA = 0` (PWM = 0) | 4 顆 H 橋電晶體全部開路斷開 (High-Z) | ❌ 靠機械摩擦力慢慢停下，煞車距離長達 45 cm！ |
| **主動動態煞車 (Dynamic Braking)** | `IN1 = LOW, IN2 = LOW, ENA = 255` | **馬達兩端經由下臂電晶體直接短路** | ✅ **利用馬達旋轉發電機效應與反電動勢 (Back-EMF)，產生巨大反向電磁煞車力矩，煞停距離縮短至 8 cm (縮減 82%)！** |

---

## 🖼️ Slide 4: 重大工程挑戰與 Root-Cause 除錯實戰 (Debugging)

### 🛠️ 挑戰 1：馬達大負載啟動引發 MCU 異常重置 (Brownout Reset & EMI)
* **故障現象**：小車在靜止起步或急轉向時，MCU 偶發性死當或重開機。
* **根因分析 (Root-Cause)**：
  1. 示波器量測發現：DC 馬達啟動瞬間的突波堵轉電流高達 2.5A，將共用的 5V 電源瞬間拉低至 3.2V 以下，觸發 ATmega328P 內部 **Brownout Detector (BOD 欠壓復位)**。
  2. 馬達碳刷換向產生的高頻電弧電磁干擾 (EMI) 沿著電源線逆灌至 MCU 時脈線路。
* **硬體工程解決方案**：
  * **電源實體隔離**：動力端 (12V 鋰電池) 與 控制端 (5V 獨立 LDO/隔離 DC-DC) 分開走線。
  * **濾波拓撲**：在馬達兩端並聯 $0.1\mu\text{F}$ 陶瓷電容吸收高頻雜訊，電源入口處加裝 $470\mu\text{F}$ 電解電容穩壓，徹底消除重置現象。

---

### 🛠️ 挑戰 2：高速 UART 串列通訊撕裂與丟包 (Packet Corruption)
* **故障現象**：RPi 4 以高頻發送導航速度指令時，小車偶爾會突發性暴衝或指令無響應。
* **根因分析**：字串傳輸格式 (`"SPEED:100,50\n"`) 解析耗時過長，且缺少校驗碼，一旦有 1 Byte 雜訊干擾就會導致後續指令位移錯亂。
* **軟體工程解決方案**：
  * 設計 **二進位幀狀態機 (Finite State Machine Parser)**：
    `[0xAA 0x55 (Sync Header)] + [MsgID (1B)] + [Payload Length (1B)] + [Data (NB)] + [CRC-16/Checksum]`
  * 配合 **環形緩衝區 (Ring Buffer)** 進行非阻塞 (Non-Blocking) 接收，達成連續 24 小時運行 0 丟包！

---

## 🖼️ Slide 5: 量化成果指標與技術技能清單 (Key Metrics & Skills)

### 📊 關鍵量化指標對比 (Before vs After)

| 關鍵性能指標 | 原始傳統架構 | 經本專案優化後架構 | 改善幅度 |
| :--- | :---: | :---: | :---: |
| **相機影像推論幀率** | 12 FPS | **32 FPS** | 🚀 **+166%** |
| **影像管線 CPU 佔用率** | 82% | **28%** | 📉 **-65%** |
| **緊急避障煞停反應時間** | 350 ms | **35 ms** | ⚡ **縮短 90%** |
| **UART 封包傳輸錯誤率** | ~3.2% | **0.00% (CRC 保障)** | 🛡️ **工業級穩定** |

---

### 🛠️ 展現之技術技能樹 (Technical Skillsets)

* **程式語言**：C (純熟底層暫存器/指標/結構體)、C++ (物件導向與 ROS 2 節點)、Python (資料處理與自動化腳本)
* **嵌入式與硬體**：ARM Cortex-M / AVR、L298N H 橋馬達驅動、PWM / Timer 輸入捕獲、I2C / SPI / UART 協定、電源隔離與抗 EMI 設計
* **作業系統與架構**：Linux 系統程式設計、POSIX `mmap` 共享記憶體、FreeRTOS / 裸機即時控制、ROS 2 節點通訊
* **演算法與模型**：PID 回授控制、差速運動學模型、有限狀態機 (FSM)、YOLOv8 Edge AI 輕量化部署
