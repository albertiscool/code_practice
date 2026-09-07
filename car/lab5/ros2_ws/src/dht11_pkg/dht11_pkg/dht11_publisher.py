import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import board
import adafruit_dht

class Dht11Publisher(Node):
    def __init__(self):
        super().__init__('dht11_publisher')
        # 建立發布者，發布主題為 "DH11TH"，佇列大小設為 10
        self.publisher_ = self.create_publisher(String, 'DH11TH', 10)
        
        # 初始化實體 DHT11 感測器，接在 GPIO 15 (board.D15)
        self.dht_device = adafruit_dht.DHT11(board.D15)
        
        # 設定每 2 秒發布一次資料
        self.timer = self.create_timer(2.0, self.timer_callback)

    def timer_callback(self):
        try:
            temp = self.dht_device.temperature
            hum = self.dht_device.humidity
            
            if temp is not None and hum is not None:
                msg = String()
                msg.data = f'溫度: {temp}°C, 濕度: {hum}%'
                self.publisher_.publish(msg)
                self.get_logger().info(f'已發布實體資料: "{msg.data}"')
            else:
                self.get_logger().warning('讀取失敗，感測器回傳空值')
                
        # 同時捕捉 RuntimeError 與 OverflowError，避免底層脈衝計算超時導致程式崩潰
        except (RuntimeError, OverflowError) as err:
            self.get_logger().debug(f'時序誤差，跳過本次發布: {err}')
            
        except Exception as e:
            self.get_logger().error(f'發布者發生未預期的錯誤: {e}')
            raise e

def main(args=None):
    rclpy.init(args=args)
    node = Dht11Publisher()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        # 安全地釋放 GPIO 資源，避免二次崩潰
        try:
            node.dht_device.exit()
        except Exception:
            pass
            
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
