cmake_minimum_required(VERSION 3.0.2)
project(biibiii_description)

find_package(catkin REQUIRED COMPONENTS
  urdf
  xacro
)

catkin_package(
  CATKIN_DEPENDS urdf xacro
)

## Build ##
include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Install ##
install(DIRECTORY rviz urdf
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)