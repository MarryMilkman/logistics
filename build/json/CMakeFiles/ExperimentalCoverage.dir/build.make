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
CMAKE_COMMAND = /C/all_with_prog/cmake/bin/cmake.exe

# The command to remove a file.
RM = /C/all_with_prog/cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /C/all_with_prog/UIT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /C/all_with_prog/UIT/build

# Utility rule file for ExperimentalCoverage.

# Include the progress variables for this target.
include json/CMakeFiles/ExperimentalCoverage.dir/progress.make

json/CMakeFiles/ExperimentalCoverage:
	cd /C/all_with_prog/UIT/build/json && /C/all_with_prog/cmake/bin/ctest.exe -D ExperimentalCoverage

ExperimentalCoverage: json/CMakeFiles/ExperimentalCoverage
ExperimentalCoverage: json/CMakeFiles/ExperimentalCoverage.dir/build.make

.PHONY : ExperimentalCoverage

# Rule to build all files generated by this target.
json/CMakeFiles/ExperimentalCoverage.dir/build: ExperimentalCoverage

.PHONY : json/CMakeFiles/ExperimentalCoverage.dir/build

json/CMakeFiles/ExperimentalCoverage.dir/clean:
	cd /C/all_with_prog/UIT/build/json && $(CMAKE_COMMAND) -P CMakeFiles/ExperimentalCoverage.dir/cmake_clean.cmake
.PHONY : json/CMakeFiles/ExperimentalCoverage.dir/clean

json/CMakeFiles/ExperimentalCoverage.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MSYS Makefiles" /C/all_with_prog/UIT /C/all_with_prog/UIT/json /C/all_with_prog/UIT/build /C/all_with_prog/UIT/build/json /C/all_with_prog/UIT/build/json/CMakeFiles/ExperimentalCoverage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : json/CMakeFiles/ExperimentalCoverage.dir/depend

