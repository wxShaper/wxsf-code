set(wxconf_cmd "wx-config")
set(wxconf_cxxflags "--cxxflags")
execute_process(COMMAND ${wxconf_cmd} ${wxconf_cxxflags}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESULT_VARIABLE wxconf_result
    OUTPUT_VARIABLE wxconf_flagset)

string(STRIP ${wxconf_flagset} wxconf_flagset)
string(REPLACE " " ";" inc_list ${wxconf_flagset})

foreach(X ${inc_list})
    string(REGEX REPLACE "^-I" "" inc_dir ${X})
    if(IS_DIRECTORY ${inc_dir})
        include_directories(${inc_dir})
    else()
        add_compile_options(${X})
    endif()
endforeach()

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib/)
link_libraries(wxcode__wxsf-3.1.0)

add_executable(wxSFSample1
        wxSFSample1App.cpp
        wxSFSample1App.h
        wxSFSample1Main.cpp
        wxSFSample1Main.h
        wx_pch.cpp
        wx_pch.h
        wxSF.rc
        res/wx.xpm
)
