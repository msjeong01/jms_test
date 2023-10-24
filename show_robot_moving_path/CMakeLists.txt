cmake_minimum_required(VERSION 3.0.2)
project(show_robot_moving_path)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  geometry_msgs
  # message_generation
  move_base_msgs
  roscpp
  rospy
  rviz
  tf
  visualization_msgs
)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
)

find_package(Boost REQUIRED COMPONENTS filesystem system)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5OpenGL REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_LIBRARIES Qt5::Widgets)

add_definitions(-DQT_NO_KEYWORDS)

# generate_messages(
#   DEPENDENCIES
#   std_msgs
#   std_srvs
# )

# message_generation 
catkin_package(
#  INCLUDE_DIRS include
 LIBRARIES ${PROJECT_NAME}
 CATKIN_DEPENDS actionlib geometry_msgs move_base_msgs roscpp rospy rviz tf visualization_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)

link_directories(
  ${catkin_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS}
)

QT5_WRAP_CPP(MOC_FILES
  src/marking_path_teleop.h
  OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED -DBOOST_LEXICAL_CAST_INCLUDED 
)

set(SOURCE_FILES
  src/marking_path_teleop.cpp
  ${MOC_FILES}
)

add_library(${PROJECT_NAME}
  ${SOURCE_FILES}
)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(${PROJECT_NAME}
  ${QT_LIBRARIES}
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
)

#############
## Install ##
#############

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

install(FILES
  plugin_description.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "show_robot_moving_path")