find_package(Kokkos REQUIRED)

add_executable(kokkos_simple simple_example.cpp)
#target_link_libraries(kokkos_simple PUBLIC shaman)
target_link_libraries(kokkos_simple PUBLIC Kokkos::kokkos)
