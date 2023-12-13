cmake_minimum_required(VERSION 3.9)
project(shaman VERSION 0.1)

option(SHAMAN_ENABLE_TESTS "Whether or not Shaman tests are run" OFF)
option(SHAMAN_ENABLE_EXAMPLES "Whether or not Shaman examples are built" OFF)
# activate or deactivate Shaman's functionalities
option(SHAMAN_ENABLE_TAGGED_ERROR "Whether or not Shaman uses tagged error to locate the sources of error" OFF)
option(SHAMAN_ENABLE_UNSTABLE_BRANCH "Whether or not Shaman detects and counts unstable branches" OFF)
option(SHAMAN_DISABLE "Use to disable shaman and use traditional types instead" OFF)
option(SHAMAN_FETCH_TPLS "Automatically gets external dependencies" OFF)

# Introduce variables:
# * CMAKE_INSTALL_LIBDIR
# * CMAKE_INSTALL_BINDIR
# * CMAKE_INSTALL_INCLUDEDIR
include(GNUInstallDirs)

if (SHAMAN_FETCH_TPLS)
    include(FetchContent)
    FetchContent_Declare(
            GTest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.14.0
            OVERRIDE_FIND_PACKAGE
    )
    FetchContent_Declare(
            Kokkos
            GIT_REPOSITORY https://github.com/kokkos/kokkos
            GIT_TAG        4.2.00
            OVERRIDE_FIND_PACKAGE
    )
endif (SHAMAN_FETCH_TPLS)

if (SHAMAN_ENABLE_TESTS)
    enable_testing()
    SET(SHAMAN_EXAMPLES_TESTS ON)
    SET(SHAMAN_PERFORMANCE_TESTS ON)
endif(SHAMAN_ENABLE_TESTS)

add_subdirectory(src)

if (SHAMAN_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif(SHAMAN_ENABLE_EXAMPLES)
