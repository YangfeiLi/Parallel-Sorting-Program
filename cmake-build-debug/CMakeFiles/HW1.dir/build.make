# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/HW1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HW1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HW1.dir/flags.make

CMakeFiles/HW1.dir/makeinput.cc.o: CMakeFiles/HW1.dir/flags.make
CMakeFiles/HW1.dir/makeinput.cc.o: ../makeinput.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HW1.dir/makeinput.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HW1.dir/makeinput.cc.o -c /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/makeinput.cc

CMakeFiles/HW1.dir/makeinput.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HW1.dir/makeinput.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/makeinput.cc > CMakeFiles/HW1.dir/makeinput.cc.i

CMakeFiles/HW1.dir/makeinput.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HW1.dir/makeinput.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/makeinput.cc -o CMakeFiles/HW1.dir/makeinput.cc.s

CMakeFiles/HW1.dir/makeinput.cc.o.requires:

.PHONY : CMakeFiles/HW1.dir/makeinput.cc.o.requires

CMakeFiles/HW1.dir/makeinput.cc.o.provides: CMakeFiles/HW1.dir/makeinput.cc.o.requires
	$(MAKE) -f CMakeFiles/HW1.dir/build.make CMakeFiles/HW1.dir/makeinput.cc.o.provides.build
.PHONY : CMakeFiles/HW1.dir/makeinput.cc.o.provides

CMakeFiles/HW1.dir/makeinput.cc.o.provides.build: CMakeFiles/HW1.dir/makeinput.cc.o


CMakeFiles/HW1.dir/mysort.cc.o: CMakeFiles/HW1.dir/flags.make
CMakeFiles/HW1.dir/mysort.cc.o: ../mysort.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/HW1.dir/mysort.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HW1.dir/mysort.cc.o -c /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/mysort.cc

CMakeFiles/HW1.dir/mysort.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HW1.dir/mysort.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/mysort.cc > CMakeFiles/HW1.dir/mysort.cc.i

CMakeFiles/HW1.dir/mysort.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HW1.dir/mysort.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/mysort.cc -o CMakeFiles/HW1.dir/mysort.cc.s

CMakeFiles/HW1.dir/mysort.cc.o.requires:

.PHONY : CMakeFiles/HW1.dir/mysort.cc.o.requires

CMakeFiles/HW1.dir/mysort.cc.o.provides: CMakeFiles/HW1.dir/mysort.cc.o.requires
	$(MAKE) -f CMakeFiles/HW1.dir/build.make CMakeFiles/HW1.dir/mysort.cc.o.provides.build
.PHONY : CMakeFiles/HW1.dir/mysort.cc.o.provides

CMakeFiles/HW1.dir/mysort.cc.o.provides.build: CMakeFiles/HW1.dir/mysort.cc.o


# Object files for target HW1
HW1_OBJECTS = \
"CMakeFiles/HW1.dir/makeinput.cc.o" \
"CMakeFiles/HW1.dir/mysort.cc.o"

# External object files for target HW1
HW1_EXTERNAL_OBJECTS =

HW1: CMakeFiles/HW1.dir/makeinput.cc.o
HW1: CMakeFiles/HW1.dir/mysort.cc.o
HW1: CMakeFiles/HW1.dir/build.make
HW1: CMakeFiles/HW1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable HW1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HW1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HW1.dir/build: HW1

.PHONY : CMakeFiles/HW1.dir/build

CMakeFiles/HW1.dir/requires: CMakeFiles/HW1.dir/makeinput.cc.o.requires
CMakeFiles/HW1.dir/requires: CMakeFiles/HW1.dir/mysort.cc.o.requires

.PHONY : CMakeFiles/HW1.dir/requires

CMakeFiles/HW1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HW1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HW1.dir/clean

CMakeFiles/HW1.dir/depend:
	cd /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1 /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1 /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/cmake-build-debug /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/cmake-build-debug /Users/Apple/Desktop/Courses/505/505sharedfolder/HW1/cmake-build-debug/CMakeFiles/HW1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HW1.dir/depend
