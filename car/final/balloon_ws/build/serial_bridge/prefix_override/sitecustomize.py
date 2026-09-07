import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/fr/ros2_ws/balloon_ws/install/serial_bridge'
