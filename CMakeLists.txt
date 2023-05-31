cmake_minimum_required (VERSION 3.17)

set(CMAKE_CXX_STANDARD 11)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -pedantic)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options(/W4)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" ON)

project (wxsf VERSION 1.16.0 LANGUAGES CXX)

include(${CMAKE_ROOT}/Modules/FindwxWidgets.cmake)
include(${CMAKE_ROOT}/Modules/UsewxWidgets.cmake)

# We need the Find package for wxWidgets to work
find_package(wxWidgets REQUIRED core base xml)
include(${wxWidgets_USE_FILE})

add_subdirectory(src)
add_subdirectory(samples)
