import sys
import serial
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32

class SerialTempBridge(Node):
    def __init__(self):
        # 1. 更改節點名稱
        super().__init__('serial_temp_bridge_node')
        
        # 2. 宣告 ROS 2 參數，讓 Port 和 Baudrate 可以從外部動態傳入
        self.declare_parameter('port_name', '/dev/ttyACM0')
        self.declare_parameter('baudrate', 9600)
        
        port = self.get_parameter('port_name').get_parameter_value().string_value
        baud = self.get_parameter('baudrate').get_parameter_value().integer_value
        
        # 3. 更改 Topic 名稱 (例如改為 /sensor/dht11_temp)
        self.temp_pub = self.create_publisher(Float32, '/sensor/dht11_temp', 10)
        
        # 初始化 UART 連線
        try:
            self.uart = serial.Serial(port, baud, timeout=1.0)
            self.get_logger().info(f'UART 連線成功 -> {port} ({baud} bps)')
        except serial.SerialException as err:
            self.get_logger().error(f'序列埠開啟失敗，請檢查硬體連接或權限: {err}')
            sys.exit(1)

        # 微調檢查頻率為 1.5 秒
        self.poll_timer = self.create_timer(1.5, self.read_uart_data)

    def read_uart_data(self):
        # 確保 UART 緩衝區有資料再讀取
        if self.uart.in_waiting > 0:
            try:
                raw_bytes = self.uart.readline()
                decoded_str = raw_bytes.decode('utf-8', errors='ignore').strip()
                
                # 預期字串格式範例: "Temp: 25.00°C"
                if "Temp:" in decoded_str:
                    # 使用 split 拆分冒號後面的部分，再去除多餘字元與空白
                    # 這樣即使格式變成 "Temp: 25.0 C" 也能正確抓到數字
                    val_part = decoded_str.split("Temp:")[1].replace("°C", "").strip()
                    current_temp = float(val_part)
                    
                    # 終端機輸出資訊微調
                    self.get_logger().info(f'當前偵測溫度: {current_temp:.1f} °C')
                    
                    # 打包並發布訊息
                    out_msg = Float32()
                    out_msg.data = current_temp
                    self.temp_pub.publish(out_msg)
                    
            except ValueError:
                self.get_logger().debug('數值轉換失敗，可能是傳輸過程中的雜訊干擾')
            except Exception as ex:
                self.get_logger().warning(f'讀取或解析時發生例外狀況: {ex}')

def main(args=None):
    rclpy.init(args=args)
    bridge_node = SerialTempBridge()
    
    try:
        rclpy.spin(bridge_node)
    except KeyboardInterrupt:
        bridge_node.get_logger().info('收到中斷訊號，正在關閉節點...')
    finally:
        # 安全關閉通訊埠
        if hasattr(bridge_node, 'uart') and bridge_node.uart.is_open:
            bridge_node.uart.close()
            bridge_node.get_logger().info('UART 埠已釋放。')
            
        bridge_node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()