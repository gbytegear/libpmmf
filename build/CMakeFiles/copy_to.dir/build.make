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
include CMakeFiles/copy_to.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/copy_to.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/copy_to.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/copy_to.dir/flags.make

CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.obj: CMakeFiles/copy_to.dir/flags.make
CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.obj: CMakeFiles/copy_to.dir/includes_CXX.rsp
CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.obj: C:/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx
CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.obj: CMakeFiles/copy_to.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/C/Users/ByteGear/Source_Code/libpmmf/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.obj"
	/C/Qt/Tools/mingw1120_64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.obj -MF CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.obj.d -o CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.obj -c /C/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx

CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.i"
	/C/Qt/Tools/mingw1120_64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx > CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.i

CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.s"
	/C/Qt/Tools/mingw1120_64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx -o CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.s

# Object files for target copy_to
copy_to_OBJECTS = \
"CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.obj"

# External object files for target copy_to
copy_to_EXTERNAL_OBJECTS =

copy_to.exe: CMakeFiles/copy_to.dir/C_/Users/ByteGear/Source_Code/libpmmf/test/copy_to.cxx.obj
copy_to.exe: CMakeFiles/copy_to.dir/build.make
copy_to.exe: libpmmf.a
copy_to.exe: CMakeFiles/copy_to.dir/linkLibs.rsp
copy_to.exe: CMakeFiles/copy_to.dir/objects1
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/C/Users/ByteGear/Source_Code/libpmmf/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable copy_to.exe"
	"/C/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/copy_to.dir/objects.a
	/C/Qt/Tools/mingw1120_64/bin/ar.exe qc CMakeFiles/copy_to.dir/objects.a @CMakeFiles/copy_to.dir/objects1
	/C/Qt/Tools/mingw1120_64/bin/c++.exe -fPIC -g -Wl,--whole-archive CMakeFiles/copy_to.dir/objects.a -Wl,--no-whole-archive -o copy_to.exe -Wl,--out-implib,libcopy_to.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/copy_to.dir/linkLibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/copy_to.dir/build: copy_to.exe
.PHONY : CMakeFiles/copy_to.dir/build

CMakeFiles/copy_to.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/copy_to.dir/cmake_clean.cmake
.PHONY : CMakeFiles/copy_to.dir/clean

CMakeFiles/copy_to.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MSYS Makefiles" /C/Users/ByteGear/Source_Code/libpmmf/build /C/Users/ByteGear/Source_Code/libpmmf/build /C/Users/ByteGear/Source_Code/libpmmf/build /C/Users/ByteGear/Source_Code/libpmmf/build /C/Users/ByteGear/Source_Code/libpmmf/build/CMakeFiles/copy_to.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/copy_to.dir/depend

