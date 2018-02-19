cmake_minimum_required(VERSION 3.9)
project(shaman_demo)

# folder for librairies
include_directories(./lib/exblas/ ./lib/cadna/)
link_directories(./lib/exblas/gpu ./lib/cadna/)

# folder for CADNA librairies
#include_directories(/opt/cadna/include)
#link_directories(/opt/cadna/lib)

# compiler and options
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_COMPILER g++)
#set(CMAKE_CXX_COMPILER clang++-5.0)
add_definitions(-DNUMERICAL_DEBUGGER)
#add_definitions(-DEXPLICIT_CASTING)
#add_definitions(-DNO_SHAMAN)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mfma -g -O1 -Wall -Wextra -Wno-unused-parameter -D__STRICT_ANSI__ -m64 -frounding-math") # -Werror -fopenmp

# files
set(SOURCE_FILES main.cpp
        # shaman
        shaman/shaman/Shaman_Decl.h shaman/shaman/Shaman.h
        shaman/shaman/NumericalDebugger.cpp shaman/shaman/NumericalDebugger.h
        # shamanizer
        shaman/shamanizer/tinyformat.h
        # tests
        tests/tests.h
        tests/exact/exact.cpp tests/exact/exact.h tests/sum.h
        tests/schrodinger.cpp tests/schrodinger.h
        tests/cadna.h
        tests/legendre.h)

# linking
add_executable(shaman_demo ${SOURCE_FILES})
target_link_libraries(shaman_demo "-lOpenCL -ltbb -lexblas -lcadnaC")
#target_link_libraries(shaman_demo "-lm -ltbb -lexblas")
