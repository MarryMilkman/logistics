# CMAKE generated file: DO NOT EDIT!
# Generated by "MSYS Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /C/all_with_prog/plugins/cmake/bin/cmake.exe

# The command to remove a file.
RM = /C/all_with_prog/plugins/cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /C/all_with_prog/logistics

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /C/all_with_prog/logistics/build

# Utility rule file for ContinuousBuild.

# Include the progress variables for this target.
include json/CMakeFiles/ContinuousBuild.dir/progress.make

json/CMakeFiles/ContinuousBuild:
	cd /C/all_with_prog/logistics/build/json && /C/all_with_prog/plugins/cmake/bin/ctest.exe -D ContinuousBuild

ContinuousBuild: json/CMakeFiles/ContinuousBuild
ContinuousBuild: json/CMakeFiles/ContinuousBuild.dir/build.make

.PHONY : ContinuousBuild

# Rule to build all files generated by this target.
json/CMakeFiles/ContinuousBuild.dir/build: ContinuousBuild

.PHONY : json/CMakeFiles/ContinuousBuild.dir/build

json/CMakeFiles/ContinuousBuild.dir/clean:
	cd /C/all_with_prog/logistics/build/json && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousBuild.dir/cmake_clean.cmake
.PHONY : json/CMakeFiles/ContinuousBuild.dir/clean

json/CMakeFiles/ContinuousBuild.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MSYS Makefiles" /C/all_with_prog/logistics /C/all_with_prog/logistics/json /C/all_with_prog/logistics/build /C/all_with_prog/logistics/build/json /C/all_with_prog/logistics/build/json/CMakeFiles/ContinuousBuild.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : json/CMakeFiles/ContinuousBuild.dir/depend

