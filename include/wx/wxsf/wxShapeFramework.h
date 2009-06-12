/***************************************************************
 * Name:      wxShapeFramework.h
 * Purpose:   Main header file
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef _WXSF_H
#define _WXSF_H

// main library classes
#include "DiagramManager.h"
#include "ShapeCanvas.h"
#include "Thumbnail.h"

// shapes' declarations
#include "RectShape.h"
#include "RoundRectShape.h"
#include "FixedRectShape.h"
#include "EllipseShape.h"
#include "CircleShape.h"
#include "DiamondShape.h"
#include "TextShape.h"
#include "EditTextShape.h"
#include "BitmapShape.h"
#include "PolygonShape.h"
#include "ControlShape.h"
#include "GridShape.h"
#include "FlexGridShape.h"

// arrows' declarations
#include "OpenArrow.h"
#include "SolidArrow.h"
#include "DiamondArrow.h"
#include "CircleArrow.h"

// connection lines' declarations
#include "LineShape.h"
#include "CurveShape.h"
#include "OrthoShape.h"

// library events
#include "SFEvents.h"

// printing support
#include "Printout.h"

// common functions
#include "CommonFcn.h"

// serialize/deserialize functionality
#include "../wxxmlserializer/XmlSerializer.h"

#endif //_WXSF_H
