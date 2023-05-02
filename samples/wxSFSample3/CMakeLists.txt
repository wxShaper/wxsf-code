add_executable(wxSFSample3
        StarShape.cpp
        StarShape.h
        wxSFSample3App.cpp
        wxSFSample3App.h
        wxSFSample3Main.cpp
        wxSFSample3Main.h
        wx_pch.cpp
        wx_pch.h
        wxSF.rc
        res/wx.xpm
)

# and for each of your dependent executable/library targets:
target_include_directories(wxSFSample3 PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_directories(wxSFSample3 PRIVATE ${PROJECT_SOURCE_DIR}/lib/)
target_link_libraries(wxSFSample3 PRIVATE wxcode__wxsf-${PROJECT_VERSION})
