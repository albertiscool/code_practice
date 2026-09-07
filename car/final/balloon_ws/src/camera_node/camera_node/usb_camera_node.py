import rclpy
from rclpy.node import Node

from sensor_msgs.msg import Image
from cv_bridge import CvBridge

import cv2


class CameraNode(Node):

    def __init__(self):
        super().__init__('camera_node')

        # Publisher
        self.publisher = self.create_publisher(
            Image,
            '/image_raw',
            10
        )

        self.bridge = CvBridge()

        # -----------------------------
        # 🎯 C270 設定（重點）
        # -----------------------------
        self.cap = cv2.VideoCapture(0, cv2.CAP_V4L2)

        # 設定 MJPG（C270 最穩）
        self.cap.set(cv2.CAP_PROP_FOURCC,
                     cv2.VideoWriter_fourcc(*'MJPG'))

        # 解析度（可改 640x480 或 1280x720）
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

        # FPS
        self.cap.set(cv2.CAP_PROP_FPS, 30)

        if not self.cap.isOpened():
            self.get_logger().error("Cannot open C270 camera!")
            return

        # Timer (30 FPS ≈ 0.033s)
        self.timer = self.create_timer(
            0.033,
            self.publish_frame
        )

        self.get_logger().info("C270 Camera Node Started")

    def publish_frame(self):

        ret, frame = self.cap.read()

        if not ret:
            self.get_logger().warn("Camera frame not received")
            return

        # OpenCV -> ROS Image
        msg = self.bridge.cv2_to_imgmsg(
            frame,
            encoding='bgr8'
        )

        msg.header.stamp = self.get_clock().now().to_msg()

        self.publisher.publish(msg)

    def destroy_node(self):

        if self.cap.isOpened():
            self.cap.release()

        super().destroy_node()


def main(args=None):

    rclpy.init(args=args)

    node = CameraNode()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
