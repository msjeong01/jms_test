cmake_minimum_required(VERSION 3.0.2)
project(numpy_tutorial)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  rospy
  std_msgs
)

################################################
## Declare ROS messages, services and actions ##
################################################

add_message_files(
  FILES
  Floats.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES numpy_tutorial
#  CATKIN_DEPENDS rospy python-numpy<
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

#############
## Install ##
#############

catkin_install_python(PROGRAMS
  scripts/numpy_listener.py
  scripts/numpy_talker.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
