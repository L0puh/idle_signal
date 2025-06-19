cmake_minimum_required(VERSION 3.31)
project(template)

cmake_policy(SET CMP0072 NEW)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   add_compile_definitions(DEBUG_MODE)
endif()

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenAL REQUIRED)
find_package(Bullet REQUIRED)

include_directories(libs)
include_directories(include)
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${BULLET_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)
file(GLOB_RECURSE LIBS    ${CMAKE_CURRENT_SOURCE_DIR}/libs/*.c ${CMAKE_CURRENT_SOURCE_DIR}/libs/*.cpp)

add_executable(app ${LIBS} ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/core/main.cpp)
target_link_libraries(app glfw OpenGL::GL assimp ${FREETYPE_LIBRARIES}
   LinearMath openal sndfile BulletCollision BulletDynamics)
