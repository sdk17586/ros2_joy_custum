#ifndef CUSTUM_JOY_TELEOP__JOY_TELEOP_NODE_HPP_
#define CUSTUM_JOY_TELEOP__JOY_TELEOP_NODE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <geometry_msgs/msg/twist.hpp>

namespace custum_joy_teleop
{

class JoyTeleopNode : public rclcpp::Node
{
public:
  JoyTeleopNode();

private:
  void joyCallback(const sensor_msgs::msg::Joy::SharedPtr msg);

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joy_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;

  // Joystick axis indices (can be configured via parameters)
  int linear_axis_;
  int angular_axis_;
  
  // Scaling factors for velocity
  double linear_scale_;
  double angular_scale_;
};

}  // namespace custum_joy_teleop

#endif  // CUSTUM_JOY_TELEOP__JOY_TELEOP_NODE_HPP_

