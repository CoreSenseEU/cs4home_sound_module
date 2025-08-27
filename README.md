# cs4home_sound_module

Use case of the [cs4home architecture prototype](https://github.com/CoreSenseEU/cs4home_architecture) focused on the cognitive module of sound perception with [llama_ros](https://github.com/mgonzs13/llama_ros/tree/humble?tab=readme-ov-file)

## Installation

```bash
sudo apt install ros-humble-rclcpp-cascade-lifecycle*
sudo apt install ros-humble-ament-cmake-clang-format
```

```bash
cd ~/ros2_ws/src
git clone https://github.com/CoreSenseEU/cs4home_sound_module
vcs import --recursive < cs4home_sound_module/thirparty.repos
cd ~/ros2_ws
colcon build --symlink-install
ros2 launch cs4home_sound_module launch_sound.launch.py
```

## Docker 
Build the llama_ros docker or download and image from [DockerHub](https://hub.docker.com/r/mgons/llama_ros/tags). You can choose to build llama_ros with CUDA (USE_CUDA) and choose the CUDA version (CUDA_VERSION). Remember that you have to use DOCKER_BUILDKIT=0 to compile llama_ros with CUDA when building the image.


```bash
DOCKER_BUILDKIT=0 docker build -t llama_ros --build-arg USE_CUDA=1 --build-arg CUDA_VERSION=12-6 .
```

Run the docker container with [Rocker](https://github.com/osrf/rocker)

```bash
cd ~/ros2_ws/src/cs4home_vision_module
rocker --nvidia --x11 \
  --network host --ipc host \
  --device /dev/snd \
  --device /dev/bus/usb/005/005 \
  --group-add audio \
  --volume ~/audio_ws:/root/ros2_ws \
  --env CYCLONEDDS_URI=file:///root/cyclone_config.xml \
  --volume ~/cyclone_config.xml:/root/cyclone_config.xml:ro \
  --privileged \
 llama_ros
```

## Simulator

If needed, you can use the RB1 robot simulation:

```bash
git clone -b mic-array-urdf https://github.com/igonzf/ros2_rb1.git
```

## Creating a Cognitive Module

Each cognitive module is defined through configuration and implemented using the base classes provided by the architecture.

### 1. Define the module in YAML

Edit the file:

`cs4home_sound_module/config/params.yaml`

Example:

```yaml
sound_recognition:
  ros_parameters:
    core: sound_context_evaluation
    afferent: sound_input
    sound_input:
      topics: ["/audio", "/doa"]
      types:
        ["audio_common_msgs/msg/AudioData", "geometry_msgs/msg/PoseStamped"]
    efferent: sound_context_output
    sound_context_output:
      topics: ["/sound_context", "/context_markers"]
      types: ["sound_msgs/msg/SoundContext", "visualization_msgs/msg/Marker"]
    meta: sound_meta
    coupling: sound_coupling
```

Fields:

- `core`: name of the main processing node
- `afferent`: input handler (subscribes to topics)
- `efferent`: output handler (publishes results)
- `meta`: module metadata manager
- `coupling`: logic that connects with other modules

### 2. Implement the Module

Create a class that inherits from cs4home_core::CognitiveModule:

```cpp
class SoundModuleCognitive : public cs4home_core::CognitiveModule {
// Instantiate lifecycle components here
};
```

Define each component as a subclass:

```cpp
class AudioInput : public cs4home_core::Afferent {};
class AudioOutput : public cs4home_core::Efferent {};
class DefaultCoupling : public cs4home_core::Coupling {};
class AudioMeta : public cs4home_core::Meta {};

class SoundRecognition : public cs4home*core::Core {
void process() override {
    // Access afferent input by index
    auto msg_audio = afferent*->get_msg<audio_common_msgs::msg::AudioData>(0);

    // Or by topic name
    auto msg_doa = afferent_->get_msg<geometry_msgs::msg::PoseStamped>("/doa");

    // Publish processed result
    efferent_->publish(0, sound);

}
};
```

Each component is managed as a ROS 2 Lifecycle Node and instantiated according to the YAML configuration.
