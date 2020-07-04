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

set(wxconf_cmd "wx-config")
set(wxconf_libs "--libs")
execute_process(COMMAND ${wxconf_cmd} ${wxconf_libs}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESULT_VARIABLE wxconf_result
    OUTPUT_VARIABLE wxconf_libset)

string(STRIP ${wxconf_libset} wxconf_libset)
link_libraries(${wxconf_libset})

add_library(wxcode__wxsf-3.1.0 SHARED
        ArrowBase.cpp
        AutoLayout.cpp
        BitmapShape.cpp
        BitmapShapeXml.cpp
        CMakeLists.txt
        CanvasHistory.cpp
        CanvasState.cpp
        CircleArrow.cpp
        CircleShape.cpp
        CommonFcn.cpp
        ControlShape.cpp
        CurveShape.cpp
        DiagramManager.cpp
        DiamondArrow.cpp
        DiamondShape.cpp
        EditTextShape.cpp
        EllipseShape.cpp
        FixedRectShape.cpp
        FlexGridShape.cpp
        GUI.fbp
        GridShape.cpp
        LineShape.cpp
        MultiSelRect.cpp
        OpenArrow.cpp
        OrthoShape.cpp
        PolygonShape.cpp
        PolygonShapeXml.cpp
        Printout.cpp
        RectShape.cpp
        RoundOrthoShape.cpp
        RoundRectShape.cpp
        SFEvents.cpp
        ScaledDC.cpp
        ShapeBase.cpp
        ShapeBaseXml.cpp
        ShapeCanvas.cpp
        ShapeDataObject.cpp
        ShapeDockpoint.cpp
        ShapeHandle.cpp
        SolidArrow.cpp
        TextShape.cpp
        TextShapeXml.cpp
        Thumbnail.cpp
        premake.lua
        wxSF.rc
        wx_pch.cpp
        wx_pch.h
        res/NoSource.xpm
        wxxmlserializer/PropertyIO.cpp
        wxxmlserializer/XmlSerializer.cpp
)
