cmake_minimum_required(VERSION 2.8.3)
project(rn_face)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  sensor_msgs
  std_msgs
  nav_msgs
  tf
  image_transport
  image_geometry
  cv_bridge
  catkin REQUIRED genmsg actionlib_msgs
)
#
# System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(OpenCV REQUIRED)


## Generate messages in the 'msg' folder
add_message_files(
   FILES
   hcma.msg
    hcm.msg
     head_cords_m_array.msg
      head_cords_m.msg
 )
generate_messages()

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES rn_face
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)
include_directories(include ${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)
## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)

## Declare a cpp library
# add_library(rn_face
#   src/${PROJECT_NAME}/rn_face.cpp
# )

## Declare a cpp executable
# add_executable(rn_face_node src/rn_face_node.cpp)
add_executable(trans_skel src/trans_skel.cpp)
set_target_properties(trans_skel PROPERTIES COMPILE_FLAGS  -std=gnu++0x)
target_link_libraries(trans_skel
    ${catkin_LIBRARIES}
    ${Boost_LIBRARIES}
)

add_executable(fdati src/fdati.cpp)
set_target_properties(fdati PROPERTIES COMPILE_FLAGS  -std=gnu++0x)
target_link_libraries(fdati
    ${catkin_LIBRARIES}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBS}
)

add_executable(fdati_crop src/fdati_crop.cpp)
set_target_properties(fdati_crop PROPERTIES COMPILE_FLAGS  -std=gnu++0x)
target_link_libraries(fdati_crop
    ${catkin_LIBRARIES}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBS}
)

add_executable(fdc2 src/fdc2.cpp)
set_target_properties(fdc2 PROPERTIES COMPILE_FLAGS  -std=gnu++0x)
target_link_libraries(fdc2
    ${catkin_LIBRARIES}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBS}
)

add_executable(cap_img_d src/cap_img_d.cpp)
set_target_properties(cap_img_d PROPERTIES COMPILE_FLAGS  -std=gnu++0x)
target_link_libraries(cap_img_d
    ${catkin_LIBRARIES}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBS}
)

add_executable(controller src/controller.cpp)
target_link_libraries(controller
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(rn_face_node rn_face_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(rn_face_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS rn_face rn_face_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_rn_face.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
