--*****************************************************************************
--*	Author:		Michal Bližňák
--*	Date:		18/11/2007
--*	Version:	1.00
--*	
--*	NOTES:
--*		- use the '/' slash for all paths.
--*****************************************************************************

dofile('../../build/premake/scripts/init.lua')

--******* Initial Setup ************
--*	Most of the setting are set here.
--**********************************

-- Set the name of your package.
package.name = "wxSFSample3"

-- Set this if you want a different name for your target than the package's name.
targetName = "wxSFSample3"

-- Set the kind of package you want to create.
--		Options: exe | winexe | lib | dll
if( windows) then
	package.kind = "winexe"
else
	package.kind = "exe"
end

-- Setup the package compiler settings.
if ( target == "vs2005" ) then
	-- Windows and Visual C++ 2005
	package.defines = { "_CRT_SECURE_NO_DEPRECATE", "_DISWARNINGS_MSVC" }
	package.buildflags = { "no-main" }
end
if( ( target == "vs2003" or target == "vs2005" ) and options["no-builtin-wchar"] ) then
	table.insert(package.buildoptions, "/Zc:wchar_t-")
end

-- Set the files to include.
package.files = { matchrecursive( "*.cpp", "*.h" ) }
if ( (windows) and not (target == "vs2005") ) then    
	table.insert(package.files, "wxSF.rc" )
end

-- Set the include paths.
package.includepaths = { "../../include" }

-- Set the libraries it links to.
package.links = { "wxSF" }

-- Set precompiled headers support
package.pchheader = "wx_pch.h"
package.pchsource = "wx_pch.cpp"

dofile('../../build/premake/scripts/wxpresets.lua')
