# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/12.48

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/12.48/build

# Utility rule file for example.

# Include the progress variables for this target.
include CMakeFiles/example.dir/progress.make

CMakeFiles/example:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pi/12.48/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Run examples"
	cd /home/pi/12.48/build/bin && ./epaper_example

example: CMakeFiles/example
example: CMakeFiles/example.dir/build.make

.PHONY : example

# Rule to build all files generated by this target.
CMakeFiles/example.dir/build: example

.PHONY : CMakeFiles/example.dir/build

CMakeFiles/example.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/example.dir/cmake_clean.cmake
.PHONY : CMakeFiles/example.dir/clean

CMakeFiles/example.dir/depend:
	cd /home/pi/12.48/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/12.48 /home/pi/12.48 /home/pi/12.48/build /home/pi/12.48/build /home/pi/12.48/build/CMakeFiles/example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/example.dir/depend

