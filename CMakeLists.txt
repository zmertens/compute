cmake_minimum_required(VERSION 3.4..4.15)

project("compute" LANGUAGES C CXX)

# Require C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(COMPUTE_APP_NAME compute)

set(EXTLIBS_DIR ${CMAKE_SOURCE_DIR}/extlibs)

# find the required packages
find_package(OpenGL REQUIRED)

set(STB_IMAGE_DIR ${EXTLIBS_DIR}/stb)
message(STATUS "stb included at ${STB_IMAGE_DIR}")

set(GLM_DIR ${EXTLIBS_DIR}/glm-0.9.7)
message(STATUS "GLM included at ${GLM_DIR}")

set(GLAD_DIR ${EXTLIBS_DIR}/glad)
message(STATUS "glad included at ${GLAD_DIR}")
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)

find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
    message(STATUS "Getting SDL via FetchContent")
    set(SDL_SHARED TRUE CACHE BOOL "Build a SDL shared library (if available)")
    set(SDL_STATIC FALSE CACHE BOOL "Build a SDL static library (if available)")
    include(FetchContent)
    FetchContent_Declare(SDL
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-3.2.10)
    set_property(DIRECTORY "${CMAKE_BINARY_DIR}/_deps/sdl-src" PROPERTY EXCLUDE_FROM_ALL TRUE)
    FetchContent_MakeAvailable(SDL)
endif ()

# Copy shared/dlls on Windows only
if (WIN32)
    add_custom_command(
            TARGET ${COMPUTE_APP_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:${COMPUTE_APP_NAME}>
            VERBATIM
    )
endif ()

set(GL_RAYTRACER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/GLRaytracer)

set(GL_RAYTRACER_SOURCE_FILES
    ${GL_RAYTRACER_DIR}/Camera.cpp
    ${GL_RAYTRACER_DIR}/Compute.cpp
    ${GL_RAYTRACER_DIR}/GLUtils.cpp
    ${GL_RAYTRACER_DIR}/Light.cpp
    ${GL_RAYTRACER_DIR}/Main.cpp
    ${GL_RAYTRACER_DIR}/Material.cpp
    ${GL_RAYTRACER_DIR}/Player.cpp
    ${GL_RAYTRACER_DIR}/SDLHelper.cpp
    ${GL_RAYTRACER_DIR}/Shader.cpp
    ${GL_RAYTRACER_DIR}/Transform.cpp
)

add_executable(${COMPUTE_APP_NAME} ${GL_RAYTRACER_SOURCE_FILES})

set(DEBUG_COMPUTE "DEBUG_COMPUTE")
target_compile_definitions(${COMPUTE_APP_NAME} PRIVATE "$<$<OR:$<STREQUAL:$<CONFIG>,Debug>,$<STREQUAL:$<CONFIG>,RelWithDebInfo>>:${DEBUG_COMPUTE}>")
target_compile_definitions(${COMPUTE_APP_NAME} PRIVATE GLM_FORCE_RADIANS)

target_compile_features(${COMPUTE_APP_NAME} PRIVATE cxx_std_20)

target_link_libraries(${COMPUTE_APP_NAME} OpenGL::GL SDL3::SDL3 glad)

target_include_directories(${COMPUTE_APP_NAME} PRIVATE ${GLM_DIR} ${GLAD_DIR}/include ${STB_IMAGE_DIR} ${GL_RAYTRACER_DIR})

# copy resources / shader files
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
