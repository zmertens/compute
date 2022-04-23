cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
# ENABLE CMP0054: Only interpret if() arguments as variables or keywords when unquoted
cmake_policy(SET CMP0054 NEW)
# ENABLE CMP0074: USE *_ROOT PATH variables
cmake_policy(SET CMP0074 NEW)

project("compute" LANGUAGES C CXX)

# Require C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_BUILD_TYPE_INIT "RelWithDebInfo")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/GlRayTracer/extlibs)

# find the required packages
find_package(OpenGL REQUIRED)

find_package(Threads)

find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIRS}")
add_definitions(-DGLM_FORCE_RADIANS)

set(GLFW_DIR ${LIB_DIR}/glfw-3.3-stable)
set(GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES ON CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_DIR})

set(GLSL_RAYTRACER_LIBS "")

if(WIN32)

endif(WIN32)
if(UNIX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
	# set the native CMake packages
	find_package(OpenGL REQUIRED)
	list(APPEND GLSL_RAYTRACER_LIBS ${OPENGL_gl_LIBRARY})

	find_package(Threads REQUIRED)
	list(APPEND GLSL_RAYTRACER_LIBS ${CMAKE_THREAD_LIBS_INIT})

	find_package(X11 REQUIRED)
	list(APPEND GLSL_RAYTRACER_LIBS ${X11_LIBRARIES})

	find_library(RT_LIB rt)
	list(APPEND GLSL_RAYTRACER_LIBS ${RT_LIB})
endif(UNIX)
if(APPLE)
	list(APPEND GLSL_RAYTRACER_LIBS ${APPLE_LIBS})
endif(APPLE)

set(GLSL_RAYTRACE_SOURCES
	GlRayTracer/extlibs/gl_core_4_5.c
	GlRayTracer/Camera.cpp
	GlRayTracer/GlUtils.cpp
	GlRayTracer/Light.cpp
	GlRayTracer/Main.cpp
	GlRayTracer/Compute.cpp
	GlRayTracer/Material.cpp
	GlRayTracer/Player.cpp
	GlRayTracer/GlfwHandler.cpp
	GlRayTracer/Shader.cpp
	GlRayTracer/Transform.cpp
)

add_executable(${PROJECT_NAME} ${GLSL_RAYTRACE_SOURCES})

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS} OpenGL::GL glfw ${GLFW_LIBRARIES} ${GLSL_RAYTRACER_LIBS})

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include ${GLM_INCLUDE_DIR}})
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_14)

# Check if we need DEBUG compiler definition
string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_tolower)
if(cmake_build_type_tolower STREQUAL "debug" OR
    cmake_build_type_tolower STREQUAL "relwithdebinfo")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_COMPUTE)
    message("Enabling DEBUG mode")
endif()

# copy resources / shader files
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
