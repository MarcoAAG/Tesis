# Install script for directory: /home/marco/Tesis/ROS/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/marco/Tesis/ROS/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  
      if (NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
        file(MAKE_DIRECTORY "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
      endif()
      if (NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/.catkin")
        file(WRITE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/.catkin" "")
      endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/marco/Tesis/ROS/install/_setup_util.py")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/marco/Tesis/ROS/install" TYPE PROGRAM FILES "/home/marco/Tesis/ROS/build/catkin_generated/installspace/_setup_util.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/marco/Tesis/ROS/install/env.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/marco/Tesis/ROS/install" TYPE PROGRAM FILES "/home/marco/Tesis/ROS/build/catkin_generated/installspace/env.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/marco/Tesis/ROS/install/setup.bash;/home/marco/Tesis/ROS/install/local_setup.bash")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/marco/Tesis/ROS/install" TYPE FILE FILES
    "/home/marco/Tesis/ROS/build/catkin_generated/installspace/setup.bash"
    "/home/marco/Tesis/ROS/build/catkin_generated/installspace/local_setup.bash"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/marco/Tesis/ROS/install/setup.sh;/home/marco/Tesis/ROS/install/local_setup.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/marco/Tesis/ROS/install" TYPE FILE FILES
    "/home/marco/Tesis/ROS/build/catkin_generated/installspace/setup.sh"
    "/home/marco/Tesis/ROS/build/catkin_generated/installspace/local_setup.sh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/marco/Tesis/ROS/install/setup.zsh;/home/marco/Tesis/ROS/install/local_setup.zsh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/marco/Tesis/ROS/install" TYPE FILE FILES
    "/home/marco/Tesis/ROS/build/catkin_generated/installspace/setup.zsh"
    "/home/marco/Tesis/ROS/build/catkin_generated/installspace/local_setup.zsh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/marco/Tesis/ROS/install/.rosinstall")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/marco/Tesis/ROS/install" TYPE FILE FILES "/home/marco/Tesis/ROS/build/catkin_generated/installspace/.rosinstall")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/marco/Tesis/ROS/build/gtest/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/image_pipeline/image_pipeline/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_arduino/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_mbed/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_msgs/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_python/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_tivac/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_vex_cortex/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_vex_v5/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_xbee/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_client/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/serial/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/msp430/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/image_pipeline/camera_calibration/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/tms570lc43/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_server/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/image_pipeline/image_proc/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/image_pipeline/image_publisher/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/ocam/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/image_pipeline/image_view/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/image_pipeline/stereo_image_proc/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/image_pipeline/depth_image_proc/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_embeddedlinux/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_test/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/rosserial/rosserial_windows/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/image_pipeline/image_rotate/cmake_install.cmake")
  include("/home/marco/Tesis/ROS/build/tf2_system/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/marco/Tesis/ROS/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
