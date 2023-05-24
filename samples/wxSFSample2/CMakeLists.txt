add_executable(wxSFSample2
        SampleCanvas.cpp
        SampleCanvas.h
        SampleShape.cpp
        SampleShape.h
        wxSFSample2App.cpp
        wxSFSample2App.h
        wxSFSample2Main.cpp
        wxSFSample2Main.h
        wx_pch.cpp
        wx_pch.h
        wxSF.rc
        res/wx.xpm
)

# and for each of your dependent executable/library targets:
target_include_directories(wxSFSample2 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(wxSFSample2 PRIVATE wxcode__wxsf-${PROJECT_VERSION})
