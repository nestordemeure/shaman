add_executable(example_lulesh lulesh.cpp)
target_link_libraries(example_lulesh shaman)

add_executable(example_mandelbrot mandelbrot.cpp)
target_link_libraries(example_mandelbrot shaman)

add_executable(example_nbody nbody.cpp)
target_link_libraries(example_nbody shaman)

add_executable(example_spectralnorm spectralnorm.cpp)
target_link_libraries(example_spectralnorm shaman)