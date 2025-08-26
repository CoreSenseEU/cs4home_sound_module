#include "cs4home_core/CognitiveModule.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

class SoundCognitiveModule : public cs4home_core::CognitiveModule {
public:
  explicit SoundCognitiveModule(
      const rclcpp::NodeOptions &options = rclcpp::NodeOptions())
      : cs4home_core::CognitiveModule("sound_cognitive_module", options) {
    RCLCPP_INFO(this->get_logger(), "SoundCognitiveModule initialized");
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SoundCognitiveModule>();
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}
