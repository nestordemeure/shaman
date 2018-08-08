find_package(GTest REQUIRED)

include(GoogleTest)

add_executable(shaman_unittests test_eft.cc)
target_link_libraries(shaman_unittests GTest::Main)

gtest_discover_tests(shaman_unittests TEST_PREFIX unit:)
