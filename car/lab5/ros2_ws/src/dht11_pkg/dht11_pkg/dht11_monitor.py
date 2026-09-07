import rclpy
from rclpy.node import Node
import board
import adafruit_dht

class Dht11MonitorNode(Node):
    def __init__(self):
        super().__init__('dht11_monitor_node')
        # 初始化 DHT11 感測器
        self.dht_device = adafruit_dht.DHT11(board.D15)
        self.timer = self.create_timer(2.0, self.timer_callback)

    def timer_callback(self):
        try:
            temp = self.dht_device.temperature
            hum = self.dht_device.humidity
            
            if temp is not None and hum is not None:
                self.get_logger().info(f'實體讀數 -> 溫度: {temp}°C, 濕度: {hum}%')
            else:
                self.get_logger().warning('讀取失敗，感測器回傳空值')
                
        # 同時捕捉 RuntimeError 與 OverflowError，視為正常的硬體讀取波動
        except (RuntimeError, OverflowError) as err:
            self.get_logger().debug(f'時序誤差重試中: {err}')
            
        except Exception as e:
            # 遇到其他嚴重錯誤時才記錄並拋出
            self.get_logger().error(f'發生未預期的錯誤: {e}')
            raise e

def main(args=None):
    rclpy.init(args=args)
    node = Dht11MonitorNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        # 使用 try-except 安全地釋放資源，避免二次崩潰
        try:
            node.dht_device.exit()
        except Exception:
            pass
            
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
