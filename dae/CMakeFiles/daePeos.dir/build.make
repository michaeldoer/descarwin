# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mustapha/framework/dae/descarwin/trunk/dae

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mustapha/framework/dae/descarwin/trunk/dae

# Include any dependencies generated for this target.
include CMakeFiles/daePeos.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/daePeos.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/daePeos.dir/flags.make

CMakeFiles/daePeos.dir/daePeos.cpp.o: CMakeFiles/daePeos.dir/flags.make
CMakeFiles/daePeos.dir/daePeos.cpp.o: daePeos.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mustapha/framework/dae/descarwin/trunk/dae/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/daePeos.dir/daePeos.cpp.o"
	/home/mustapha/framework/paradiseo-1.2.1/mpich2/bin/mpicxx   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/daePeos.dir/daePeos.cpp.o -c /home/mustapha/framework/dae/descarwin/trunk/dae/daePeos.cpp

CMakeFiles/daePeos.dir/daePeos.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/daePeos.dir/daePeos.cpp.i"
	/home/mustapha/framework/paradiseo-1.2.1/mpich2/bin/mpicxx  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/mustapha/framework/dae/descarwin/trunk/dae/daePeos.cpp > CMakeFiles/daePeos.dir/daePeos.cpp.i

CMakeFiles/daePeos.dir/daePeos.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/daePeos.dir/daePeos.cpp.s"
	/home/mustapha/framework/paradiseo-1.2.1/mpich2/bin/mpicxx  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/mustapha/framework/dae/descarwin/trunk/dae/daePeos.cpp -o CMakeFiles/daePeos.dir/daePeos.cpp.s

CMakeFiles/daePeos.dir/daePeos.cpp.o.requires:
.PHONY : CMakeFiles/daePeos.dir/daePeos.cpp.o.requires

CMakeFiles/daePeos.dir/daePeos.cpp.o.provides: CMakeFiles/daePeos.dir/daePeos.cpp.o.requires
	$(MAKE) -f CMakeFiles/daePeos.dir/build.make CMakeFiles/daePeos.dir/daePeos.cpp.o.provides.build
.PHONY : CMakeFiles/daePeos.dir/daePeos.cpp.o.provides

CMakeFiles/daePeos.dir/daePeos.cpp.o.provides.build: CMakeFiles/daePeos.dir/daePeos.cpp.o
.PHONY : CMakeFiles/daePeos.dir/daePeos.cpp.o.provides.build

# Object files for target daePeos
daePeos_OBJECTS = \
"CMakeFiles/daePeos.dir/daePeos.cpp.o"

# External object files for target daePeos
daePeos_EXTERNAL_OBJECTS =

daePeos: CMakeFiles/daePeos.dir/daePeos.cpp.o
daePeos: lib/libdaex.a
daePeos: CMakeFiles/daePeos.dir/build.make
daePeos: CMakeFiles/daePeos.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable daePeos"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/daePeos.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/daePeos.dir/build: daePeos
.PHONY : CMakeFiles/daePeos.dir/build

CMakeFiles/daePeos.dir/requires: CMakeFiles/daePeos.dir/daePeos.cpp.o.requires
.PHONY : CMakeFiles/daePeos.dir/requires

CMakeFiles/daePeos.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/daePeos.dir/cmake_clean.cmake
.PHONY : CMakeFiles/daePeos.dir/clean

CMakeFiles/daePeos.dir/depend:
	cd /home/mustapha/framework/dae/descarwin/trunk/dae && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mustapha/framework/dae/descarwin/trunk/dae /home/mustapha/framework/dae/descarwin/trunk/dae /home/mustapha/framework/dae/descarwin/trunk/dae /home/mustapha/framework/dae/descarwin/trunk/dae /home/mustapha/framework/dae/descarwin/trunk/dae/CMakeFiles/daePeos.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/daePeos.dir/depend

