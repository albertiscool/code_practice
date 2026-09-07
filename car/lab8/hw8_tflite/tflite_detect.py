import cv2
import numpy as np
from ai_edge_litert.interpreter import Interpreter

def load_labels(filename):
    with open(filename, 'r') as f:
        return [line.strip() for line in f.readlines()]

def main():
    # 1. 載入 TFLite 模型與標籤
    model_path = "detect.tflite"
    label_path = "labelmap.txt"
    
    print("⏳ 正在載入 TFLite 模型...")
    interpreter = Interpreter(model_path=model_path)
    interpreter.allocate_tensors()
    labels = load_labels(label_path)

    # 取得模型的輸入與輸出格式要求
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    height = input_details[0]['shape'][1]
    width = input_details[0]['shape'][2]
    floating_model = (input_details[0]['dtype'] == np.float32)

    # 2. 開啟 USB 攝影機 (請確認編號是否正確)
    cam_index = 0
    cap = cv2.VideoCapture(cam_index, cv2.CAP_V4L2)
    
    if not cap.isOpened():
        print("❌ 無法開啟攝影機，請檢查編號！")
        return

    print("✅ 模型載入完成！開始進行即時物件偵測 (按 'q' 離開)")

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        # 3. 影像前處理 (轉換成模型要的 300x300 RGB 格式)
        image_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        image_resized = cv2.resize(image_rgb, (width, height))
        input_data = np.expand_dims(image_resized, axis=0)

        # 4. 執行 TFLite 推論
        interpreter.set_tensor(input_details[0]['index'], input_data)
        interpreter.invoke()

        # 5. 取得偵測結果 (抓取框座標、類別、信心度)
        boxes = interpreter.get_tensor(output_details[0]['index'])[0]
        classes = interpreter.get_tensor(output_details[1]['index'])[0]
        scores = interpreter.get_tensor(output_details[2]['index'])[0]

        # 6. 將結果畫在畫面上
        for i in range(len(scores)):
            if scores[i] > 0.5: # 只顯示信心度大於 50% 的物件
                # 換算回原始畫面的座標
                ymin = int(max(1, (boxes[i][0] * frame.shape[0])))
                xmin = int(max(1, (boxes[i][1] * frame.shape[1])))
                ymax = int(min(frame.shape[0], (boxes[i][2] * frame.shape[0])))
                xmax = int(min(frame.shape[1], (boxes[i][3] * frame.shape[1])))
                
                # 畫框與文字
                object_name = labels[int(classes[i]) + 1]
                label = f"{object_name}: {int(scores[i]*100)}%"
                cv2.rectangle(frame, (xmin, ymin), (xmax, ymax), (0, 255, 0), 2)
                cv2.putText(frame, label, (xmin, ymin - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

        # 顯示畫面
        cv2.imshow('TFLite Object Detection', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
