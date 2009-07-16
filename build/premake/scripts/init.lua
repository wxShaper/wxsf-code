--*****************************************************************************
--*	Author:		Michal Bližňák
--*	Date:		18/11/2007
--*	Version:	1.00
--*	
--*	NOTES:
--*		- use the '/' slash for all paths.
--*****************************************************************************

function trim (s)
    return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

--******* Initial Setup ************
--*	Most of the setting are set here.
--**********************************

-- Adding additional options to support all the build types.
addoption( "shared", "Create a dynamic link library (.dll) version" )
addoption( "static-runtime", "Use static runtime C/C++ libraries" )
addoption( "no-builtin-wchar", "Do not treat wchar_t as a builtin type" )
addoption( "unicode", "Use the Unicode character set" )
addoption( "with-wx-shared", "Link against wxWidgets as a shared library" )
addoption( "wx-version", "Used wxWidgets version" )

wx_target = ""
if ( windows ) then
	wx_target = "wxmsw"
elseif ( linux ) then
	wx_target = "wx_gtk2"
elseif ( macosx ) then
	wx_target = "wx_mac"
end

usign = ""
if( options["unicode"] ) then
	usign = "u" 
end

wx_release = options["wx-version"]
