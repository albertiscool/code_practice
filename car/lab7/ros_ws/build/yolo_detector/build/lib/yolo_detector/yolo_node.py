import rclpy, cv2
from rclpy.node import Node
from sensor_msgs.msg import Image
from std_msgs.msg import String
from cv_bridge import CvBridge
from ultralytics import YOLO

class YoloNode(Node):
    def __init__(self):
        super().__init__('yolo_node')
        self.subscription = self.create_subscription(Image, '/camera/image_raw', self.image_callback, 10)
        self.publisher_ = self.create_publisher(String, '/yolo_detections', 10)
        self.bridge = CvBridge()
        self.model = YOLO('yolov8n.pt')

    def image_callback(self, msg):
        frame = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
        results = self.model(frame, verbose=False)
        
        detections = [f"{self.model.names[int(b.cls[0])]}: {float(b.conf[0]):.2f}" for b in results[0].boxes]
        det_msg = String(data=", ".join(detections) if detections else "No objects")
        self.publisher_.publish(det_msg)
        
        cv2.imshow("YOLOv8 Detection", results[0].plot())
        cv2.waitKey(1)

def main(args=None):
    rclpy.init(args=args)
    node = YoloNode()
    rclpy.spin(node)
    node.destroy_node()
    cv2.destroyAllWindows()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
