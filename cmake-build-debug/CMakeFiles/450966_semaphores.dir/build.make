# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = "/Users/andrejpistek/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/191.7479.33/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/andrejpistek/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/191.7479.33/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/450966_semaphores.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/450966_semaphores.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/450966_semaphores.dir/flags.make

CMakeFiles/450966_semaphores.dir/main.c.o: CMakeFiles/450966_semaphores.dir/flags.make
CMakeFiles/450966_semaphores.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/450966_semaphores.dir/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/450966_semaphores.dir/main.c.o   -c "/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores/main.c"

CMakeFiles/450966_semaphores.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/450966_semaphores.dir/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores/main.c" > CMakeFiles/450966_semaphores.dir/main.c.i

CMakeFiles/450966_semaphores.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/450966_semaphores.dir/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores/main.c" -o CMakeFiles/450966_semaphores.dir/main.c.s

# Object files for target 450966_semaphores
450966_semaphores_OBJECTS = \
"CMakeFiles/450966_semaphores.dir/main.c.o"

# External object files for target 450966_semaphores
450966_semaphores_EXTERNAL_OBJECTS =

450966_semaphores: CMakeFiles/450966_semaphores.dir/main.c.o
450966_semaphores: CMakeFiles/450966_semaphores.dir/build.make
450966_semaphores: CMakeFiles/450966_semaphores.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable 450966_semaphores"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/450966_semaphores.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/450966_semaphores.dir/build: 450966_semaphores

.PHONY : CMakeFiles/450966_semaphores.dir/build

CMakeFiles/450966_semaphores.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/450966_semaphores.dir/cmake_clean.cmake
.PHONY : CMakeFiles/450966_semaphores.dir/clean

CMakeFiles/450966_semaphores.dir/depend:
	cd "/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores" "/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores" "/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores/cmake-build-debug" "/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores/cmake-build-debug" "/Users/andrejpistek/Documents/Dokumenty/Saxion (Applied Science)/LED (ETI 2)/Quarter 4/OS 2/Assignment2/450966_semaphores/cmake-build-debug/CMakeFiles/450966_semaphores.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/450966_semaphores.dir/depend

