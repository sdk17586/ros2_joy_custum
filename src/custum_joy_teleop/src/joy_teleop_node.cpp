#include "custum_joy_teleop/joy_teleop_node.hpp"

#include <memory>

namespace custum_joy_teleop {

JoyTeleopNode::JoyTeleopNode() : Node("joy_teleop_node") {
  // Declare parameters
  this->declare_parameter<int>("linear_axis", 1); // Default: left stick Y-axis
  this->declare_parameter<int>("angular_axis",
                               3); // Default: axes[3] (4th value)
  this->declare_parameter<double>("linear_scale", 1.0);
  this->declare_parameter<double>("angular_scale", 1.0);

  // Get parameters
  linear_axis_ = this->get_parameter("linear_axis").as_int();
  angular_axis_ = this->get_parameter("angular_axis").as_int();
  linear_scale_ = this->get_parameter("linear_scale").as_double();
  angular_scale_ = this->get_parameter("angular_scale").as_double();

  // Create subscriber for /joy topic
  joy_sub_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy", 10,
      std::bind(&JoyTeleopNode::joyCallback, this, std::placeholders::_1));

  // Create publisher for /cmd_vel topic
  cmd_vel_pub_ =
      this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

  // Create publisher for string messages
  string_pub_ = this->create_publisher<std_msgs::msg::String>("/x_button", 10);

  prev_x_button_state_ = false;
}

void JoyTeleopNode::joyCallback(const sensor_msgs::msg::Joy::SharedPtr msg) {
  //조이스틱 제어
  processAxisInputs(msg);

  //버튼 제어
  processButtonInputs(msg);
}

void JoyTeleopNode::processAxisInputs(const sensor_msgs::msg::Joy::SharedPtr msg) {

  if (static_cast<int>(msg->axes.size()) <= linear_axis_ ||
      static_cast<int>(msg->axes.size()) <= angular_axis_) {
    RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 1000,
                         "Joystick message has insufficient axes. Expected at "
                         "least %d axes, got %zu",
                         std::max(linear_axis_, angular_axis_) + 1,
                         msg->axes.size());
    return;
  }

  auto twist_msg = geometry_msgs::msg::Twist();

  twist_msg.linear.x = msg->axes[linear_axis_] * linear_scale_;
  twist_msg.angular.z = msg->axes[angular_axis_] * angular_scale_;

  cmd_vel_pub_->publish(twist_msg);
}

void JoyTeleopNode::processButtonInputs(const sensor_msgs::msg::Joy::SharedPtr msg) {
  if (msg->buttons.empty()) {
    return;
  }

  // Handle X button (usually buttons[0])
  if (msg->buttons.size() > 0) {
    bool x_button_pressed = (msg->buttons[2] == 1);
    handleXButton(x_button_pressed);
  }

  // 여기에 다른 버튼 처리 로직 추가 가능
  // 예: handleYButton, handleAButton, handleBButton 등
}

void JoyTeleopNode::handleXButton(bool pressed) {
  // X버튼 눌림 감지 (not pressed -> pressed 전환 시)
  if (pressed && !prev_x_button_state_) {
    auto string_msg = std_msgs::msg::String();
    string_msg.data = "x_push";
    string_pub_->publish(string_msg);
    RCLCPP_INFO(this->get_logger(), "X button pressed: published 'x_push'");
  }

  prev_x_button_state_ = pressed;
}

} 
