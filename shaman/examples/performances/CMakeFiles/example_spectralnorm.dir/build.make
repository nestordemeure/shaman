# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /ccc/dsku/nfs-server/temp_user/cont001/ocre/demeuren/clion-2018.2.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /ccc/dsku/nfs-server/temp_user/cont001/ocre/demeuren/clion-2018.2.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman

# Include any dependencies generated for this target.
include examples/performances/CMakeFiles/example_spectralnorm.dir/depend.make

# Include the progress variables for this target.
include examples/performances/CMakeFiles/example_spectralnorm.dir/progress.make

# Include the compile flags for this target's objects.
include examples/performances/CMakeFiles/example_spectralnorm.dir/flags.make

examples/performances/CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.o: examples/performances/CMakeFiles/example_spectralnorm.dir/flags.make
examples/performances/CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.o: examples/performances/spectralnorm.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/performances/CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.o"
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/performances && /ccc/dsku/nfs-server/temp_user/cont001/ocre/demeuren/spack/opt/spack/linux-centos7-x86_64/gcc-7.3.0/llvm-7.0.0rc3-6rek55lmxxbb64s6iksq453xh5zbvyov/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.o -c /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/performances/spectralnorm.cpp

examples/performances/CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.i"
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/performances && /ccc/dsku/nfs-server/temp_user/cont001/ocre/demeuren/spack/opt/spack/linux-centos7-x86_64/gcc-7.3.0/llvm-7.0.0rc3-6rek55lmxxbb64s6iksq453xh5zbvyov/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/performances/spectralnorm.cpp > CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.i

examples/performances/CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.s"
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/performances && /ccc/dsku/nfs-server/temp_user/cont001/ocre/demeuren/spack/opt/spack/linux-centos7-x86_64/gcc-7.3.0/llvm-7.0.0rc3-6rek55lmxxbb64s6iksq453xh5zbvyov/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/performances/spectralnorm.cpp -o CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.s

# Object files for target example_spectralnorm
example_spectralnorm_OBJECTS = \
"CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.o"

# External object files for target example_spectralnorm
example_spectralnorm_EXTERNAL_OBJECTS =

examples/performances/example_spectralnorm: examples/performances/CMakeFiles/example_spectralnorm.dir/spectralnorm.cpp.o
examples/performances/example_spectralnorm: examples/performances/CMakeFiles/example_spectralnorm.dir/build.make
examples/performances/example_spectralnorm: src/libshaman.a
examples/performances/example_spectralnorm: examples/performances/CMakeFiles/example_spectralnorm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable example_spectralnorm"
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/performances && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/example_spectralnorm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/performances/CMakeFiles/example_spectralnorm.dir/build: examples/performances/example_spectralnorm

.PHONY : examples/performances/CMakeFiles/example_spectralnorm.dir/build

examples/performances/CMakeFiles/example_spectralnorm.dir/clean:
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/performances && $(CMAKE_COMMAND) -P CMakeFiles/example_spectralnorm.dir/cmake_clean.cmake
.PHONY : examples/performances/CMakeFiles/example_spectralnorm.dir/clean

examples/performances/CMakeFiles/example_spectralnorm.dir/depend:
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/performances /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/performances /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/performances/CMakeFiles/example_spectralnorm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/performances/CMakeFiles/example_spectralnorm.dir/depend

