@echo off
:Start
cls
echo.
echo -----=====================================================================-----
Echo ----==                 Welcome to the Build File Creator                 ==----
echo ---===                     For the WxShapeFramework                      ===---
echo --====              (http://sourceforge.net/projects/wxsf/)              ====--
Echo -=====                                                                   =====-
Echo --==== Creates you a make or project file for your environment of choice ====--
Echo ---===                                                                   ===---
Echo ----== Press Any Key To Continue                                         ==----
echo -----=====================================================================-----
pause>nul
:reset
set failselection=0
set target=nul
:IdeSelector
cls
echo -----=
Echo -----= Available build file formats
Echo -----=
echo -----= 1) Code::Blocks project
echo -----= 2) CodeLite project
echo -----= 3) GNU Makefile
echo -----= 4) MS Visual Studio 2005/2008 project
echo -----=
echo -----= e) Exit
echo -----=
if not %target%==nul (echo -----=%target% is not a valid selection) else echo -----=
set /p target=-----= Select a target: 
if %target%==1 goto CbSelected
if %target%==2 goto ClSelected
if %target%==3 goto GnuSelected
if %target%==4 goto MsSelected
if %target%==e goto Exit
goto IdeSelector

:CbSelected
cls
echo -----= 
Echo -----= Code::Blocks Options
echo -----= 1) --target cb-gcc --wx-version 28 --unicode --with-wx-shared --shared
echo -----= 2) --target cb-gcc --wx-version 28 --unicode --static-runtime
echo -----=
echo -----= b) Back
echo -----= e) Exit
set selection=1
if %failselection%==1 (echo -----= %selection% is not a valid selection) else echo -----=
set /p selection=-----= Select a parameter build (default 1): 

if %selection%==1 goto CbSelected%selection%
if %selection%==2 goto CbSelected%selection%
if %selection%==b goto Reset
if %selection%==e goto Exit

set failselection=1
goto CbSelected
:CbSelected1
cls
echo -----=
echo -----= Option "--target cb-gcc --wx-version 28 --unicode --with-wx-shared --shared" Selected
echo -----= Firing up the generator!
echo ------------------------------------
premake\premake-win32.exe --target cb-gcc --wx-version 28 --unicode --with-wx-shared --shared
echo ------------------------------------
echo -----= Done!
goto Exit
:CbSelected2
cls
echo -----=
echo -----= Option "--target cb-gcc --wx-version 28 --unicode --static-runtime" Selected
echo -----= Firing up the generator!
echo -----===============================
premake\premake-win32.exe --target cb-gcc --wx-version 28 --unicode --static-runtime
echo -----===============================
echo -----= Done!
goto Exit


:ClSelected
cls
echo -----= 
Echo -----= Codelite Options
echo -----= 1) --target cl-gcc --wx-version 28 --unicode --with-wx-shared --shared
echo -----= 2) --target cl-gcc --wx-version 28 --unicode --static-runtime
echo -----= 3) --target cl-gcc --wx-version 30 --unicode --with-wx-shared --shared --use-wx-config
echo -----=
echo -----= b) Back
echo -----= e) Exit
set selection=1
if %failselection%==1 (echo -----= %selection% is not a valid selection) else echo -----=
set /p selection=-----= Select a parameter build (default 1): 

if %selection%==1 goto ClSelected%selection%
if %selection%==2 goto ClSelected%selection%
if %selection%==3 goto ClSelected%selection%
if %selection%==b goto Reset
if %selection%==e goto Exit

set failselection=1
goto ClSelected
:ClSelected1
cls
echo -----=
echo -----= Option "--target cl-gcc --wx-version 28 --unicode --with-wx-shared --shared" Selected
echo -----= Firing up the generator!
echo -----===============================
premake\premake-win32.exe --target cl-gcc --wx-version 28 --unicode --with-wx-shared --shared
echo -----===============================
echo -----= Done!
goto Exit
:ClSelected2
cls
echo -----=
echo -----= Option "--target cl-gcc --wx-version 28 --unicode --static-runtime" Selected
echo -----= Firing up the generator!
echo -----===============================
premake\premake-win32.exe --target cl-gcc --wx-version 28 --unicode --static-runtime
echo -----===============================
echo -----= Done!
goto Exit
:ClSelected3
cls
echo -----=
echo -----= Option "--target cl-gcc --wx-version 30 --unicode --with-wx-shared --shared --use-wx-config" Selected
echo -----= Firing up the generator!
echo -----===============================
premake\premake-win32.exe --target cl-gcc --wx-version 30 --unicode --with-wx-shared --shared --use-wx-config
echo -----===============================
echo -----= Done!
goto Exit

:GnuSelected
cls
echo.
Echo -----= GNU Makefile Options
echo -----= 1) --target gnu --wx-version 28 --unicode --with-wx-shared --shared
echo -----= 2) --target gnu --wx-version 28 --unicode --static-runtime
echo -----= 3) --target gnu --wx-version 30 --unicode --with-wx-shared --shared --use-wx-config
echo -----=
echo -----= b) Back
echo -----= e) Exit
set selection=2
if %failselection%==1 (echo -----= %selection% is not a valid selection) else echo -----=
set /p selection=-----= Select a parameter build (default 2): 

if %selection%==1 goto GnuSelected%selection%
if %selection%==2 goto GnuSelected%selection%
if %selection%==3 goto GnuSelected%selection%
if %selection%==b goto Reset
if %selection%==e goto Exit

set failselection=1
goto GnuSelected
:GnuSelected1
cls
echo -----=
echo -----= Option "--target gnu --wx-version 28 --unicode --with-wx-shared --shared" Selected
echo -----= Firing up the generator!
echo -----===============================
premake\premake-win32.exe --target gnu --wx-version 28 --unicode --with-wx-shared --shared
echo -----===============================
echo -----= Done!
goto Exit
:GnuSelected2
cls
echo -----=
echo -----= Option "--target gnu --wx-version 28 --unicode --static-runtime" Selected
echo -----= Firing up the generator!
echo -----===============================
premake\premake-win32.exe --target gnu --wx-version 28 --unicode --static-runtime
echo -----===============================
echo -----= Done!
goto Exit
:GnuSelected3
cls
echo -----=
echo -----= Option "--target gnu --wx-version 30 --unicode --with-wx-shared --shared --use-wx-config" Selected
echo -----= Firing up the generator!
echo -----===============================

premake\premake-win32.exe --target gnu --wx-version 30 --unicode --with-wx-shared --shared --use-wx-config
echo -----===============================
echo -----= Done!
goto Exit

:MsSelected
cls
echo -----= 
Echo -----= Visual Studio Options
echo -----= 1) --target vs2005 --wx-version 28 --unicode --with-wx-shared --shared --no-builtin-wchar
echo -----= 2) --target vs2005 --wx-version 28 --unicode --with-wx-shared --shared
echo -----= 3) --target vs2005 --wx-version 28 --unicode --static-runtime --no-builtin-wchar
echo -----= 4) --target vs2005 --wx-version 28 --unicode --static-runtime
echo -----=
echo -----= b) Back
echo -----= e) Exit
set selection=2
if %failselection%==1 (echo -----= %selection% is not a valid selection) else echo -----=
set /p selection=-----= Select a parameter build (default 2): 

if %selection%==1 goto MsSelected%selection%
if %selection%==2 goto MsSelected%selection%
if %selection%==3 goto MsSelected%selection%
if %selection%==4 goto MsSelected%selection%
if %selection%==b goto Reset
if %selection%==e goto Exit

set failselection=1
goto MsSelected
:MsSelected1
cls
echo -----=
echo -----= Option "--target vs2005 --wx-version 28 --unicode --with-wx-shared --shared --no-builtin-wchar" Selected
echo -----= Firing up the generator!
echo -----===============================
premake\premake-win32.exe --target vs2005 --wx-version 28 --unicode --with-wx-shared --shared --no-builtin-wchar
echo -----===============================
echo -----= Done!
goto Exit
:MsSelected2
cls
echo -----=
echo -----= Option "--target vs2005 --wx-version 28 --unicode --with-wx-shared --shared" Selected
echo -----= Firing up the generator!
echo -----===============================
premake\premake-win32.exe --target vs2005 --wx-version 28 --unicode --with-wx-shared --shared
echo -----===============================
echo -----= Done!
goto Exit
:MsSelected3
cls
echo -----=
echo -----= Option "--target vs2005 --wx-version 28 --unicode --static-runtime --no-builtin-wchar" Selected
echo -----= Firing up the generator!
echo ------==============================
premake\premake-win32.exe --target vs2005 --wx-version 28 --unicode --static-runtime --no-builtin-wchar
echo -----===============================
echo -----= Done!
goto Exit
:MsSelected4
cls
echo -----=
echo -----= Option "--target vs2005 --wx-version 28 --unicode --static-runtime" Selected
echo -----= Firing up the generator!
echo -----===============================
premake\premake-win32.exe --target vs2005 --wx-version 28 --unicode --static-runtime
echo -----===============================
echo -----= Done!
goto Exit

:Exit
Echo -----=
echo -----= Exiting...
pause
