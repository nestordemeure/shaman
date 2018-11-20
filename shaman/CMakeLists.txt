cmake_minimum_required(VERSION 3.9)
project(shaman VERSION 0.1)

option(SHAMAN_ENABLE_TESTS "Whether or not Shaman tests are run" ON)
option(SHAMAN_ENABLE_EXAMPLES "Whether or not Shaman examples are built" ON)

<<<<<<< HEAD
# Introduce variables:
# * CMAKE_INSTALL_LIBDIR
# * CMAKE_INSTALL_BINDIR
# * CMAKE_INSTALL_INCLUDEDIR
include(GNUInstallDirs)
=======
# flags
#add_definitions(-DNUMERICAL_DEBUGGER)
add_definitions(-DUNSTABLE_BRANCH_DEBUGGER)
add_definitions(-DCANCELATION_DEBUGGER)
add_definitions(-DUNSTABLE_OP_DEBUGGER)
#add_definitions(-DNUMERICAL_ZERO_DEBUGGER)
#add_definitions(-DEXPLICIT_CASTING) # forbid implicit casts between types
#add_definitions(-DNO_SHAMAN) # deactivate Shaman
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fopenmp -mfma -O2 -Wall -Wextra -Wno-unused-parameter -Werror -D__STRICT_ANSI__ -Wno-unknown-pragmas")
>>>>>>> d99b237c32cd2234dd746d21fd48e6b01abb837f

if (SHAMAN_ENABLE_TESTS)
    enable_testing()
    SET(SHAMAN_EXAMPLES_TESTS ON)
    SET(SHAMAN_PERFORMANCE_TESTS ON)
endif(SHAMAN_ENABLE_TESTS)

add_subdirectory(src)

if (SHAMAN_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif(SHAMAN_ENABLE_EXAMPLES)
