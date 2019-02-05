find_package(GTest)

if (GTest_FOUND)
    include(GoogleTest)

    add_executable(shaman_unittests test_eft.cc)
    target_link_libraries(shaman_unittests GTest::Main)

    target_compile_features(shaman_unittests PUBLIC
    cxx_std_11 # for std::fma
    cxx_binary_literals
    )

gtest_discover_tests(shaman_unittests TEST_PREFIX unit:)
endif(GTest_FOUND)
