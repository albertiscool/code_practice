from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():

    camera_node = Node(
        package='camera_node',
        executable='usb_camera_node',
        name='usb_camera_node',
        output='screen'
    )

    detector_node = Node(
        package='balloon_detector',
        executable='balloon_detector_node',
        name='balloon_detector',
        output='screen'
    )

    controller_node = Node(
        package='car_controller',
        executable='controller_node',
        name='car_controller',
        output='screen'
    )

    serial_node = Node(
        package='serial_bridge',
        executable='serial_bridge_node',
        name='serial_bridge',
        output='screen'
    )

    return LaunchDescription([
        camera_node,
        detector_node,
        controller_node,
        serial_node
    ])
