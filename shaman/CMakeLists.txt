cmake_minimum_required(VERSION 3.9)
project(shaman)

# compiler
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_COMPILER g++)

# flags
#add_definitions(-DEXPLICIT_CASTING)
#add_definitions(-DNO_SHAMAN)
# to get readable perf output : -g -fno-omit-frame-pointer -fno-inline
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -fno-omit-frame-pointer -fno-inline -fopenmp -mfma -O3 -Wall -Wextra -Wno-unused-parameter -Werror -D__STRICT_ANSI__ -Wno-unknown-pragmas")

# files
set(SOURCE_FILES main.cpp
        # shaman
        shaman/src/GlobalVar.h
        shaman/src/GlobalVar.cpp
        shaman/src/Tagger.h
        shaman/src/ErrorSum.h
        shaman/Shaman.h
        shaman/src/Methods.h
        shaman/src/Operators.h
        shaman/src/Functions.h
        # tests
        #tests/test_eft.h
        #tests/test_loop.h
        # examples
        examples/various.h
        examples/schrodinger/schrodinger.cpp examples/schrodinger/schrodinger.h
        examples/legendre.h
        examples/sqrtHeron.h)

# linking
add_executable(shaman ${SOURCE_FILES})
