# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gaodc/TDD_C

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gaodc/TDD_C/build

# Include any dependencies generated for this target.
include test/CMakeFiles/test_tdd.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/test_tdd.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_tdd.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_tdd.dir/flags.make

test/CMakeFiles/test_tdd.dir/try.cpp.o: test/CMakeFiles/test_tdd.dir/flags.make
test/CMakeFiles/test_tdd.dir/try.cpp.o: ../test/try.cpp
test/CMakeFiles/test_tdd.dir/try.cpp.o: test/CMakeFiles/test_tdd.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_tdd.dir/try.cpp.o"
	cd /home/gaodc/TDD_C/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/test_tdd.dir/try.cpp.o -MF CMakeFiles/test_tdd.dir/try.cpp.o.d -o CMakeFiles/test_tdd.dir/try.cpp.o -c /home/gaodc/TDD_C/test/try.cpp

test/CMakeFiles/test_tdd.dir/try.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_tdd.dir/try.cpp.i"
	cd /home/gaodc/TDD_C/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/test/try.cpp > CMakeFiles/test_tdd.dir/try.cpp.i

test/CMakeFiles/test_tdd.dir/try.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_tdd.dir/try.cpp.s"
	cd /home/gaodc/TDD_C/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/test/try.cpp -o CMakeFiles/test_tdd.dir/try.cpp.s

# Object files for target test_tdd
test_tdd_OBJECTS = \
"CMakeFiles/test_tdd.dir/try.cpp.o"

# External object files for target test_tdd
test_tdd_EXTERNAL_OBJECTS =

test/test_tdd: test/CMakeFiles/test_tdd.dir/try.cpp.o
test/test_tdd: test/CMakeFiles/test_tdd.dir/build.make
test/test_tdd: test/CMakeFiles/test_tdd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_tdd"
	cd /home/gaodc/TDD_C/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_tdd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_tdd.dir/build: test/test_tdd
.PHONY : test/CMakeFiles/test_tdd.dir/build

test/CMakeFiles/test_tdd.dir/clean:
	cd /home/gaodc/TDD_C/build/test && $(CMAKE_COMMAND) -P CMakeFiles/test_tdd.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_tdd.dir/clean

test/CMakeFiles/test_tdd.dir/depend:
	cd /home/gaodc/TDD_C/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gaodc/TDD_C /home/gaodc/TDD_C/test /home/gaodc/TDD_C/build /home/gaodc/TDD_C/build/test /home/gaodc/TDD_C/build/test/CMakeFiles/test_tdd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_tdd.dir/depend
