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
CMAKE_SOURCE_DIR = /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae

# Include any dependencies generated for this target.
include CMakeFiles/dae.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dae.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dae.dir/flags.make

CMakeFiles/dae.dir/dae.cpp.o: CMakeFiles/dae.dir/flags.make
CMakeFiles/dae.dir/dae.cpp.o: dae.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/dae.dir/dae.cpp.o"
	/home/mustapha/framework/paradiseo-1.2.1/mpich2/bin/mpicxx   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/dae.dir/dae.cpp.o -c /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae/dae.cpp

CMakeFiles/dae.dir/dae.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dae.dir/dae.cpp.i"
	/home/mustapha/framework/paradiseo-1.2.1/mpich2/bin/mpicxx  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae/dae.cpp > CMakeFiles/dae.dir/dae.cpp.i

CMakeFiles/dae.dir/dae.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dae.dir/dae.cpp.s"
	/home/mustapha/framework/paradiseo-1.2.1/mpich2/bin/mpicxx  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae/dae.cpp -o CMakeFiles/dae.dir/dae.cpp.s

CMakeFiles/dae.dir/dae.cpp.o.requires:
.PHONY : CMakeFiles/dae.dir/dae.cpp.o.requires

CMakeFiles/dae.dir/dae.cpp.o.provides: CMakeFiles/dae.dir/dae.cpp.o.requires
	$(MAKE) -f CMakeFiles/dae.dir/build.make CMakeFiles/dae.dir/dae.cpp.o.provides.build
.PHONY : CMakeFiles/dae.dir/dae.cpp.o.provides

CMakeFiles/dae.dir/dae.cpp.o.provides.build: CMakeFiles/dae.dir/dae.cpp.o
.PHONY : CMakeFiles/dae.dir/dae.cpp.o.provides.build

# Object files for target dae
dae_OBJECTS = \
"CMakeFiles/dae.dir/dae.cpp.o"

# External object files for target dae
dae_EXTERNAL_OBJECTS =

dae: CMakeFiles/dae.dir/dae.cpp.o
dae: lib/libdaex.a
dae: CMakeFiles/dae.dir/build.make
dae: CMakeFiles/dae.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable dae"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dae.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dae.dir/build: dae
.PHONY : CMakeFiles/dae.dir/build

CMakeFiles/dae.dir/requires: CMakeFiles/dae.dir/dae.cpp.o.requires
.PHONY : CMakeFiles/dae.dir/requires

CMakeFiles/dae.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dae.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dae.dir/clean

CMakeFiles/dae.dir/depend:
	cd /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae /home/mustapha/framework/DescarwinCommit/descarwin/branches/dae/CMakeFiles/dae.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dae.dir/depend

