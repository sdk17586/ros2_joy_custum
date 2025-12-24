#include "custum_joy_teleop/joy_teleop_node.hpp"
#include <rclcpp/rclcpp.hpp>

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<custum_joy_teleop::JoyTeleopNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
