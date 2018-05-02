cmake_minimum_required(VERSION 3.9)
project(shaman)

# compiler
set(CMAKE_CXX_STANDARD 11)

# flags
add_definitions(-DNUMERICAL_DEBUGGER)
#add_definitions(-DNUMERICAL_ZERO_DEBUGGER)
#add_definitions(-DEXPLICIT_CASTING)
#add_definitions(-DNO_SHAMAN)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fopenmp -mfma -O1 -Wall -Wextra -Wno-unused-parameter -Werror -D__STRICT_ANSI__ -Wno-unknown-pragmas")

# files
set(SOURCE_FILES main.cpp
        # shaman
        shaman/Shaman.h
        # tests
        tests/test_eft.h
        # examples
        examples/tests.h
        examples/schrodinger.cpp examples/schrodinger.h
        examples/legendre.h shaman/src/Eft.h shaman/src/Operators.h shaman/src/Functions.h shaman/src/Methods.h)

# linking
add_executable(shaman ${SOURCE_FILES})
