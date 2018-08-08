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

if(SHAMAN_EXAMPLES_TESTS)
    add_test(NAME test_cadna_rump COMMAND cadna_rump)
    add_test(NAME test_cadna_second_order COMMAND cadna_second_order)
    add_test(NAME test_cadna_hilbert COMMAND cadna_hilbert)
    add_test(NAME test_cadna_muller COMMAND cadna_muller)
    add_test(NAME test_cadna_newton COMMAND cadna_newton)
    add_test(NAME test_cadna_gauss COMMAND cadna_gauss)
    add_test(NAME test_cadna_jacobi COMMAND cadna_jacobi)
#    add_test(NAME test_cadna_failures COMMAND cadna_failures)
endif(SHAMAN_EXAMPLES_TESTS)