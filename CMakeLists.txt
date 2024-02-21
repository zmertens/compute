cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
# ENABLE CMP0054: Only interpret if() arguments as variables or keywords when unquoted
cmake_policy(SET CMP0054 NEW)
# ENABLE CMP0074: USE *_ROOT PATH variables
cmake_policy(SET CMP0074 NEW)

project("compute" LANGUAGES C CXX)

# Require C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE_INIT "RelWithDebInfo")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(EXT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extlibs)

# find the required packages
find_package(OpenGL REQUIRED)

find_package(Threads)

set(GLM_DIR ${EXT_LIB_DIR}/glm-0.9.7)
message(STATUS "GLM included at ${GLM_DIR}")
add_definitions(-DGLM_FORCE_RADIANS)

set(GLFW_DIR ${EXT_LIB_DIR}/glfw-3.3-stable)
message(STATUS "GLFW3 included at ${GLFW_DIR}")
set(GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES ON CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_DIR})

set(GLAD_DIR ${EXT_LIB_DIR}/glad)
message(STATUS "glad included at ${GLAD_DIR}")
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)

set(STB_DIR ${EXT_LIB_DIR}/stb)
message(STATUS "STB included at ${STB_DIR}")

set(GLSL_RAYTRACE_SOURCES
	${EXT_LIB_DIR}/glad/src/glad.c
	GlRayTracer/GlfwHandler.cpp
	GlRayTracer/Shader.cpp
	GlRayTracer/Camera.cpp
	GlRayTracer/GlUtils.cpp
	GlRayTracer/Light.cpp
	GlRayTracer/Compute.cpp
	GlRayTracer/Material.cpp
	GlRayTracer/Player.cpp
	GlRayTracer/Transform.cpp
	GlRayTracer/Main.cpp
)

add_executable(${PROJECT_NAME} ${GLSL_RAYTRACE_SOURCES})

target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw)

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include ${GLM_DIR} ${GLAD_DIR}/include ${STB_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_14)

# Check if we need DEBUG compiler definition
string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_tolower)
if(cmake_build_type_tolower STREQUAL "debug" OR
    cmake_build_type_tolower STREQUAL "relwithdebinfo")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "DEBUG_COMPUTE")
    message(STATUS "Enabling DEBUG mode")
endif()

# copy resources / shader files
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
