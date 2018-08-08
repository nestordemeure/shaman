cmake_minimum_required(VERSION 3.9)
project(src)

# compiler
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_COMPILER g++)

# flags
#add_definitions(-DNO_SHAMAN)
# interprocedural optim -flto -Ofast
# to get readable perf output : -g -fno-omit-frame-pointer -fno-inline
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fopenmp -mfma -O3 -Wall -Wextra -Wno-unused-parameter -Werror -D__STRICT_ANSI__ -Wno-unknown-pragmas")

add_subdirectory(src)

# linking
#add_executable(src main.cpp ${SOURCE_SHAMAN} ${SOURCE_EXAMPLES})
#add_executable(lulesh examples/performances/lulesh.cpp ${SOURCE_SHAMAN})
enable_testing()
SET(SHAMAN_EXAMPLES_TESTS ON)
SET(SHAMAN_PERFORMANCE_TESTS ON)
add_subdirectory(examples)