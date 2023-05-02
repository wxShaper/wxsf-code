
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
# and for each of your dependent executable/library targets:
target_include_directories(wxSFDemo PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_directories(wxSFDemo PRIVATE ${PROJECT_SOURCE_DIR}/lib/)
target_link_libraries(wxSFDemo PRIVATE wxcode__wxsf-${PROJECT_VERSION})
