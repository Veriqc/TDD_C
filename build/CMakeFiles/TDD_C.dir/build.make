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
include CMakeFiles/TDD_C.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TDD_C.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TDD_C.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TDD_C.dir/flags.make

CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.o: ../include/CircuitOptimizer.cpp
CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.o -MF CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.o.d -o CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.o -c /home/gaodc/TDD_C/include/CircuitOptimizer.cpp

CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/CircuitOptimizer.cpp > CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.i

CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/CircuitOptimizer.cpp -o CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.s

CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.o: ../include/dd/Edge.cpp
CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.o -MF CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.o.d -o CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.o -c /home/gaodc/TDD_C/include/dd/Edge.cpp

CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/dd/Edge.cpp > CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.i

CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/dd/Edge.cpp -o CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.s

CMakeFiles/TDD_C.dir/include/dd/Node.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/dd/Node.cpp.o: ../include/dd/Node.cpp
CMakeFiles/TDD_C.dir/include/dd/Node.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TDD_C.dir/include/dd/Node.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/dd/Node.cpp.o -MF CMakeFiles/TDD_C.dir/include/dd/Node.cpp.o.d -o CMakeFiles/TDD_C.dir/include/dd/Node.cpp.o -c /home/gaodc/TDD_C/include/dd/Node.cpp

CMakeFiles/TDD_C.dir/include/dd/Node.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/dd/Node.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/dd/Node.cpp > CMakeFiles/TDD_C.dir/include/dd/Node.cpp.i

CMakeFiles/TDD_C.dir/include/dd/Node.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/dd/Node.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/dd/Node.cpp -o CMakeFiles/TDD_C.dir/include/dd/Node.cpp.s

CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.o: ../include/dd/Operations.cpp
CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.o -MF CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.o.d -o CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.o -c /home/gaodc/TDD_C/include/dd/Operations.cpp

CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/dd/Operations.cpp > CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.i

CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/dd/Operations.cpp -o CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.s

CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.o: ../include/operations/Expression.cpp
CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.o -MF CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.o.d -o CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.o -c /home/gaodc/TDD_C/include/operations/Expression.cpp

CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/operations/Expression.cpp > CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.i

CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/operations/Expression.cpp -o CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.s

CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.o: ../include/operations/NonUnitaryOperation.cpp
CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.o -MF CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.o.d -o CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.o -c /home/gaodc/TDD_C/include/operations/NonUnitaryOperation.cpp

CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/operations/NonUnitaryOperation.cpp > CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.i

CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/operations/NonUnitaryOperation.cpp -o CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.s

CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.o: ../include/operations/Operation.cpp
CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.o -MF CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.o.d -o CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.o -c /home/gaodc/TDD_C/include/operations/Operation.cpp

CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/operations/Operation.cpp > CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.i

CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/operations/Operation.cpp -o CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.s

CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.o: ../include/operations/StandardOperation.cpp
CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.o -MF CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.o.d -o CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.o -c /home/gaodc/TDD_C/include/operations/StandardOperation.cpp

CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/operations/StandardOperation.cpp > CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.i

CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/operations/StandardOperation.cpp -o CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.s

CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.o: ../include/operations/SymbolicOperation.cpp
CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.o -MF CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.o.d -o CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.o -c /home/gaodc/TDD_C/include/operations/SymbolicOperation.cpp

CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/operations/SymbolicOperation.cpp > CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.i

CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/operations/SymbolicOperation.cpp -o CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.s

CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.o: ../include/parsers/GRCSParser.cpp
CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.o -MF CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.o.d -o CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.o -c /home/gaodc/TDD_C/include/parsers/GRCSParser.cpp

CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/parsers/GRCSParser.cpp > CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.i

CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/parsers/GRCSParser.cpp -o CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.s

CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.o: ../include/parsers/qasm_parser/Parser.cpp
CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.o -MF CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.o.d -o CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.o -c /home/gaodc/TDD_C/include/parsers/qasm_parser/Parser.cpp

CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/parsers/qasm_parser/Parser.cpp > CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.i

CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/parsers/qasm_parser/Parser.cpp -o CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.s

CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.o: ../include/parsers/qasm_parser/Scanner.cpp
CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.o -MF CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.o.d -o CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.o -c /home/gaodc/TDD_C/include/parsers/qasm_parser/Scanner.cpp

CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/parsers/qasm_parser/Scanner.cpp > CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.i

CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/parsers/qasm_parser/Scanner.cpp -o CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.s

CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.o: ../include/parsers/QASMParser.cpp
CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.o -MF CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.o.d -o CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.o -c /home/gaodc/TDD_C/include/parsers/QASMParser.cpp

CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/parsers/QASMParser.cpp > CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.i

CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/parsers/QASMParser.cpp -o CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.s

CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.o: ../include/parsers/QCParser.cpp
CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.o -MF CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.o.d -o CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.o -c /home/gaodc/TDD_C/include/parsers/QCParser.cpp

CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/parsers/QCParser.cpp > CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.i

CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/parsers/QCParser.cpp -o CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.s

CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.o: ../include/parsers/RealParser.cpp
CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.o -MF CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.o.d -o CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.o -c /home/gaodc/TDD_C/include/parsers/RealParser.cpp

CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/parsers/RealParser.cpp > CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.i

CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/parsers/RealParser.cpp -o CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.s

CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.o: ../include/parsers/TFCParser.cpp
CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.o -MF CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.o.d -o CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.o -c /home/gaodc/TDD_C/include/parsers/TFCParser.cpp

CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/parsers/TFCParser.cpp > CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.i

CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/parsers/TFCParser.cpp -o CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.s

CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.o: ../include/QuantumComputation.cpp
CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.o -MF CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.o.d -o CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.o -c /home/gaodc/TDD_C/include/QuantumComputation.cpp

CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/include/QuantumComputation.cpp > CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.i

CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/include/QuantumComputation.cpp -o CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.s

CMakeFiles/TDD_C.dir/main.cpp.o: CMakeFiles/TDD_C.dir/flags.make
CMakeFiles/TDD_C.dir/main.cpp.o: ../main.cpp
CMakeFiles/TDD_C.dir/main.cpp.o: CMakeFiles/TDD_C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object CMakeFiles/TDD_C.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TDD_C.dir/main.cpp.o -MF CMakeFiles/TDD_C.dir/main.cpp.o.d -o CMakeFiles/TDD_C.dir/main.cpp.o -c /home/gaodc/TDD_C/main.cpp

CMakeFiles/TDD_C.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TDD_C.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaodc/TDD_C/main.cpp > CMakeFiles/TDD_C.dir/main.cpp.i

CMakeFiles/TDD_C.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TDD_C.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaodc/TDD_C/main.cpp -o CMakeFiles/TDD_C.dir/main.cpp.s

# Object files for target TDD_C
TDD_C_OBJECTS = \
"CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.o" \
"CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.o" \
"CMakeFiles/TDD_C.dir/include/dd/Node.cpp.o" \
"CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.o" \
"CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.o" \
"CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.o" \
"CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.o" \
"CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.o" \
"CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.o" \
"CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.o" \
"CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.o" \
"CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.o" \
"CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.o" \
"CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.o" \
"CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.o" \
"CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.o" \
"CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.o" \
"CMakeFiles/TDD_C.dir/main.cpp.o"

# External object files for target TDD_C
TDD_C_EXTERNAL_OBJECTS =

TDD_C: CMakeFiles/TDD_C.dir/include/CircuitOptimizer.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/dd/Edge.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/dd/Node.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/dd/Operations.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/operations/Expression.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/operations/NonUnitaryOperation.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/operations/Operation.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/operations/StandardOperation.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/operations/SymbolicOperation.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/parsers/GRCSParser.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Parser.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/parsers/qasm_parser/Scanner.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/parsers/QASMParser.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/parsers/QCParser.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/parsers/RealParser.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/parsers/TFCParser.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/include/QuantumComputation.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/main.cpp.o
TDD_C: CMakeFiles/TDD_C.dir/build.make
TDD_C: CMakeFiles/TDD_C.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gaodc/TDD_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Linking CXX executable TDD_C"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TDD_C.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TDD_C.dir/build: TDD_C
.PHONY : CMakeFiles/TDD_C.dir/build

CMakeFiles/TDD_C.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TDD_C.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TDD_C.dir/clean

CMakeFiles/TDD_C.dir/depend:
	cd /home/gaodc/TDD_C/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gaodc/TDD_C /home/gaodc/TDD_C /home/gaodc/TDD_C/build /home/gaodc/TDD_C/build /home/gaodc/TDD_C/build/CMakeFiles/TDD_C.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TDD_C.dir/depend

