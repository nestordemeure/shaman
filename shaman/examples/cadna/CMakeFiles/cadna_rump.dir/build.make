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
CMAKE_COMMAND = /ccc/dsku/nfs-server/temp_user/cont001/ocre/demeuren/cmake/installdir/bin/cmake

# The command to remove a file.
RM = /ccc/dsku/nfs-server/temp_user/cont001/ocre/demeuren/cmake/installdir/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman

# Include any dependencies generated for this target.
include examples/cadna/CMakeFiles/cadna_rump.dir/depend.make

# Include the progress variables for this target.
include examples/cadna/CMakeFiles/cadna_rump.dir/progress.make

# Include the compile flags for this target's objects.
include examples/cadna/CMakeFiles/cadna_rump.dir/flags.make

examples/cadna/CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.o: examples/cadna/CMakeFiles/cadna_rump.dir/flags.make
examples/cadna/CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.o: examples/cadna/rump_polynomial.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/cadna/CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.o"
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/cadna && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.o -c /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/cadna/rump_polynomial.cpp

examples/cadna/CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.i"
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/cadna && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/cadna/rump_polynomial.cpp > CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.i

examples/cadna/CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.s"
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/cadna && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/cadna/rump_polynomial.cpp -o CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.s

# Object files for target cadna_rump
cadna_rump_OBJECTS = \
"CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.o"

# External object files for target cadna_rump
cadna_rump_EXTERNAL_OBJECTS =

examples/cadna/cadna_rump: examples/cadna/CMakeFiles/cadna_rump.dir/rump_polynomial.cpp.o
examples/cadna/cadna_rump: examples/cadna/CMakeFiles/cadna_rump.dir/build.make
examples/cadna/cadna_rump: src/libshaman.a
examples/cadna/cadna_rump: examples/cadna/CMakeFiles/cadna_rump.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cadna_rump"
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/cadna && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cadna_rump.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/cadna/CMakeFiles/cadna_rump.dir/build: examples/cadna/cadna_rump

.PHONY : examples/cadna/CMakeFiles/cadna_rump.dir/build

examples/cadna/CMakeFiles/cadna_rump.dir/clean:
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/cadna && $(CMAKE_COMMAND) -P CMakeFiles/cadna_rump.dir/cmake_clean.cmake
.PHONY : examples/cadna/CMakeFiles/cadna_rump.dir/clean

examples/cadna/CMakeFiles/cadna_rump.dir/depend:
	cd /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/cadna /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/cadna /ccc/home/cont001/ocre/demeuren/thesis/code/shaman/shaman/examples/cadna/CMakeFiles/cadna_rump.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/cadna/CMakeFiles/cadna_rump.dir/depend
