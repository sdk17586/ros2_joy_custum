#include "custum_joy_teleop/joy_teleop_node.hpp"

#include <memory>

namespace custum_joy_teleop
{

JoyTeleopNode::JoyTeleopNode()
: Node("joy_teleop_node")
{
  // Declare parameters
  this->declare_parameter<int>("linear_axis", 1);  // Default: left stick Y-axis
  this->declare_parameter<int>("angular_axis", 3);  // Default: axes[3] (4th value)
  this->declare_parameter<double>("linear_scale", 1.0);
  this->declare_parameter<double>("angular_scale", 1.0);

  // Get parameters
  linear_axis_ = this->get_parameter("linear_axis").as_int();
  angular_axis_ = this->get_parameter("angular_axis").as_int();
  linear_scale_ = this->get_parameter("linear_scale").as_double();
  angular_scale_ = this->get_parameter("angular_scale").as_double();

  // Create subscriber for /joy topic
  joy_sub_ = this->create_subscription<sensor_msgs::msg::Joy>(
    "/joy",
    10,
    std::bind(&JoyTeleopNode::joyCallback, this, std::placeholders::_1));

  // Create publisher for /cmd_vel topic
  cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>(
    "/cmd_vel",
    10);

  RCLCPP_INFO(this->get_logger(), "Joy Teleop Node started");
  RCLCPP_INFO(this->get_logger(), "Linear axis: %d, Angular axis: %d", linear_axis_, angular_axis_);
  RCLCPP_INFO(this->get_logger(), "Linear scale: %.2f, Angular scale: %.2f", linear_scale_, angular_scale_);
}

void JoyTeleopNode::joyCallback(const sensor_msgs::msg::Joy::SharedPtr msg)
{
  // Check if axes are valid
  if (static_cast<int>(msg->axes.size()) <= linear_axis_ ||
      static_cast<int>(msg->axes.size()) <= angular_axis_)
  {
    RCLCPP_WARN_THROTTLE(
      this->get_logger(),
      *this->get_clock(),
      1000,
      "Joystick message has insufficient axes. Expected at least %d axes, got %zu",
      std::max(linear_axis_, angular_axis_) + 1,
      msg->axes.size());
    return;
  }

  // Create Twist message
  auto twist_msg = geometry_msgs::msg::Twist();

  // Map joystick axes to linear and angular velocities
  twist_msg.linear.x = msg->axes[linear_axis_] * linear_scale_;
  twist_msg.angular.z = msg->axes[angular_axis_] * angular_scale_;

  // Publish the command
  cmd_vel_pub_->publish(twist_msg);
}

}  // namespace custum_joy_teleop

