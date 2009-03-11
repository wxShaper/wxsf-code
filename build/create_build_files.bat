@echo off

rem ========== Code::Blocks project files ==========
rem premake\premake-win32.exe --target cb-gcc --unicode --with-wx-shared --shared
rem premake\premake-win32.exe --target cb-gcc --unicode --static-runtime
echo done...
echo.

rem ==========CodeLite project files ==========
rem premake\premake-win32.exe --target cl-gcc --unicode --with-wx-shared --shared
premake\premake-win32.exe --target cl-gcc --unicode --static-runtime
echo done...
echo.

rem ========== GNU Makefile ==========
rem premake\premake-win32.exe --target gnu --unicode --with-wx-shared --shared
premake\premake-win32.exe --target gnu --unicode --static-runtime
echo done...
echo.

rem ========== MS Visual Studio 2005/2008 project files ==========
rem premake\premake-win32.exe --target vs2005 --unicode --with-wx-shared --shared --no-builtin-wchar
rem premake\premake-win32.exe --target vs2005 --unicode --static-runtime --no-builtin-wchar
premake\premake-win32.exe --target vs2005 --unicode --static-runtime
echo done...
echo.

echo Done generating all project files for wxShapeFramework