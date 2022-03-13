cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)

project ("GLSL_RAYTRACER")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/GlRayTracer/extlibs)

# find the required packages
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
	find_package( OpenGL REQUIRED )
	list( APPEND GLSL_RAYTRACER_LIBS ${OPENGL_gl_LIBRARY} )
endif(WIN32)
if(UNIX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
	# set the native CMake packages
	find_package( OpenGL REQUIRED )
	list( APPEND GLSL_RAYTRACER_LIBS ${OPENGL_gl_LIBRARY} )

	find_package( Threads REQUIRED )
	list( APPEND GLSL_RAYTRACER_LIBS ${CMAKE_THREAD_LIBS_INIT} )

	find_package( X11 REQUIRED )
	list( APPEND GLSL_RAYTRACER_LIBS ${X11_LIBRARIES})

	find_library( RT_LIB rt )
	list( APPEND GLSL_RAYTRACER_LIBS ${RT_LIB} )
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
	GlRayTracer/Material.cpp
	GlRayTracer/Player.cpp
	GlRayTracer/GlfwHandler.cpp
	GlRayTracer/Shader.cpp
	GlRayTracer/Transform.cpp
)

add_executable( ${PROJECT_NAME} ${GLSL_RAYTRACE_SOURCES} )

target_link_libraries( ${PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${GLSL_RAYTRACER_LIBS})

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include ${GLM_INCLUDE_DIR}})
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)

# copy resources / shader files
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
