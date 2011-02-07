#ifndef IDS_H_INCLUDED
#define IDS_H_INCLUDED

enum APPIDS
{
    // menu IDs
    //---------------------------------------------------------------//
    IDM_SAVEASBITMAP = wxID_HIGHEST + 1,

    // tool IDs
    //---------------------------------------------------------------//
    IDT_FIRST_TOOLMARKER,

    IDT_GRID,
    IDT_SHADOW,
	IDT_GC,
    IDT_TOOL,
    IDT_RECTSHP,
    IDT_SQUARESHP,
    IDT_RNDRECTSHP,
    IDT_ELLIPSESHP,
    IDT_CIRCLESHP,
    IDT_DIAMONDSHP,
    IDT_TEXTSHP,
    IDT_EDITTEXTSHP,
    IDT_BITMAPSHP,
    IDT_GRIDSHP,
    IDT_FLEXGRIDSHP,
    IDT_LINESHP,
    IDT_STANDALONELINESHP,
    IDT_CURVESHP,
	IDT_ORTHOSHP,
	IDT_RNDORTHOSHP,
    IDT_ALIGN_LEFT,
    IDT_ALIGN_RIGHT,
    IDT_ALIGN_TOP,
    IDT_ALIGN_BOTTOM,
    IDT_ALIGN_MIDDLE,
    IDT_ALIGN_CENTER,

    IDT_LAST_TOOLMARKER,
	
	IDM_AUTOLAYOUT_FIRST,
	IDM_AUTOLAYOUT_LAST = IDM_AUTOLAYOUT_FIRST + 100,

    // other constrols
    //---------------------------------------------------------------//
    IDT_COLORPICKER
};

#endif // IDS_H_INCLUDED
