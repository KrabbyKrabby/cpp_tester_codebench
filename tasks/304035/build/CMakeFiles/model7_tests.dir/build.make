# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035/build

# Include any dependencies generated for this target.
include CMakeFiles/model7_tests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/model7_tests.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/model7_tests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/model7_tests.dir/flags.make

CMakeFiles/model7_tests.dir/codegen:
.PHONY : CMakeFiles/model7_tests.dir/codegen

CMakeFiles/model7_tests.dir/test.cpp.o: CMakeFiles/model7_tests.dir/flags.make
CMakeFiles/model7_tests.dir/test.cpp.o: /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035/test.cpp
CMakeFiles/model7_tests.dir/test.cpp.o: CMakeFiles/model7_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/model7_tests.dir/test.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/model7_tests.dir/test.cpp.o -MF CMakeFiles/model7_tests.dir/test.cpp.o.d -o CMakeFiles/model7_tests.dir/test.cpp.o -c /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035/test.cpp

CMakeFiles/model7_tests.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/model7_tests.dir/test.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035/test.cpp > CMakeFiles/model7_tests.dir/test.cpp.i

CMakeFiles/model7_tests.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/model7_tests.dir/test.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035/test.cpp -o CMakeFiles/model7_tests.dir/test.cpp.s

# Object files for target model7_tests
model7_tests_OBJECTS = \
"CMakeFiles/model7_tests.dir/test.cpp.o"

# External object files for target model7_tests
model7_tests_EXTERNAL_OBJECTS =

model7_tests: CMakeFiles/model7_tests.dir/test.cpp.o
model7_tests: CMakeFiles/model7_tests.dir/build.make
model7_tests: lib/libgtest_main.a
model7_tests: lib/libgtest.a
model7_tests: CMakeFiles/model7_tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable model7_tests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/model7_tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/model7_tests.dir/build: model7_tests
.PHONY : CMakeFiles/model7_tests.dir/build

CMakeFiles/model7_tests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/model7_tests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/model7_tests.dir/clean

CMakeFiles/model7_tests.dir/depend:
	cd /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035 /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035 /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035/build /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035/build /Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/304035/build/CMakeFiles/model7_tests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/model7_tests.dir/depend

