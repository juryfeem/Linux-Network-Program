# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/dete/Documents/Epoll_ET

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dete/Documents/Epoll_ET/build

# Include any dependencies generated for this target.
include CMakeFiles/epoll_ET_on_pipe.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/epoll_ET_on_pipe.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/epoll_ET_on_pipe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/epoll_ET_on_pipe.dir/flags.make

CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.o: CMakeFiles/epoll_ET_on_pipe.dir/flags.make
CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.o: ../epoll_ET_on_pipe.c
CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.o: CMakeFiles/epoll_ET_on_pipe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dete/Documents/Epoll_ET/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.o -MF CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.o.d -o CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.o -c /home/dete/Documents/Epoll_ET/epoll_ET_on_pipe.c

CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dete/Documents/Epoll_ET/epoll_ET_on_pipe.c > CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.i

CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dete/Documents/Epoll_ET/epoll_ET_on_pipe.c -o CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.s

# Object files for target epoll_ET_on_pipe
epoll_ET_on_pipe_OBJECTS = \
"CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.o"

# External object files for target epoll_ET_on_pipe
epoll_ET_on_pipe_EXTERNAL_OBJECTS =

epoll_ET_on_pipe: CMakeFiles/epoll_ET_on_pipe.dir/epoll_ET_on_pipe.c.o
epoll_ET_on_pipe: CMakeFiles/epoll_ET_on_pipe.dir/build.make
epoll_ET_on_pipe: CMakeFiles/epoll_ET_on_pipe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dete/Documents/Epoll_ET/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable epoll_ET_on_pipe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/epoll_ET_on_pipe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/epoll_ET_on_pipe.dir/build: epoll_ET_on_pipe
.PHONY : CMakeFiles/epoll_ET_on_pipe.dir/build

CMakeFiles/epoll_ET_on_pipe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/epoll_ET_on_pipe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/epoll_ET_on_pipe.dir/clean

CMakeFiles/epoll_ET_on_pipe.dir/depend:
	cd /home/dete/Documents/Epoll_ET/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dete/Documents/Epoll_ET /home/dete/Documents/Epoll_ET /home/dete/Documents/Epoll_ET/build /home/dete/Documents/Epoll_ET/build /home/dete/Documents/Epoll_ET/build/CMakeFiles/epoll_ET_on_pipe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/epoll_ET_on_pipe.dir/depend
