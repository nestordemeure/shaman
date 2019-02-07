add_library(shaman shaman/tagged/global_vars.cpp)

target_include_directories(shaman PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

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

# should be added to target_compile_options ?
if (SHAMAN_ENABLE_TAGGED_ERROR)
    target_compile_options(shaman -DSHAMAN_TAGGED_ERROR)
endif(SHAMAN_ENABLE_TAGGED_ERROR)

# should be added to target_compile_options ?
if (SHAMAN_DISABLE)
    target_compile_options(shaman -DNO_SHAMAN)
endif(SHAMAN_DISABLE)

# should be added to target_compile_options ?
if (SHAMAN_ENABLE_UNSTABLE_BRANCH)
   target_compile_options(shaman -DSHAMAN_UNSTABLE_BRANCH)
endif(SHAMAN_ENABLE_UNSTABLE_BRANCH)

# Layout. This works for all platforms:
#   * <prefix>/lib*/cmake/<PROJECT-NAME>
#   * <prefix>/lib*/
#   * <prefix>/include/
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Use:
#   * PROJECT_VERSION
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * TARGETS_EXPORT_NAME
#   * PROJECT_NAME
configure_package_config_file(
    "cmake/shaman-config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

install(FILES shaman.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES shaman/eft.h shaman/methods.h shaman/operators.h shaman/functions.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/shaman)
install(DIRECTORY shaman/helpers shaman/tagged
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/shaman)
install(TARGETS shaman EXPORT ${TARGETS_EXPORT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})


# Config
#   * <prefix>/lib/cmake/Foo/FooConfig.cmake
#   * <prefix>/lib/cmake/Foo/FooConfigVersion.cmake
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

install(EXPORT ${TARGETS_EXPORT_NAME}
        DESTINATION ${config_install_dir}
        NAMESPACE ${namespace})