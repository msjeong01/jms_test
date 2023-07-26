cmake_minimum_required(VERSION 3.0.2)
project(assign02)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  std_srvs
)

## Generate services in the 'srv' folder
add_service_files(
   FILES
   TwoNums.srv
   #FirstNum.srv
   #SecondNum.srv
   #Operator.srv
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
  std_srvs
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES assign02
#  CATKIN_DEPENDS message_generation roscpp rospy std_msgs std_srvs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(cal_two_nums_server src/cal_two_nums_server.cpp)
target_link_libraries(cal_two_nums_server ${catkin_LIBRARIES})
add_dependencies(cal_two_nums_server ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})