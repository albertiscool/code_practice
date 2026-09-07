import rclpy
from rclpy.node import Node
import board
import adafruit_dht

class Dht11MonitorNode(Node):
    def __init__(self):
        super().__init__('dht11_monitor_node')
        # 初始化 DHT11 感測器，腳位接在 GPIO 15
        self.dht_device = adafruit_dht.DHT11(board.D15)
        # DHT11 取樣頻率不可高於 0.5Hz，設定 2 秒讀取一次
        self.timer = self.create_timer(2.0, self.timer_callback)

    def timer_callback(self):
        try:
            # 讀取溫濕度數值
            temp = self.dht_device.temperature
            hum = self.dht_device.humidity
            
            if temp is not None and hum is not None:
                self.get_logger().info(f'實體讀數 -> 溫度: {temp}°C, 濕度: {hum}%')
            else:
                self.get_logger().warning('讀取失敗，感測器回傳空值')
                
        except RuntimeError as err:
            # DHT11 常因 Linux 時序問題拋出例外，直接忽略並等待下次讀取
            self.get_logger().debug(f'讀取重試中: {err.args[0]}')
        except Exception as e:
            self.dht_device.exit()
            raise e

def main(args=None):
    rclpy.init(args=args)
    node = Dht11MonitorNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.dht_device.exit() # 釋放 GPIO 資源
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
