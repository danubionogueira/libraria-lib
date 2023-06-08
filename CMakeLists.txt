cmake_minimum_required(VERSION 3.18.4)

set(LIB_TYPE SHARED)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("Debug mode")
	set(LIB_TYPE STATIC)
endif ()

project(libraria-lib VERSION 0.0.0.1)

#if (${CMAKE_SYSTEM_NAME} EQUAL "Linux")
	add_definitions(-DIBPP_LINUX)
#elseif (${CMAKE_SYSTEM_NAME} EQUAL "Windows")
	#add_definitions(-DIBPP_WINDOWS)
#elseif (${CMAKE_SYSTEM_NAME} EQUAL "Darwin")
	#add_definitions(-DIBPP_DARWIN)
#endif ()

add_library(ibpp ibpp/all_in_one.cpp)

add_library(libraria ${LIB_TYPE} src/Book.cpp src/Utils.cpp)
add_executable(libraria-test src/Test.cpp src/Main.cpp)
target_link_libraries(libraria-test libraria ibpp fbclient)
