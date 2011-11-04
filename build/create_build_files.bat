@echo off

rem ========== Code::Blocks project files ==========
rem premake\premake-win32.exe --target cb-gcc --wx-version 28 --unicode --with-wx-shared --shared
rem premake\premake-win32.exe --target cb-gcc --wx-version 28 --unicode --static-runtime
rem echo done...
rem echo.

rem ==========CodeLite project files ==========
premake\premake-win32.exe --target cl-gcc --wx-version 28 --unicode --with-wx-shared --shared
rem premake\premake-win32.exe --target cl-gcc --wx-version 28 --unicode --static-runtime
echo done...
echo.

rem ========== GNU Makefile ==========
rem premake\premake-win32.exe --target gnu --wx-version 28 --unicode --with-wx-shared --shared
premake\premake-win32.exe --target gnu --wx-version 28 --unicode --static-runtime
echo done...
echo.

rem ========== MS Visual Studio 2005/2008 project files ==========
rem premake\premake-win32.exe --target vs2005 --wx-version 28 --unicode --with-wx-shared --shared --no-builtin-wchar
premake\premake-win32.exe --target vs2005 --wx-version 28 --unicode --with-wx-shared --shared
rem premake\premake-win32.exe --target vs2005 --wx-version 28 --unicode --static-runtime --no-builtin-wchar
rem premake\premake-win32.exe --target vs2005 --wx-version 28 --unicode --static-runtime
echo done...
echo.

echo Done generating all project files for wxShapeFramework.