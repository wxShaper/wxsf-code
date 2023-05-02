set(SRCS 
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

add_library(wxcode__wxsf-${PROJECT_VERSION} SHARED ${SRCS})

target_include_directories(wxcode__wxsf-${PROJECT_VERSION} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(wxcode__wxsf-${PROJECT_VERSION} ${wxWidgets_LIBRARIES})
