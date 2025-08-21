cmake_minimum_required(VERSION 3.10)
project(cs4home_sound_project)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(cs4home_core REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(sound_msgs REQUIRED)
find_package(audio_common_msgs REQUIRED)
find_package(yolo_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sound_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)


set(dependencies
  ament_cmake
  rclcpp
  rclcpp_lifecycle
  rclcpp_action
  cs4home_core
  sound_msgs
  audio_common_msgs
  geometry_msgs
  sound_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
  yolo_msgs
  std_msgs
  visualization_msgs
)

set(SOURCES
  src/${PROJECT_NAME}/SoundCognitiveModule.cpp
  src/${PROJECT_NAME}/Master.cpp
)

install(DIRECTORY
  config
  DESTINATION share/${PROJECT_NAME}
)

# add_library(${PROJECT_NAME} SHARED
#   src/SoundCognitiveModule.cpp
# )

# ament_target_dependencies(${PROJECT_NAME} ${dependencies})

# CORE
add_library(sound_recognition SHARED src/SoundRecognition.cpp)
ament_target_dependencies(sound_recognition ${dependencies} ${test_dependencies})
target_link_libraries(sound_recognition)

# AFFERENT
add_library(audio_input SHARED src/AudioInput.cpp)
ament_target_dependencies(audio_input ${dependencies} ${test_dependencies})
target_link_libraries(audio_input)

# EFFERENT
add_library(audio_output SHARED src/AudioOutput.cpp)
ament_target_dependencies(audio_output ${dependencies} ${test_dependencies})
target_link_libraries(audio_output)

# META
add_library(audio_meta SHARED src/AudioMeta.cpp)
ament_target_dependencies(audio_meta ${dependencies} ${test_dependencies})
target_link_libraries(audio_meta)

# COUPLING
add_library(audio_coupling SHARED src/AudioCoupling.cpp)
ament_target_dependencies(audio_coupling ${dependencies} ${test_dependencies})
target_link_libraries(audio_coupling)

add_executable(sound_cognitive_module src/SoundCognitiveModule.cpp)
ament_target_dependencies(sound_cognitive_module ${dependencies})
target_link_libraries(sound_cognitive_module sound_recognition audio_input audio_output audio_meta audio_coupling)


# MASTER

add_executable(sound_master src/Master.cpp)
ament_target_dependencies(sound_master ${dependencies})
target_link_libraries(sound_master)

install(TARGETS
  sound_cognitive_module
  sound_recognition
  audio_input
  audio_output
  audio_meta
  audio_coupling
  sound_master
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  sound_cognitive_module
  sound_master
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories(include)
ament_export_libraries(sound_recognition audio_input audio_output audio_meta audio_coupling)
ament_export_dependencies(${dependencies})
ament_package()
