add_library(shaman shaman/tagged/GlobalVar.cpp)

target_include_directories(shaman PUBLIC .)
target_compile_features(shaman PUBLIC cxx_decltype)
target_compile_options(shaman PUBLIC -mfma)