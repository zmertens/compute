# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.22

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\zach\Desktop\zmertens-compute

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\zach\Desktop\zmertens-compute\build

# Include any dependencies generated for this target.
include CMakeFiles/GLSL_RAYTRACER.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/GLSL_RAYTRACER.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GLSL_RAYTRACER.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GLSL_RAYTRACER.dir/flags.make

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.obj: CMakeFiles/GLSL_RAYTRACER.dir/flags.make
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.obj: CMakeFiles/GLSL_RAYTRACER.dir/includes_C.rsp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.obj: ../GlRayTracer/extlibs/gl_core_4_5.c
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.obj: CMakeFiles/GLSL_RAYTRACER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.obj"
	C:\Strawberry\c\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.obj -MF CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\extlibs\gl_core_4_5.c.obj.d -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\extlibs\gl_core_4_5.c.obj -c C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\extlibs\gl_core_4_5.c

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.i"
	C:\Strawberry\c\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\extlibs\gl_core_4_5.c > CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\extlibs\gl_core_4_5.c.i

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.s"
	C:\Strawberry\c\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\extlibs\gl_core_4_5.c -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\extlibs\gl_core_4_5.c.s

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/flags.make
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/includes_CXX.rsp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.obj: ../GlRayTracer/Camera.cpp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.obj"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.obj -MF CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Camera.cpp.obj.d -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Camera.cpp.obj -c C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Camera.cpp

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.i"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Camera.cpp > CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Camera.cpp.i

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.s"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Camera.cpp -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Camera.cpp.s

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/flags.make
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/includes_CXX.rsp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.obj: ../GlRayTracer/GlUtils.cpp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.obj"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.obj -MF CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\GlUtils.cpp.obj.d -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\GlUtils.cpp.obj -c C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\GlUtils.cpp

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.i"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\GlUtils.cpp > CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\GlUtils.cpp.i

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.s"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\GlUtils.cpp -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\GlUtils.cpp.s

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/flags.make
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/includes_CXX.rsp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.obj: ../GlRayTracer/Light.cpp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.obj"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.obj -MF CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Light.cpp.obj.d -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Light.cpp.obj -c C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Light.cpp

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.i"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Light.cpp > CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Light.cpp.i

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.s"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Light.cpp -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Light.cpp.s

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/flags.make
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/includes_CXX.rsp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.obj: ../GlRayTracer/Main.cpp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.obj"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.obj -MF CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Main.cpp.obj.d -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Main.cpp.obj -c C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Main.cpp

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.i"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Main.cpp > CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Main.cpp.i

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.s"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Main.cpp -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Main.cpp.s

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/flags.make
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/includes_CXX.rsp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.obj: ../GlRayTracer/Material.cpp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.obj"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.obj -MF CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Material.cpp.obj.d -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Material.cpp.obj -c C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Material.cpp

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.i"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Material.cpp > CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Material.cpp.i

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.s"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Material.cpp -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Material.cpp.s

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/flags.make
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/includes_CXX.rsp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.obj: ../GlRayTracer/Player.cpp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.obj"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.obj -MF CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Player.cpp.obj.d -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Player.cpp.obj -c C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Player.cpp

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.i"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Player.cpp > CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Player.cpp.i

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.s"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Player.cpp -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Player.cpp.s

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/flags.make
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/includes_CXX.rsp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.obj: ../GlRayTracer/GlfwHandler.cpp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.obj"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.obj -MF CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\GlfwHandler.cpp.obj.d -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\GlfwHandler.cpp.obj -c C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\GlfwHandler.cpp

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.i"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\GlfwHandler.cpp > CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\GlfwHandler.cpp.i

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.s"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\GlfwHandler.cpp -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\GlfwHandler.cpp.s

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/flags.make
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/includes_CXX.rsp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.obj: ../GlRayTracer/Shader.cpp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.obj"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.obj -MF CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Shader.cpp.obj.d -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Shader.cpp.obj -c C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Shader.cpp

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.i"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Shader.cpp > CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Shader.cpp.i

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.s"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Shader.cpp -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Shader.cpp.s

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/flags.make
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/includes_CXX.rsp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.obj: ../GlRayTracer/Transform.cpp
CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.obj: CMakeFiles/GLSL_RAYTRACER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.obj"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.obj -MF CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Transform.cpp.obj.d -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Transform.cpp.obj -c C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Transform.cpp

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.i"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Transform.cpp > CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Transform.cpp.i

CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.s"
	C:\Strawberry\c\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\zach\Desktop\zmertens-compute\GlRayTracer\Transform.cpp -o CMakeFiles\GLSL_RAYTRACER.dir\GlRayTracer\Transform.cpp.s

# Object files for target GLSL_RAYTRACER
GLSL_RAYTRACER_OBJECTS = \
"CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.obj" \
"CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.obj" \
"CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.obj" \
"CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.obj" \
"CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.obj" \
"CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.obj" \
"CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.obj" \
"CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.obj" \
"CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.obj" \
"CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.obj"

# External object files for target GLSL_RAYTRACER
GLSL_RAYTRACER_EXTERNAL_OBJECTS =

GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/extlibs/gl_core_4_5.c.obj
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Camera.cpp.obj
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlUtils.cpp.obj
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Light.cpp.obj
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Main.cpp.obj
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Material.cpp.obj
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Player.cpp.obj
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/GlfwHandler.cpp.obj
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Shader.cpp.obj
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/GlRayTracer/Transform.cpp.obj
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/build.make
GLSL_RAYTRACER.exe: GlRayTracer/extlibs/glfw-3.3-stable/src/libglfw3dll.a
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/linklibs.rsp
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/objects1.rsp
GLSL_RAYTRACER.exe: CMakeFiles/GLSL_RAYTRACER.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable GLSL_RAYTRACER.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\GLSL_RAYTRACER.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GLSL_RAYTRACER.dir/build: GLSL_RAYTRACER.exe
.PHONY : CMakeFiles/GLSL_RAYTRACER.dir/build

CMakeFiles/GLSL_RAYTRACER.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\GLSL_RAYTRACER.dir\cmake_clean.cmake
.PHONY : CMakeFiles/GLSL_RAYTRACER.dir/clean

CMakeFiles/GLSL_RAYTRACER.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\zach\Desktop\zmertens-compute C:\Users\zach\Desktop\zmertens-compute C:\Users\zach\Desktop\zmertens-compute\build C:\Users\zach\Desktop\zmertens-compute\build C:\Users\zach\Desktop\zmertens-compute\build\CMakeFiles\GLSL_RAYTRACER.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/GLSL_RAYTRACER.dir/depend

