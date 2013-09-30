cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})
rosbuild_add_executable(trans_skel src/trans_skel.cpp)
rosbuild_add_executable(fdati src/fdati.cpp)
rosbuild_add_executable(fdati_crop src/fdati_crop.cpp)
rosbuild_add_executable(cap_img_d src/cap_img_d.cpp)
rosbuild_add_executable(controller src/controller.cpp)
rosbuild_add_executable(battery src/battery.cpp)
rosbuild_add_executable(move_script src/move_script.cpp)

rosbuild_add_compile_flags(fdati -std=gnu++0x )
rosbuild_add_compile_flags(fdati_crop -std=gnu++0x )
rosbuild_add_compile_flags(fdati -std=gnu++0x)
rosbuild_add_compile_flags(fdati_crop -std=gnu++0x )
rosbuild_add_compile_flags(trans_skel -std=gnu++0x )
rosbuild_add_compile_flags(cap_img_d -std=gnu++0x)
rosbuild_add_compile_flags(controller -std=gnu++0x)
rosbuild_add_compile_flags(battery -std=gnu++0x)
rosbuild_add_compile_flags(move_script -std=gnu++0x)


rosbuild_add_boost_directories()
rosbuild_link_boost(fdati  filesystem system)
rosbuild_link_boost(fdati_crop  filesystem system)
rosbuild_link_boost(cap_img_d filesystem system)
rosbuild_link_boost(trans_skel filesystem system)
rosbuild_link_boost(controller filesystem system)
rosbuild_link_boost(battery filesystem system)
rosbuild_link_boost(move_script filesystem system)
