cmake_minimum_required(VERSION 3.0.2)
project(biibiii_gazebo)

find_package(catkin REQUIRED COMPONENTS
  gazebo_ros
  geometry_msgs
  nav_msgs
  roscpp
  rospy
  sensor_msgs
  std_msgs
  tf
)

find_package(gazebo REQUIRED)

catkin_package(
  # INCLUDE_DIRS include
  # LIBRARIES biibiii_gazebo
  # CATKIN_DEPENDS gazebo_ros geometry_msgs nav_msgs roscpp rospy sensor_msgs std_msgs tf
  # DEPENDS system_lib
  # DEPENDS GAZEBO
)

link_directories(${GAZEBO_LIBRARY_DIRS})

## Build ##
include_directories(
  # include
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
)

## Install ##
install(DIRECTORY launch worlds 
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)