add_executable(example_lulesh lulesh.cpp)
target_link_libraries(example_lulesh shaman)

add_executable(example_mandelbrot mandelbrot.cpp)
target_link_libraries(example_mandelbrot shaman)

add_executable(example_nbody nbody.cpp)
target_link_libraries(example_nbody shaman)

add_executable(example_spectralnorm spectralnorm.cpp)
target_link_libraries(example_spectralnorm shaman)

if(SHAMAN_PERFORMANCE_TESTS)
    add_test(NAME test_lulesh COMMAND example_lulesh 2)
    add_test(NAME test_mandelbrot COMMAND example_mandelbrot 50)
    add_test(NAME test_nbody COMMAND example_nbody 500)
    add_test(NAME test_spectralnorm COMMAND example_spectralnorm 100)
endif(SHAMAN_PERFORMANCE_TESTS)