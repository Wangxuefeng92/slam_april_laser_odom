# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "slam_april_laser_odom: 3 messages, 0 services")

set(MSG_I_FLAGS "-Islam_april_laser_odom:/home/wxf/catkin_ws/src/slam_april_laser_odom/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/indigo/share/geometry_msgs/cmake/../msg;-Inav_msgs:/opt/ros/indigo/share/nav_msgs/cmake/../msg;-Iactionlib_msgs:/opt/ros/indigo/share/actionlib_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(slam_april_laser_odom_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetection.msg" NAME_WE)
add_custom_target(_slam_april_laser_odom_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "slam_april_laser_odom" "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetection.msg" "geometry_msgs/Point:geometry_msgs/PoseStamped:geometry_msgs/Quaternion:std_msgs/Header:geometry_msgs/Pose"
)

get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/PoseStampedArray.msg" NAME_WE)
add_custom_target(_slam_april_laser_odom_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "slam_april_laser_odom" "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/PoseStampedArray.msg" "geometry_msgs/Point:geometry_msgs/PoseStamped:geometry_msgs/Quaternion:std_msgs/Header:geometry_msgs/Pose"
)

get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetectionArray.msg" NAME_WE)
add_custom_target(_slam_april_laser_odom_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "slam_april_laser_odom" "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetectionArray.msg" "geometry_msgs/Point:slam_april_laser_odom/AprilTagDetection:geometry_msgs/Quaternion:geometry_msgs/Pose:geometry_msgs/PoseStamped:std_msgs/Header"
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(slam_april_laser_odom
  "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/slam_april_laser_odom
)
_generate_msg_cpp(slam_april_laser_odom
  "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/PoseStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/slam_april_laser_odom
)
_generate_msg_cpp(slam_april_laser_odom
  "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetectionArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetection.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/slam_april_laser_odom
)

### Generating Services

### Generating Module File
_generate_module_cpp(slam_april_laser_odom
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/slam_april_laser_odom
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(slam_april_laser_odom_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(slam_april_laser_odom_generate_messages slam_april_laser_odom_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetection.msg" NAME_WE)
add_dependencies(slam_april_laser_odom_generate_messages_cpp _slam_april_laser_odom_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/PoseStampedArray.msg" NAME_WE)
add_dependencies(slam_april_laser_odom_generate_messages_cpp _slam_april_laser_odom_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetectionArray.msg" NAME_WE)
add_dependencies(slam_april_laser_odom_generate_messages_cpp _slam_april_laser_odom_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(slam_april_laser_odom_gencpp)
add_dependencies(slam_april_laser_odom_gencpp slam_april_laser_odom_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS slam_april_laser_odom_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(slam_april_laser_odom
  "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/slam_april_laser_odom
)
_generate_msg_lisp(slam_april_laser_odom
  "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/PoseStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/slam_april_laser_odom
)
_generate_msg_lisp(slam_april_laser_odom
  "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetectionArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetection.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/slam_april_laser_odom
)

### Generating Services

### Generating Module File
_generate_module_lisp(slam_april_laser_odom
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/slam_april_laser_odom
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(slam_april_laser_odom_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(slam_april_laser_odom_generate_messages slam_april_laser_odom_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetection.msg" NAME_WE)
add_dependencies(slam_april_laser_odom_generate_messages_lisp _slam_april_laser_odom_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/PoseStampedArray.msg" NAME_WE)
add_dependencies(slam_april_laser_odom_generate_messages_lisp _slam_april_laser_odom_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetectionArray.msg" NAME_WE)
add_dependencies(slam_april_laser_odom_generate_messages_lisp _slam_april_laser_odom_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(slam_april_laser_odom_genlisp)
add_dependencies(slam_april_laser_odom_genlisp slam_april_laser_odom_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS slam_april_laser_odom_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(slam_april_laser_odom
  "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/slam_april_laser_odom
)
_generate_msg_py(slam_april_laser_odom
  "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/PoseStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/slam_april_laser_odom
)
_generate_msg_py(slam_april_laser_odom
  "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetectionArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Point.msg;/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetection.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/indigo/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/slam_april_laser_odom
)

### Generating Services

### Generating Module File
_generate_module_py(slam_april_laser_odom
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/slam_april_laser_odom
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(slam_april_laser_odom_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(slam_april_laser_odom_generate_messages slam_april_laser_odom_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetection.msg" NAME_WE)
add_dependencies(slam_april_laser_odom_generate_messages_py _slam_april_laser_odom_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/PoseStampedArray.msg" NAME_WE)
add_dependencies(slam_april_laser_odom_generate_messages_py _slam_april_laser_odom_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wxf/catkin_ws/src/slam_april_laser_odom/msg/AprilTagDetectionArray.msg" NAME_WE)
add_dependencies(slam_april_laser_odom_generate_messages_py _slam_april_laser_odom_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(slam_april_laser_odom_genpy)
add_dependencies(slam_april_laser_odom_genpy slam_april_laser_odom_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS slam_april_laser_odom_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/slam_april_laser_odom)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/slam_april_laser_odom
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(slam_april_laser_odom_generate_messages_cpp std_msgs_generate_messages_cpp)
add_dependencies(slam_april_laser_odom_generate_messages_cpp geometry_msgs_generate_messages_cpp)
add_dependencies(slam_april_laser_odom_generate_messages_cpp nav_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/slam_april_laser_odom)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/slam_april_laser_odom
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(slam_april_laser_odom_generate_messages_lisp std_msgs_generate_messages_lisp)
add_dependencies(slam_april_laser_odom_generate_messages_lisp geometry_msgs_generate_messages_lisp)
add_dependencies(slam_april_laser_odom_generate_messages_lisp nav_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/slam_april_laser_odom)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/slam_april_laser_odom\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/slam_april_laser_odom
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(slam_april_laser_odom_generate_messages_py std_msgs_generate_messages_py)
add_dependencies(slam_april_laser_odom_generate_messages_py geometry_msgs_generate_messages_py)
add_dependencies(slam_april_laser_odom_generate_messages_py nav_msgs_generate_messages_py)
