wxShapeFramework for wxWidgets
------------------------------
Author: Michal Bliznak, 2007
e-mail: michal.bliznak@tiscali.cz
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
- Code::Blocks (www.codeblocks.org) IDE with MinGW C++ compiler or MS Visual Studio 2005/
  Visual C++ 2005 Express are recommended for opening included workspace/project files.


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
-------------------
For building of the library files and the sample and demo projects you must 
create a build/project files first. This task can be done via included premake build
system located in the ./build directory. Simple run one of the appropriate shell scripts 
(create_build_files.bat for MS Windows platform, create_build_files.sh for Linux and
create_build_files_mac.sh for OS X platform). These scripts will create project/make files
suitable for choosen build targets and IDEs. Currently the Code::Blocks and MS Visual
Studio 2005/Visual C++ 2005 Express project files for Windows/Linux platforms are
supported as well as standard GNU makefiles. If you want to change some build target
properties modify the script files to fulfil your needs.



