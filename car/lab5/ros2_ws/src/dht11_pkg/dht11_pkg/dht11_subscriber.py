import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class Dht11Subscriber(Node):
    def __init__(self):
        super().__init__('dht11_subscriber')
        # 訂閱主題 "DH11TH"
        self.subscription = self.create_subscription(
            String,
            'DH11TH',
            self.listener_callback,
            10
        )
        self.subscription  # 防止被垃圾回收

    def listener_callback(self, msg):
        self.get_logger().info(f'收到主題資料 -> {msg.data}')

def main(args=None):
    rclpy.init(args=args)
    node = Dht11Subscriber()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
