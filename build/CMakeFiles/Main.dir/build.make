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
CMAKE_SOURCE_DIR = /home/cuifh/own/OO

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cuifh/own/OO/build

# Include any dependencies generated for this target.
include CMakeFiles/Main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Main.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Main.dir/flags.make

CMakeFiles/Main.dir/src/Shader.cpp.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/src/Shader.cpp.o: ../src/Shader.cpp
CMakeFiles/Main.dir/src/Shader.cpp.o: CMakeFiles/Main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cuifh/own/OO/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Main.dir/src/Shader.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Main.dir/src/Shader.cpp.o -MF CMakeFiles/Main.dir/src/Shader.cpp.o.d -o CMakeFiles/Main.dir/src/Shader.cpp.o -c /home/cuifh/own/OO/src/Shader.cpp

CMakeFiles/Main.dir/src/Shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/src/Shader.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cuifh/own/OO/src/Shader.cpp > CMakeFiles/Main.dir/src/Shader.cpp.i

CMakeFiles/Main.dir/src/Shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/src/Shader.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cuifh/own/OO/src/Shader.cpp -o CMakeFiles/Main.dir/src/Shader.cpp.s

CMakeFiles/Main.dir/src/main.cpp.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/Main.dir/src/main.cpp.o: CMakeFiles/Main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cuifh/own/OO/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Main.dir/src/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Main.dir/src/main.cpp.o -MF CMakeFiles/Main.dir/src/main.cpp.o.d -o CMakeFiles/Main.dir/src/main.cpp.o -c /home/cuifh/own/OO/src/main.cpp

CMakeFiles/Main.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/src/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cuifh/own/OO/src/main.cpp > CMakeFiles/Main.dir/src/main.cpp.i

CMakeFiles/Main.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/src/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cuifh/own/OO/src/main.cpp -o CMakeFiles/Main.dir/src/main.cpp.s

CMakeFiles/Main.dir/external/glad/src/glad.c.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/external/glad/src/glad.c.o: ../external/glad/src/glad.c
CMakeFiles/Main.dir/external/glad/src/glad.c.o: CMakeFiles/Main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cuifh/own/OO/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Main.dir/external/glad/src/glad.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Main.dir/external/glad/src/glad.c.o -MF CMakeFiles/Main.dir/external/glad/src/glad.c.o.d -o CMakeFiles/Main.dir/external/glad/src/glad.c.o -c /home/cuifh/own/OO/external/glad/src/glad.c

CMakeFiles/Main.dir/external/glad/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Main.dir/external/glad/src/glad.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cuifh/own/OO/external/glad/src/glad.c > CMakeFiles/Main.dir/external/glad/src/glad.c.i

CMakeFiles/Main.dir/external/glad/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Main.dir/external/glad/src/glad.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cuifh/own/OO/external/glad/src/glad.c -o CMakeFiles/Main.dir/external/glad/src/glad.c.s

CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.o: ../external/stb_image/src/stb_image.cpp
CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.o: CMakeFiles/Main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cuifh/own/OO/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.o -MF CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.o.d -o CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.o -c /home/cuifh/own/OO/external/stb_image/src/stb_image.cpp

CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cuifh/own/OO/external/stb_image/src/stb_image.cpp > CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.i

CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cuifh/own/OO/external/stb_image/src/stb_image.cpp -o CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.s

# Object files for target Main
Main_OBJECTS = \
"CMakeFiles/Main.dir/src/Shader.cpp.o" \
"CMakeFiles/Main.dir/src/main.cpp.o" \
"CMakeFiles/Main.dir/external/glad/src/glad.c.o" \
"CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.o"

# External object files for target Main
Main_EXTERNAL_OBJECTS =

../bin/Main: CMakeFiles/Main.dir/src/Shader.cpp.o
../bin/Main: CMakeFiles/Main.dir/src/main.cpp.o
../bin/Main: CMakeFiles/Main.dir/external/glad/src/glad.c.o
../bin/Main: CMakeFiles/Main.dir/external/stb_image/src/stb_image.cpp.o
../bin/Main: CMakeFiles/Main.dir/build.make
../bin/Main: CMakeFiles/Main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cuifh/own/OO/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable ../bin/Main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Main.dir/build: ../bin/Main
.PHONY : CMakeFiles/Main.dir/build

CMakeFiles/Main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Main.dir/clean

CMakeFiles/Main.dir/depend:
	cd /home/cuifh/own/OO/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cuifh/own/OO /home/cuifh/own/OO /home/cuifh/own/OO/build /home/cuifh/own/OO/build /home/cuifh/own/OO/build/CMakeFiles/Main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Main.dir/depend
