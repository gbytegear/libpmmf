# CMAKE generated file: DO NOT EDIT!
# Generated by "MSYS Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = "/C/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "/C/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /C/Users/ByteGear/Source_Code/libpmmf/build

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /C/Users/ByteGear/Source_Code/libpmmf/build

# Include any dependencies generated for this target.
include CMakeFiles/dog.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/dog.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/dog.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dog.dir/flags.make

CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.obj: CMakeFiles/dog.dir/flags.make
CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.obj: CMakeFiles/dog.dir/includes_CXX.rsp
CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.obj: C:/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx
CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.obj: CMakeFiles/dog.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/C/Users/ByteGear/Source_Code/libpmmf/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.obj"
	/C/Qt/Tools/mingw1120_64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.obj -MF CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.obj.d -o CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.obj -c /C/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx

CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.i"
	/C/Qt/Tools/mingw1120_64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx > CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.i

CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.s"
	/C/Qt/Tools/mingw1120_64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx -o CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.s

# Object files for target dog
dog_OBJECTS = \
"CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.obj"

# External object files for target dog
dog_EXTERNAL_OBJECTS =

dog.exe: CMakeFiles/dog.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/cat.cxx.obj
dog.exe: CMakeFiles/dog.dir/build.make
dog.exe: libpmmf.a
dog.exe: CMakeFiles/dog.dir/linkLibs.rsp
dog.exe: CMakeFiles/dog.dir/objects1
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/C/Users/ByteGear/Source_Code/libpmmf/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable dog.exe"
	"/C/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/dog.dir/objects.a
	/C/Qt/Tools/mingw1120_64/bin/ar.exe qc CMakeFiles/dog.dir/objects.a @CMakeFiles/dog.dir/objects1
	/C/Qt/Tools/mingw1120_64/bin/c++.exe -fPIC -g -Wl,--whole-archive CMakeFiles/dog.dir/objects.a -Wl,--no-whole-archive -o dog.exe -Wl,--out-implib,libdog.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/dog.dir/linkLibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/dog.dir/build: dog.exe
.PHONY : CMakeFiles/dog.dir/build

CMakeFiles/dog.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dog.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dog.dir/clean

CMakeFiles/dog.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MSYS Makefiles" /C/Users/ByteGear/Source_Code/libpmmf/build /C/Users/ByteGear/Source_Code/libpmmf/build /C/Users/ByteGear/Source_Code/libpmmf/build /C/Users/ByteGear/Source_Code/libpmmf/build /C/Users/ByteGear/Source_Code/libpmmf/build/CMakeFiles/dog.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dog.dir/depend

