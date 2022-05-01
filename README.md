wxShapeFramework for wxWidgets
------------------------------
Author: Michal Bliznak, 2007
e-mail: michal.bliznak@gmail.com
www: http://sourceforge.net/projects/wxsf


What wxShapeFramework is?
-------------------------

wxShapeFramework (wxSF) is a software library/framework based on wxWidgets which allows
easy development of software applications manipulating with 
graphical objects (shapes) like various CASE tools, technological processes modeling
tools, etc.

The library consists of several classes encapsulating so called 'Shape canvas' (visual
GUI control used for management of included diagrams; it supports serialization/
deserialization to XML files, clipboard and drag&drop operations with diagram's
components, undo/redo operations, diagrams' export to BMP files, etc) and diagrams'
components as well (basic rectangular and eliptic shapes, line and curve shape,
polygonial shapes, static and in-place editable text, bitmap images, etc).

The shape framework (and shape canvas) allows to define relationship between various
shape types (for example which shape can be a child of another one, which shape types
can be connected together by which connection type, how do various connections look
like, etc) and provides ability to interactively design diagrams composited of those
shape objects.

More over, the library source code is based on wxWidgets GUI toolkit, so it is platform
independ (as far as the appropriate wxWidgets port is available for a required target
platform).


Distribution content:
---------------------

.\bin		- Redistributable binary files needed for running of client applications
		  using the wxSF's Dlls (contains basic wxWidgets library's dlls and
		  C runtime libraries for both MINGW and MS VC++ 2008).
.\demo		- Standalone executable of wxShapeFramework demo app.
.\doc		- Library documentation.
.\include	- wxSF header files.
.\lib		- Prebuild wxSF libraries suitable for both static and dynamic
		  linking and supported compilers (MinGW, MSVC8 and MSVC9) on MS Windows.

Binaries have been built with GCC 3.4.5, MSVC9 (MS Visual Studio 2008) and unicode release
 of wxWidgets 2.8.7.

