cmake_minimum_required(VERSION 3.0.2)
project(biibiii_navigation)

find_package(catkin REQUIRED COMPONENTS
  gazebo_ros
  geometry_msgs
  nav_msgs
  roscpp
  rospy
  sensor_msgs
  std_msgs
  std_srvs
  tf
)

find_package(gazebo REQUIRED)
#catkin_python_setup()

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES biibiii_navigation
#  CATKIN_DEPENDS gazebo_ros geometry_msgs nav_msgs roscpp rospy sensor_msgs std_msgs std_srvs tf
#  DEPENDS system_lib
)

## Build ##
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
)

install(DIRECTORY launch maps params rviz
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
