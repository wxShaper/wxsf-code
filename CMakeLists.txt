cmake_minimum_required (VERSION 3.17)

set(CMAKE_CXX_STANDARD 11)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project (wxsf VERSION 1.16.0 LANGUAGES CXX)

include(${CMAKE_ROOT}/Modules/FindwxWidgets.cmake)
include(${CMAKE_ROOT}/Modules/UsewxWidgets.cmake)

# We need the Find package for wxWidgets to work
find_package(wxWidgets REQUIRED core base xml)
include(${wxWidgets_USE_FILE})

add_subdirectory(src)
add_subdirectory(samples)
