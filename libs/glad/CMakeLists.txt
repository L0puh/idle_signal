cmake_minimum_required(VERSION 3.15)

project(glad)

add_library(glad STATIC glad.c)

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
