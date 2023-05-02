add_executable(wxSFSample4
        wxSFSample4App.cpp
        wxSFSample4App.h
        wxSFSample4Main.cpp
        wxSFSample4Main.h
        wx_pch.cpp
        wx_pch.h
        wxSF.rc
        res/wx.xpm
)
# and for each of your dependent executable/library targets:
target_include_directories(wxSFSample4 PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_directories(wxSFSample4 PRIVATE ${PROJECT_SOURCE_DIR}/lib/)
target_link_libraries(wxSFSample4 PRIVATE wxcode__wxsf-${PROJECT_VERSION})
