# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = "/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Server.dir/flags.make

CMakeFiles/Server.dir/src/Server/server.cpp.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/src/Server/server.cpp.o: ../src/Server/server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Server.dir/src/Server/server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Server.dir/src/Server/server.cpp.o -c "/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3/src/Server/server.cpp"

CMakeFiles/Server.dir/src/Server/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Server.dir/src/Server/server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3/src/Server/server.cpp" > CMakeFiles/Server.dir/src/Server/server.cpp.i

CMakeFiles/Server.dir/src/Server/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Server.dir/src/Server/server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3/src/Server/server.cpp" -o CMakeFiles/Server.dir/src/Server/server.cpp.s

# Object files for target Server
Server_OBJECTS = \
"CMakeFiles/Server.dir/src/Server/server.cpp.o"

# External object files for target Server
Server_EXTERNAL_OBJECTS =

Server: CMakeFiles/Server.dir/src/Server/server.cpp.o
Server: CMakeFiles/Server.dir/build.make
Server: CMakeFiles/Server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Server.dir/build: Server

.PHONY : CMakeFiles/Server.dir/build

CMakeFiles/Server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Server.dir/clean

CMakeFiles/Server.dir/depend:
	cd "/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3" "/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3" "/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3/cmake-build-debug" "/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3/cmake-build-debug" "/mnt/c/Users/Ben/OneDrive/10 - 2021 Fall/CMPUT 379/Assignment3/cmake-build-debug/CMakeFiles/Server.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Server.dir/depend

