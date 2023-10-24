cmake_minimum_required(VERSION 3.0.2)
project(rviz_teleop_plugin)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  roscpp
  rospy
  rviz
  sensor_msgs
  std_msgs
  std_srvs
  move_base_msgs
  actionlib
  tf
  interactive_markers
  visualization_msgs
  message_generation
  nav_core
)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_OGRE_MODULE_PATH}
)

set(CMAKE_MODULE_PATH ${CMAKE_OGRE_MODULE_PATH} "/usr/share/OGRE/cmake/modules")

find_package(Boost REQUIRED COMPONENTS filesystem system)

find_package(OGRE REQUIRED COMPONENTS filesystem system)

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

# add_service_files(
#   FILES
#   # modeChange.srv
# )

generate_messages(
  DEPENDENCIES
  std_msgs
  std_srvs
)

catkin_package(
  # INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS geometry_msgs nav_msgs roscpp rospy rviz sensor_msgs std_msgs std_srvs move_base_msgs actionlib tf interactive_markers visualization_msgs message_generation nav_core
  # DEPENDS system_lib
)

include_directories(
  # include
  ${catkin_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
)

link_directories(
  ${catkin_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS}
  ${OGRE_LIBRARIES_DIRS}
)

QT5_WRAP_CPP(MOC_FILES
  src/qt_teleop_ui.h
  OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED -DBOOST_LEXICAL_CAST_INCLUDED 
)

set(SOURCE_FILES
  src/qt_teleop_ui.cpp
  ${MOC_FILES}
)

add_library(${PROJECT_NAME}
  # src/qt_teleop.cpp
  # include/rviz_teleop_plugin/qt_teleop.ui
  # include/rviz_teleop_plugin/qt_teleop_ui.h
  # ${MOC_FILES} ${UIC_FILES} ${SOURCE_FILES}
  ${SOURCE_FILES}
)

add_dependencies(rviz_teleop_plugin ${PROJECT_NAME_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(${PROJECT_NAME}
  # Qt5::Widgets
  ${QT_LIBRARIES}
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
  ${OGRE_LIBRARIES}
)

install(TARGETS 
  ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(FILES plugin_description.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

set_target_properties(rviz_teleop_plugin PROPERTIES OUTPUT_NAME "rviz_teleop_plugin")