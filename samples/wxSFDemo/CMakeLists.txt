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

add_executable(wxSFDemo
        Art.h
        FrameCanvas.cpp
        FrameCanvas.h
        GUI.cpp
        GUI.fbp
        GUI.h
        Ids.h
        MainApp.cpp
        MainApp.h
        MainFrame.cpp
        MainFrame.h
        Images/motyl.bmp
        Images/motyl2.bmp
        wx_pch.cpp
        wx_pch.h
        wxSF.rc
        res/AlignBottom.xpm
        res/AlignCenter.xpm
        res/AlignLeft.xpm
        res/AlignMiddle.xpm
        res/AlignRight.xpm
        res/AlignTop.xpm
        res/Bitmap.xpm
        res/Circle.xpm
        res/Curve.xpm
        res/Diamond.xpm
        res/EditText.xpm
        res/Ellipse.xpm
        res/FixedRect.xpm
        res/FlexGrid.xpm
        res/GC.xpm
        res/Grid.xpm
        res/Line.xpm
        res/NoSource.xpm
        res/OrthoLine.xpm
        res/Rect.xpm
        res/RoundOrthoLine.xpm
        res/RoundRect.xpm
        res/Shadow.xpm
        res/StandAloneLine.xpm
        res/Text.xpm
        res/Tool.xpm
        res/wx.xpm
)
