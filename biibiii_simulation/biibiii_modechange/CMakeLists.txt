cmake_minimum_required(VERSION 3.0.2)
project(biibiii_modechange)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  std_srvs
  message_generation
)

add_service_files(
  FILES
  modeChange.srv
)

generate_messages(
  DEPENDENCIES
  std_srvs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES biibiii_modechange
#  CATKIN_DEPENDS roscpp rospy std_msgs std_srvs message_generation
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)


install(DIRECTORY launch srv
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

add_executable(mode_change src/mode_change.cpp)
target_link_libraries(mode_change ${catkin_LIBRARIES})
add_dependencies(mode_change
  ${catkin_EXPORTED_TARGETS}
)