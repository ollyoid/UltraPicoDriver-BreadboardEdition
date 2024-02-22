# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver/build

# Utility rule file for ultra-pico-driver_phase_pio_h.

# Include any custom commands dependencies for this target.
include CMakeFiles/ultra-pico-driver_phase_pio_h.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ultra-pico-driver_phase_pio_h.dir/progress.make

CMakeFiles/ultra-pico-driver_phase_pio_h: phase.pio.h

phase.pio.h: /home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver/phase.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating phase.pio.h"
	pioasm/pioasm -o c-sdk /home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver/phase.pio /home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver/build/phase.pio.h

ultra-pico-driver_phase_pio_h: CMakeFiles/ultra-pico-driver_phase_pio_h
ultra-pico-driver_phase_pio_h: phase.pio.h
ultra-pico-driver_phase_pio_h: CMakeFiles/ultra-pico-driver_phase_pio_h.dir/build.make
.PHONY : ultra-pico-driver_phase_pio_h

# Rule to build all files generated by this target.
CMakeFiles/ultra-pico-driver_phase_pio_h.dir/build: ultra-pico-driver_phase_pio_h
.PHONY : CMakeFiles/ultra-pico-driver_phase_pio_h.dir/build

CMakeFiles/ultra-pico-driver_phase_pio_h.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ultra-pico-driver_phase_pio_h.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ultra-pico-driver_phase_pio_h.dir/clean

CMakeFiles/ultra-pico-driver_phase_pio_h.dir/depend:
	cd /home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver /home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver /home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver/build /home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver/build /home/oliver/Nextcloud/University/PhD/Acoustophoresis/Hardware/Ultra-Pico-Driver-Breadboad-Edition/firmware/ultra-pico-driver/build/CMakeFiles/ultra-pico-driver_phase_pio_h.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ultra-pico-driver_phase_pio_h.dir/depend

