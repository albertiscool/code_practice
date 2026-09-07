import rclpy
from rclpy.node import Node

from std_msgs.msg import Float32MultiArray
from geometry_msgs.msg import Twist


class CarController(Node):

    def __init__(self):

        super().__init__('car_controller')

        self.sub = self.create_subscription(
            Float32MultiArray,
            '/balloon_target',
            self.callback,
            10
        )

        self.pub = self.create_publisher(
            Twist,
            '/cmd_vel',
            10
        )

        self.get_logger().info("Car Controller Started")

        self.center_x = 320
        self.tolerance = 40

    def callback(self, msg):

        data = msg.data
        cmd = Twist()

        # ❌ 沒看到球 → 搜尋
        if len(data) == 0:
            cmd.angular.z = 0.3
            self.pub.publish(cmd)
            return

        x = data[0]
        area = data[2]

        error = x - self.center_x

        # 🔄 左右修正
        if abs(error) > self.tolerance:

            cmd.linear.x = 0.0

            if error > 0:
                cmd.angular.z = -0.3
            else:
                cmd.angular.z = 0.3

        else:
            # 🚗 前進
            cmd.linear.x = 0.2
            cmd.angular.z = 0.0

        # 🛑 停止條件（太近）
        if area > 25000:
            cmd.linear.x = 0.0
            cmd.angular.z = 0.0

        self.pub.publish(cmd)


def main(args=None):

    rclpy.init(args=args)

    node = CarController()

    rclpy.spin(node)

    node.destroy_node()

    rclpy.shutdown()


if __name__ == '__main__':
    main()
