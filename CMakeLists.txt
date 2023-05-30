cmake_minimum_required(VERSION 3.18.4)

set(LIB_TYPE SHARED)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("Debug mode")
	set(LIB_TYPE STATIC)
endif ()

project(libraria-lib VERSION 0.0.0.1)

add_library(libraria ${LIB_TYPE} src/Book.cpp)
add_executable(libraria-test src/Test.cpp src/Main.cpp)
target_link_libraries(libraria-test libraria)
