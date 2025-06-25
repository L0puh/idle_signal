cmake_minimum_required(VERSION 3.31)
project(game_engine)

cmake_policy(SET CMP0072 NEW)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   add_compile_definitions(DEBUG_MODE)
endif()

#                       BUILD THIRD-PARTY LIBRARIES                       #

macro(find_or_build LIB_NAME FIND_PKG_NAME TARGET_NAME)
    find_package(${FIND_PKG_NAME} QUIET)
    if(${FIND_PKG_NAME}_FOUND)
        message(STATUS "Using system ${LIB_NAME}")
        set(${TARGET_NAME} ${FIND_PKG_NAME}::${FIND_PKG_NAME} PARENT_SCOPE)
    else()
        message(STATUS "Building bundled ${LIB_NAME}")
        add_subdirectory(libs/${LIB_NAME})
        set(${TARGET_NAME} ${LIB_NAME} PARENT_SCOPE)
    endif()
endmacro()


find_or_build(glfw glfw GLFW_LIB)
find_or_build(spdlog spdlog SPDLOG_LIB)
find_or_build(bullet3 Bullet Bullet_LIB) 

find_package(fmt QUIET)
if (fmt_FOUND)
   message(STATUS "Using system fmt")
   set(FMT_LIB fmt::fmt)
else()
    message(STATUS "Building bundled fmt")
    add_subdirectory(libs/fmt)
    set(FMT_LIB fmt)

endif()
find_package(Freetype QUIET)
if(Freetype_FOUND)
    message(STATUS "Using system FreeType")
    set(FREETYPE_LIBS ${FREETYPE_LIBRARIES})
else()
    message(STATUS "Building bundled FreeType")
    add_subdirectory(libs/freetype)
    set(FREETYPE_LIBS freetype)
    set(FREETYPE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype/include)
endif()

find_package(assimp QUIET)
if(assimp_FOUND)
    message(STATUS "Using system assimp")
    set(ASSIMP_LIB assimp)
else()
    message(STATUS "Building bundled assimp")
    add_subdirectory(libs/assimp)
    set(ASSIMP_LIB assimp)
endif()
find_package(OpenAL QUIET)
if(OpenAL_FOUND)
    message(STATUS "Using system openal")
    set(OPENAL_LIB OpenAL::OpenAL)
else()
    message(STATUS "Building bundled openal-soft")
    add_subdirectory(libs/openal-soft)
    set(OPENAL_LIB openal)
endif()

find_package(SndFile QUIET)
if (SndFile_FOUND)
    message(STATUS "Using system sndfile")
    set(SndFile_LIB SndFile::sndfile)
else()
    message(STATUS "Building bundled sndfile")
    add_subdirectory(libs/libsndfile)
    set(SndFile_LIB sndfile)
endif()


add_subdirectory(libs/glad)
add_subdirectory(libs/imgui)
set(GLAD_LIB glad)
set(IMGUI_LIB imgui)

set(GLM_INCLUDE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm)
set(KHR_INCLUDE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/libs/KHR)
set(STB_INCLUDE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb)
set(IMGUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)

#                               SOURCE CODE                               #


file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/core/main.cpp)

#                       LINK THIRD-PARTY LIBRARIES                        #

add_executable(app ${SOURCES})

target_link_libraries(app PRIVATE
    ${FMT_LIB}
    ${SPDLOG_LIB}
    ${IMGUI_LIB}
    ${ASSIMP_LIB}
    ${FREETYPE_LIBS}
    ${GLFW_LIB}
    ${OPENAL_LIB}
    BulletDynamics  
    BulletCollision
    LinearMath
    ${GLAD_LIB}
    ${SndFile_LIB}
)

target_include_directories(app PRIVATE include)
target_include_directories(app PRIVATE 
   ${GLM_INCLUDE_DIR}
   ${KHR_INCLUDE_DIR} 
   ${STB_INCLUDE_DIR} 
   ${BULLET_INCLUDE_DIRS}
   ${FREETYPE_INCLUDE_DIRS} 
   ${IMGUI_INCLUDE_DIR} 
   ${IMGUI_INCLUDE_DIR}
   ${SNDFILE_INCLUDE_DIRS}

)
