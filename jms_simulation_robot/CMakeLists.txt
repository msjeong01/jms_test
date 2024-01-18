cmake_minimum_required(VERSION 3.0.2)
project(jms_simulation_robot)

find_package(catkin REQUIRED COMPONENTS
  gazebo_ros
  geometry_msgs
  nav_msgs
  roscpp
  sensor_msgs
  std_msgs
  std_srvs
  tf
  rospy
)

find_package(gazebo REQUIRED)

#catkin_python_setup()

catkin_package(
  #CATKIN_DEPENDS rospy geometry_msgs
)


link_directories(${GAZEBO_LIBRARY_DIRS})

include_directories(
  #include
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
)

# if(CATKIN_ENABLE_TESTING)
#   find_package(roslaunch REQUIRED)
#   roslaunch_add_file_check(launch)
# endif()

catkin_install_python(PROGRAMS
  scripts/teleop_key.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch maps params rviz scripts jms_slam urdf worlds
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)