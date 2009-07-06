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

Requirements:
-------------

- wxSF uses wxWidgets (www.wxwidgets.org) and was tested with version 2.8.x on MS Windows
  and Linux (Ubuntu 7.04) target platforms.
- Code::Blocks (www.codeblocks.org) IDE with MinGW C++ compiler or MS Visual Studio 2005/2008
  Express are recommended for opening included workspace/project files but the library can be
  built from the command line as well.


Distribution content:
---------------------

.\build		- Premake build system for creation of various project/make files
.\doc		- wxSF doxygen documentation file (in CHM and HTML format).
.\include	- Library headers files (needed for both hosting applications and
		library itself)
.\samples	- Sample and demo projects with commented source code.
.\src		- Source code ad project files of the wxSF library
changes.txt	- Information about library version and release changes.

.\lib		- Lib files (will be created after library/demo app build)



Build instructions:
===================
The library and included sample projects can be build with two different build systems included in
a release package. Both of them are located in the ./build subdirectory and you can freely choose which one to use.

Premake build system:
---------------------
Premake build scripts and executables are suitable for creation of various IDE project files and 
GNU makefile as well. You can simply create Code::Blocks and MS VS 2005/2008 project files by invoking of
the included batch scripts.


Installation - win32:

When building on win32, go to the ./build subdirectory and simply type:

> create_build_files.bat

There are few standard build configurations defined in the batch file and you can freely modify it or you can
inkove the premake system directly as follows:

Examples of usage of the premake scripts:

For Code::Blocks Workspace/Project files producing shared library with UNICODE support, using wxWidgets DLLs type:
> premake\premake-win32.exe --target cb-gcc --unicode --with-wx-shared --shared

For MSVS 2005/2008 Workspace/Project files producing staticaly built library with UNICODE support and staticaly linked runtime
and no built-in wchar type:
> premake\premake-win32.exe --target vs2005 --unicode --static-runtime --no-builtin-wchar

For GNU makefile producing static library (static link of both wxWidgets libs and runtime) type:
> premake\premake-win32.exe --target gnu --unicode --static-runtime


Installation - Unix:

When building on Unix, go to the ./build subdirectory and simply type:

sh ./create_build_files.sh

or you can invoke the premake system as described in the Win32 installation section above.

Examples of usage of the premake scripts on Unix:

For Code::Blocks Workspace/Project files producing shared library with UNICODE support, using non-debug wxWidgets DLLs type: 
premake/premake-linux --target cb-gcc --shared --with-wx-shared --unicode --disable-wx-debug

For Code::Blocks Workspace/Project files producing staticaly built library with debugging and UNICODE support and staticaly linked runtime
premake/premake-linux --target cb-gcc --unicode

For GNU makefile producing static library (static link of wxWidgets (with debugging support) libs and runtime) type:
premake/premake-linux --target gnu --unicode


Note that all needed executables and scripts are included in the release package so you can use the premake build system immediately.


Bakefile build system:
----------------------

Installation - win32:

When building on win32, you can use the makefiles generated by the bakefile build system located in the ./build
folder using the WX_DIR, WX_DEBUG, WX_UNICODE, WX_SHARED and WX_MONOLITHIC options to match your wxWidgets build or you
can re-create these makefiles by invoking

> bakefile_gen

command from within the ./build directory (in this case the bakefile build system must be installed on your system).

Examples of usage of the prebuild makefiles:
> nmake -f makefile.vc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1 WX_MONOLITHIC=0/1
> make -f makefile.bcc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1 WX_MONOLITHIC=0/1
> mingw32-make -f makefile.gcc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1 WX_MONOLITHIC=0/1

Note that all supplied command line parameters are optional. If some of them are missing then the default values stored
in relevant makefile are used instead.

Installation - unix:

When building on a Linux/GNU-based system, go to the component installation folder and type:

./configure
make
make install

to build against the default build of wxWidgets (the one listed as 'default' by wx-config --list).
Type "./configure --help" for more info.

You can also clean all built/installed files by typing:

make clean
make uninstall

If you want to re-create the ./configure and Makefile scripts then you must have bakefile and autoconf
system installed on your system. If all the prerequisities are present, go to the ./build subdirectory and type:

bakefile_gen
./acregen.sh



