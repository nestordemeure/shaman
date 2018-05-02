cmake_minimum_required(VERSION 3.9)
project(shaman)

# compiler
set(CMAKE_CXX_STANDARD 11)
# mpi
#set(CMAKE_CXX_COMPILER /usr/local/libre/bin/mpic++)

# flags
add_definitions(-DNUMERICAL_DEBUGGER)
#add_definitions(-DNUMERICAL_ZERO_DEBUGGER)
#add_definitions(-DEXPLICIT_CASTING)
#add_definitions(-DNO_SHAMAN)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fopenmp -mfma -O1 -Wall -Wextra -Wno-unused-parameter -Werror -D__STRICT_ANSI__ -Wno-unknown-pragmas")

# files
set(SOURCE_FILES main.cpp
        # shaman
        shaman/src/NumericalDebugger.cpp shaman/src/NumericalDebugger.h
        shaman/src/Shaman_Decl.h
        shaman/Shaman.h
        # examples
        examples/tests.h
        examples/schrodinger.cpp examples/schrodinger.h
        examples/legendre.h)

# linking
add_executable(shaman ${SOURCE_FILES})
#target_link_libraries(shaman "-lstdc++ -lOpenCL -ltbb -lexblas -lcadnaC")
#target_link_libraries(shaman_demo "-lstdc++ -lm -ltbb -lexblas -lcadnaC")
