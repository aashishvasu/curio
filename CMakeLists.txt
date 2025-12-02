cmake_minimum_required(VERSION 4.0)

project(Curio VERSION 0.1
		DESCRIPTION "A small game (engine?)"
		LANGUAGES CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	# Ensure -std=c++xx instead of -std=g++xx
	set(CMAKE_CXX_EXTENSIONS OFF)

	# Support folders in IDEs
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# Dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# External deps
include(cmake/dependencies.cmake)

# Add library
if(NOT TARGET curiolib)
	add_subdirectory(curiolib)
endif()

# Add executable
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_subdirectory(sandbox)
endif()
