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

# and for each of your dependent executable/library targets:
target_include_directories(wxSFSample1 PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(wxSFSample1 PRIVATE wxcode__wxsf-${PROJECT_VERSION})
