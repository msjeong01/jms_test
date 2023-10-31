cmake_minimum_required(VERSION 3.0.2)
project(parameters_in_rospy)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES parameters_in_rospy
#  CATKIN_DEPENDS message_generation roscpp rospy std_msgs
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
  scripts/param_talker.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
