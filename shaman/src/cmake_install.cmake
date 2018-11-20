# Install script for directory: /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/ccc/home/cont001/ocre/demeuren/tmp/shaman")
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
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/shaman.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/shaman" TYPE FILE FILES
    "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/shaman/eft.h"
    "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/shaman/methods.h"
    "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/shaman/operators.h"
    "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/shaman/functions.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/shaman" TYPE DIRECTORY FILES
    "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/shaman/helpers"
    "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/shaman/tagged"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE STATIC_LIBRARY FILES "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/libshaman.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/shaman" TYPE FILE FILES
    "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/generated/shamanConfig.cmake"
    "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/generated/shamanConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/shaman/shamanTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/shaman/shamanTargets.cmake"
         "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/CMakeFiles/Export/lib64/cmake/shaman/shamanTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/shaman/shamanTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/shaman/shamanTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/shaman" TYPE FILE FILES "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/CMakeFiles/Export/lib64/cmake/shaman/shamanTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/shaman" TYPE FILE FILES "/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/CMakeFiles/Export/lib64/cmake/shaman/shamanTargets-noconfig.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/src/shaman/unit_tests/cmake_install.cmake")

endif()

