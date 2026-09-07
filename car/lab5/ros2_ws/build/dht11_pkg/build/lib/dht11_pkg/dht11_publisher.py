import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import board
import adafruit_dht

class Dht11Publisher(Node):
    def __init__(self):
        super().__init__('dht11_publisher')
        self.publisher_ = self.create_publisher(String, 'DH11TH', 10)
        self.dht_device = adafruit_dht.DHT11(board.D15)
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
                
        except RuntimeError as err:
            # 發生 checksum 或時序錯誤時跳過本次發布
            self.get_logger().debug(f'讀取錯誤，跳過本次發布: {err.args[0]}')
        except Exception as e:
            self.dht_device.exit()
            raise e

def main(args=None):
    rclpy.init(args=args)
    node = Dht11Publisher()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.dht_device.exit()
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
