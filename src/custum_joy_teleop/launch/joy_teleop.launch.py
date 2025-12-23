from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    # Launch arguments
    linear_axis_arg = DeclareLaunchArgument(
        'linear_axis',
        default_value='1',
        description='Joystick axis index for linear velocity (default: 1)'
    )
    
    angular_axis_arg = DeclareLaunchArgument(
        'angular_axis',
        default_value='3',
        description='Joystick axis index for angular velocity (default: 3, axes[3] - 4th value)'
    )
    
    linear_scale_arg = DeclareLaunchArgument(
        'linear_scale',
        default_value='1.0',
        description='Scaling factor for linear velocity (default: 1.0)'
    )
    
    angular_scale_arg = DeclareLaunchArgument(
        'angular_scale',
        default_value='1.0',
        description='Scaling factor for angular velocity (default: 1.0)'
    )

    # Joy node (joystick driver)
    joy_node = Node(
        package='joy',
        executable='joy_node',
        name='joy_node',
        output='screen'
    )

    # Joy teleop node
    joy_teleop_node = Node(
        package='custum_joy_teleop',
        executable='joy_teleop_node',
        name='joy_teleop_node',
        parameters=[{
            'linear_axis': LaunchConfiguration('linear_axis'),
            'angular_axis': LaunchConfiguration('angular_axis'),
            'linear_scale': LaunchConfiguration('linear_scale'),
            'angular_scale': LaunchConfiguration('angular_scale'),
        }],
        output='screen'
    )

    return LaunchDescription([
        linear_axis_arg,
        angular_axis_arg,
        linear_scale_arg,
        angular_scale_arg,
        joy_node,
        joy_teleop_node,
    ])

