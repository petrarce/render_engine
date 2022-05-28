# stop if cmake version below 3.5
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and supported languages
project(graphics CXX C)

# detect operating system and host processor
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

# place binaries and libraries according to GNU standards
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_ASSETS_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_ASSETSDIR})


# project version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/" CACHE PATH "Path to cmake modules")

include(default.cmake)
include(cmake/modules/HelperMacros.cmake)

include_directories(AFTER .)
include_directories(AFTER molecular-programgenerator)
include_directories(AFTER molecular-util)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Threads)
find_package(OpenGL)
find_package(Eigen3)
find_package(Boost 1.63 COMPONENTS unit_test_framework REQUIRED)

enable_testing()

add_subdirectory(3rdparty/)
add_subdirectory(molecular-util)
add_subdirectory(molecular-programgenerator)
add_subdirectory(molecular-meshfile)
add_subdirectory(molecular-gfx)
add_subdirectory(src)
add_subdirectory(apps/LearnOpenGL/)
add_subdirectory(apps/demos)
