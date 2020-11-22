cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)

project ("GLSL_RAYTRACER")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIRS}")
# find_package(SDL 2.0 REQUIRED)
# message(STATUS "Found SDL 2 in ${SFML_INCLUDE_DIR}")

set(GLSL_RAYTRACER_INCLUDE ${GLSL_RAYTRACER_SOURCE_DIR})
list(APPEND GLSL_RAYTRACER_INCLUDE ${GLM_INCLUDE_DIR})
add_definitions(-DGLM_FORCE_RADIANS)

# since the 'GLSL_RAYTRACER_LIBS' variable hasn't been set yet
# - go ahead and set it
set(GLSL_RAYTRACER_LIBS )
# list(APPEND GLSL_RAYTRACER_INCLUDE ${SDL_INCLUDE_DIR})
list(APPEND GLSL_RAYTRACER_LIBS SDL2)

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
	GlRayTracer/SdlManager.cpp
	GlRayTracer/Shader.cpp
	GlRayTracer/Transform.cpp
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/GlRayTracer/extlibs)
add_executable( ${PROJECT_NAME} ${GLSL_RAYTRACE_SOURCES} )
# target_include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/GlRayTracer/extlibs)
target_link_libraries( ${PROJECT_NAME} ${SDL_LIBRARIES}  ${GLSL_RAYTRACER_LIBS})

# copy resources / shader files
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
