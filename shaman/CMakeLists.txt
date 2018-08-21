cmake_minimum_required(VERSION 3.9)
project(shaman VERSION 0.1)

option(SHAMAN_ENABLE_TESTS "Whether or not Shaman tests are run" ON)
option(SHAMAN_ENABLE_EXAMPLES "Whether or not Shaman examples are built" ON)

if (SHAMAN_ENABLE_TESTS)
    enable_testing()
    SET(SHAMAN_EXAMPLES_TESTS ON)
    SET(SHAMAN_PERFORMANCE_TESTS ON)
endif(SHAMAN_ENABLE_TESTS)

add_subdirectory(src)

if (SHAMAN_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif(SHAMAN_ENABLE_EXAMPLES)