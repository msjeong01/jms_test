cmake_minimum_required(VERSION 3.0.2)
project(slam)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  cartographer_ros_msgs
)

catkin_package(
 INCLUDE_DIRS include
#  LIBRARIES slam
 CATKIN_DEPENDS roscpp sensor_msgs cartographer_ros_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(flat_world_imu_node src/flat_world_imu_node.cpp)
add_dependencies(flat_world_imu_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(flat_world_imu_node
  ${catkin_LIBRARIES}
)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

install(TARGETS flat_world_imu_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

install(DIRECTORY config launch rviz
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)