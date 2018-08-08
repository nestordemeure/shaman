add_executable(cadna_rump rump_polynomial.cpp)
target_link_libraries(cadna_rump shaman)

add_executable(cadna_second_order second_order.cpp)
target_link_libraries(cadna_second_order shaman)

add_executable(cadna_hilbert hilbert_determinant.cpp)
target_link_libraries(cadna_hilbert shaman)

add_executable(cadna_muller muller.cpp)
target_link_libraries(cadna_muller shaman)

add_executable(cadna_newton newton.cpp)
target_link_libraries(cadna_newton shaman)

add_executable(cadna_gauss gauss.cpp)
target_link_libraries(cadna_gauss shaman)

add_executable(cadna_jacobi jacobi.cpp)
target_link_libraries(cadna_jacobi shaman)

add_executable(cadna_failures cadna_failures.cpp)
target_link_libraries(cadna_failures shaman)