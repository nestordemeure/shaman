cmake_minimum_required(VERSION 3.9)
project(shaman_demo)

# folder for librairies
include_directories(./lib/exblas/ ./lib/cadna/ ./lib/eigen/)
link_directories(./lib/exblas/gpu ./lib/cadna/)

# compiler
set(CMAKE_CXX_STANDARD 14)
# ocre
#set(CMAKE_CXX_COMPILER /opt/verificarlo/bin/verificarlo)
#set(CMAKE_CXX_COMPILER /ccc/home/cont001/opendev/opendev/RHEL7/gcc48/llvm/3.8.0/bin/clang)
# laptop
#set(CMAKE_CXX_COMPILER verificarlo)
#set(CMAKE_CXX_COMPILER clang++-5.0)
# mpi
#set(CMAKE_CXX_COMPILER /usr/local/libre/bin/mpic++)

# flags
add_definitions(-DNUMERICAL_DEBUGGER)
#add_definitions(-DNUMERICAL_ZERO_DEBUGGER)
#add_definitions(-DEXPLICIT_CASTING)
#add_definitions(-DNO_SHAMAN)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mfma -O1 -Wall -Wextra -Wno-unused-parameter -D__STRICT_ANSI__ -Wno-unknown-pragmas") # -fopenmp -Werror -m64 -frounding-math

# files
set(SOURCE_FILES main.cpp
        # shaman
        shaman/shaman/NumericalDebugger.cpp shaman/shaman/NumericalDebugger.h
        shaman/shaman/Shaman_Decl.h shaman/shaman/Shaman.h
        shaman/shaman/Shaman_eigen.h
        #shaman/shaman/Shaman_mpi.h
        # shamanizer
        shaman/shamanizer/tinyformat.h
        # tests
        tests/tests.h
        tests/exact/exact.cpp tests/exact/exact.h tests/sum.h
        #tests/mpi_test.h
        tests/schrodinger.cpp tests/schrodinger.h
        tests/cadna.h
        tests/legendre.h
        tests/eigen.h)

# linking
add_executable(shaman_demo ${SOURCE_FILES})
target_link_libraries(shaman_demo "-lstdc++ -lOpenCL -ltbb -lexblas -lcadnaC")
#target_link_libraries(shaman_demo "-lstdc++ -lm -ltbb -lexblas -lcadnaC")
