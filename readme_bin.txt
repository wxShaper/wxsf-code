wxShapeFramework for wxWidgets
------------------------------
Author: Michal Bliznak, 2007
e-mail: michal.bliznak@tiscali.cz
www: http://sourceforge.net/projects/wxsf


What wxShapeFramework is?
-------------------------

wxShapeFramework (wxSF) is a software library/framework based on wxWidgets which
allows easy development of software applications which GUI manipulates with 
graphical objects (shapes) like various CASE tools, technological processes modeling
tools, etc.

The library consists of several classes encapsulating so called Shape canvas (visual
GUI control used for management of included diagrams which supports serialization/
deserialization to XML files, clipboard and drag&drop operations with diagram's
components, undo/redo operations, diagrams' export to BMP files, etc) and diagrams'
components as well (basic rectangular and eliptic shapes, line and curve shape,
polygonial shapes, static and in-place editable text, bitmap images, etc).

The shape framework (and shape canvas) allows to define relationship between various
shape types (for example which shape can be a child of another one, which shape types
can be connected together by which connection type, how do various connections look
like, etc) and provides ability to interactively design diagrams composited of those
shape objects.

More over, the library source code is based on wxWidgets GUI toolkit, so it is platform independ (as far as the appropriate wxWidgets port is available for a required target platform).


Distribution content:
---------------------

.\Release	- standalone executable of wxShapeFramework demo app
.\ReleaseDLL	- DLL-based executable and relevant DLLs of wxShapeFramework demo
		application.

All binaries have been built with GCC 3.4.5 and unicode release of wxWidgets 2.8.3.p1
(both static and dynamic link).

Note:
-----

This package do not allow building of user applications based on wxSF. To do this you
must download source files package that includes both library source code files and
the demo application source files as well.

