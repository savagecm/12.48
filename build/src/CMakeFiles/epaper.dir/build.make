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

# Include any dependencies generated for this target.
include src/CMakeFiles/epaper.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/epaper.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/epaper.dir/flags.make

src/CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.o: src/CMakeFiles/epaper.dir/flags.make
src/CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.o: ../src/GUI/GUI_BMPfile.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/12.48/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.o"
	cd /home/pi/12.48/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.o -c /home/pi/12.48/src/GUI/GUI_BMPfile.cpp

src/CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.i"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/12.48/src/GUI/GUI_BMPfile.cpp > CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.i

src/CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.s"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/12.48/src/GUI/GUI_BMPfile.cpp -o CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.s

src/CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.o: src/CMakeFiles/epaper.dir/flags.make
src/CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.o: ../src/GUI/GUI_Paint.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/12.48/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.o"
	cd /home/pi/12.48/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.o -c /home/pi/12.48/src/GUI/GUI_Paint.cpp

src/CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.i"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/12.48/src/GUI/GUI_Paint.cpp > CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.i

src/CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.s"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/12.48/src/GUI/GUI_Paint.cpp -o CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.s

src/CMakeFiles/epaper.dir/config/DEV_Config.cpp.o: src/CMakeFiles/epaper.dir/flags.make
src/CMakeFiles/epaper.dir/config/DEV_Config.cpp.o: ../src/config/DEV_Config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/12.48/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/epaper.dir/config/DEV_Config.cpp.o"
	cd /home/pi/12.48/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epaper.dir/config/DEV_Config.cpp.o -c /home/pi/12.48/src/config/DEV_Config.cpp

src/CMakeFiles/epaper.dir/config/DEV_Config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epaper.dir/config/DEV_Config.cpp.i"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/12.48/src/config/DEV_Config.cpp > CMakeFiles/epaper.dir/config/DEV_Config.cpp.i

src/CMakeFiles/epaper.dir/config/DEV_Config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epaper.dir/config/DEV_Config.cpp.s"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/12.48/src/config/DEV_Config.cpp -o CMakeFiles/epaper.dir/config/DEV_Config.cpp.s

src/CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.o: src/CMakeFiles/epaper.dir/flags.make
src/CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.o: ../src/config/RPI_sysfs_gpio.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/12.48/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.o"
	cd /home/pi/12.48/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.o -c /home/pi/12.48/src/config/RPI_sysfs_gpio.cpp

src/CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.i"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/12.48/src/config/RPI_sysfs_gpio.cpp > CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.i

src/CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.s"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/12.48/src/config/RPI_sysfs_gpio.cpp -o CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.s

src/CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.o: src/CMakeFiles/epaper.dir/flags.make
src/CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.o: ../src/ePaper/EPD_12in48.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/12.48/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.o"
	cd /home/pi/12.48/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.o -c /home/pi/12.48/src/ePaper/EPD_12in48.cpp

src/CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.i"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/12.48/src/ePaper/EPD_12in48.cpp > CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.i

src/CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.s"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/12.48/src/ePaper/EPD_12in48.cpp -o CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.s

src/CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.o: src/CMakeFiles/epaper.dir/flags.make
src/CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.o: ../src/ePaper/EPD_12in48b.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/12.48/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.o"
	cd /home/pi/12.48/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.o -c /home/pi/12.48/src/ePaper/EPD_12in48b.cpp

src/CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.i"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/12.48/src/ePaper/EPD_12in48b.cpp > CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.i

src/CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.s"
	cd /home/pi/12.48/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/12.48/src/ePaper/EPD_12in48b.cpp -o CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.s

# Object files for target epaper
epaper_OBJECTS = \
"CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.o" \
"CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.o" \
"CMakeFiles/epaper.dir/config/DEV_Config.cpp.o" \
"CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.o" \
"CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.o" \
"CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.o"

# External object files for target epaper
epaper_EXTERNAL_OBJECTS =

../lib/libepaper.so.1.0.0: src/CMakeFiles/epaper.dir/GUI/GUI_BMPfile.cpp.o
../lib/libepaper.so.1.0.0: src/CMakeFiles/epaper.dir/GUI/GUI_Paint.cpp.o
../lib/libepaper.so.1.0.0: src/CMakeFiles/epaper.dir/config/DEV_Config.cpp.o
../lib/libepaper.so.1.0.0: src/CMakeFiles/epaper.dir/config/RPI_sysfs_gpio.cpp.o
../lib/libepaper.so.1.0.0: src/CMakeFiles/epaper.dir/ePaper/EPD_12in48.cpp.o
../lib/libepaper.so.1.0.0: src/CMakeFiles/epaper.dir/ePaper/EPD_12in48b.cpp.o
../lib/libepaper.so.1.0.0: src/CMakeFiles/epaper.dir/build.make
../lib/libepaper.so.1.0.0: src/CMakeFiles/epaper.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/12.48/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX shared library ../../lib/libepaper.so"
	cd /home/pi/12.48/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/epaper.dir/link.txt --verbose=$(VERBOSE)
	cd /home/pi/12.48/build/src && $(CMAKE_COMMAND) -E cmake_symlink_library ../../lib/libepaper.so.1.0.0 ../../lib/libepaper.so.1 ../../lib/libepaper.so

../lib/libepaper.so.1: ../lib/libepaper.so.1.0.0
	@$(CMAKE_COMMAND) -E touch_nocreate ../lib/libepaper.so.1

../lib/libepaper.so: ../lib/libepaper.so.1.0.0
	@$(CMAKE_COMMAND) -E touch_nocreate ../lib/libepaper.so

# Rule to build all files generated by this target.
src/CMakeFiles/epaper.dir/build: ../lib/libepaper.so

.PHONY : src/CMakeFiles/epaper.dir/build

src/CMakeFiles/epaper.dir/clean:
	cd /home/pi/12.48/build/src && $(CMAKE_COMMAND) -P CMakeFiles/epaper.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/epaper.dir/clean

src/CMakeFiles/epaper.dir/depend:
	cd /home/pi/12.48/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/12.48 /home/pi/12.48/src /home/pi/12.48/build /home/pi/12.48/build/src /home/pi/12.48/build/src/CMakeFiles/epaper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/epaper.dir/depend
