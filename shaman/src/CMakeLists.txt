add_library(shaman shaman/tagged/global_vars.cpp)

target_include_directories(shaman PUBLIC .)

# TODO: list all C++ features shaman relies on.
target_compile_features(shaman PUBLIC
  cxx_decltype
  cxx_auto_type
  cxx_thread_local
  cxx_std_11 # for std::fma
  )
target_compile_options(shaman PUBLIC -mfma)

if (SHAMAN_ENABLE_TESTS)
    add_subdirectory(shaman/unit_tests)
endif(SHAMAN_ENABLE_TESTS)
