cmake_minimum_required(VERSION 2.8.3)
project(slam_april_laser_odom)
add_definitions(-std=c++11 -Wall)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  serial
  std_msgs
  geometry_msgs
  tf

  ethz_apriltag2
  image_transport
  sensor_msgs
  nav_msgs
  cv_bridge
  message_generation
  nodelet
  cmake_modules
)
find_package(Eigen REQUIRED)

add_message_files(
  FILES
  AprilTagDetection.msg
  AprilTagDetectionArray.msg
  PoseStampedArray.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
  nav_msgs
)
catkin_package(
  CATKIN_DEPENDS
    serial
    std_msgs
    geometry_msgs
    ethz_apriltag2 image_transport roscpp sensor_msgs nav_msgs tf cv_bridge message_runtime nodelet
    DEPENDS Eigen
)
###########
## Build ##
###########
# 添加g2o的依赖
# 因为g2o不是常用库，要添加它的findg2o.cmake文件
LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )
SET( G2O_ROOT /usr/local/include/g2o )
FIND_PACKAGE( G2O REQUIRED )
# CSparse
FIND_PACKAGE( CSparse REQUIRED )
INCLUDE_DIRECTORIES( ${G2O_INCLUDE_DIR} ${CSPARSE_INCLUDE_DIR} )
#########################3
include_directories(include)
include_directories(
  ${Eigen_INCLUDE_DIRS}
)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(serial_slam src/serial_slam.cpp)

target_link_libraries(serial_slam
  ${catkin_LIBRARIES}
)

add_library(apriltag_detector_slam
  src/apriltag_detector_slam.cpp
)
target_link_libraries(apriltag_detector_slam
  ${catkin_LIBRARIES}
)
add_dependencies(apriltag_detector_slam apriltags_ros_generate_messages_cpp)


add_library(apriltag_detector_slam_nodelet src/apriltag_detector_slam_nodelet.cpp)
target_link_libraries(apriltag_detector_slam_nodelet
  ${catkin_LIBRARIES}
  apriltag_detector_slam
)

add_executable(apriltag_detector_slam_node src/apriltag_detector_slam_node.cpp)
target_link_libraries(apriltag_detector_slam_node
  ${catkin_LIBRARIES}
  apriltag_detector_slam
)

add_executable(image_pub_slam src/image_pub_slam.cpp)
target_link_libraries(image_pub_slam
  ${catkin_LIBRARIES}
)

add_executable(apriltag_g2o_slam src/apriltag_g2o_slam.cpp)
target_link_libraries(apriltag_g2o_slam
  ${catkin_LIBRARIES} g2o_core g2o_types_slam2d g2o_solver_csparse g2o_stuff g2o_csparse_extension ${CSPARSE_LIBRARY}
)


#############
## Install ##image_pub
#############

## Mark executables and/or libraries for installation
install(TARGETS apriltag_detector_slam apriltag_detector_slam_nodelet apriltag_detector_slam_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/apriltags_ros
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

## Mark other files for installation (e.g. launch and bag files, etc.)
install(FILES nodelet_plugins.xml
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
