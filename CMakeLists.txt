cmake_minimum_required (VERSION 3.17)
project (wxsf)

find_program(WXCONFIG wx-config)
if(NOT WXCONFIG)
    message(FATAL_ERROR "wx-config not found!")
endif()

add_subdirectory(src)
add_subdirectory(samples)
