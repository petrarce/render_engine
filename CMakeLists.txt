# stop if cmake version below 3.5
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and supported languages
project(graphics CXX)

# detect operating system and host processor
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

# place binaries and libraries according to GNU standards
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

find_package(Qt5 COMPONENTS Core REQUIRED)

# project version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
if(CMAKE_BUILD_TYPE MATCHES RELEASE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif()

include_directories(AFTER .)
include_directories(AFTER molecular-programgenerator)
include_directories(AFTER molecular-util)

find_package(Threads)
find_package(OpenGL)
find_package(Eigen3)

add_subdirectory(src/core/)
add_subdirectory(src/DearImGui/)
add_subdirectory(src/apps/)
add_subdirectory(src/physics)
add_subdirectory(molecular-programgenerator)
add_subdirectory(molecular-util)
add_subdirectory(molecular-gfx)
add_subdirectory(molecular-meshfile)




