#ifndef CUSTUM_JOY_TELEOP__JOY_TELEOP_NODE_HPP_
#define CUSTUM_JOY_TELEOP__JOY_TELEOP_NODE_HPP_

#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <std_msgs/msg/string.hpp>

namespace custum_joy_teleop {

class JoyTeleopNode : public rclcpp::Node {
public:
  JoyTeleopNode();

private:
  void initializeParameters();
  void initializeSubscribers();
  void initializePublishers();

  void joyCallback(const sensor_msgs::msg::Joy::SharedPtr msg);
  void processAxisInputs(const sensor_msgs::msg::Joy::SharedPtr msg);
  void processButtonInputs(const sensor_msgs::msg::Joy::SharedPtr msg);
  void handleXButton(bool pressed);

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joy_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr button_pub_;

  // Joystick axis indices (can be configured via parameters)
  int linear_axis_;
  int angular_axis_;

  // Scaling factors for velocity
  double linear_scale_;
  double angular_scale_;

  // Track previous button state to detect button press
  bool prev_x_button_state_;
};

} // namespace custum_joy_teleop

#endif // CUSTUM_JOY_TELEOP__JOY_TELEOP_NODE_HPP_
