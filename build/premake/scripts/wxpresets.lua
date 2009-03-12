--*****************************************************************************
--*	Author:		Michal Bli¾òák
--*	Date:		18/11/2007
--*	Version:	1.00
--*	
--*	NOTES:
--*		- use the '/' slash for all paths.
--*****************************************************************************

--******* GENERAL SETUP **********
--*	Settings that are not dependant
--*	on the operating system.
--*********************************

-- Package options
if ( not windows ) then
	addoption( "disable-wx-debug", "Compile against a wxWidgets library without debugging" )
end

-- Common setup
package.language = "c++"

-- Set object output directory.
if ( options["unicode"] ) then
	package.config["Debug"].objdir = ".objsud"
	package.config["Release"].objdir = ".objsu"
else
	package.config["Debug"].objdir = ".objsd"
	package.config["Release"].objdir = ".objs"
end

-- Set debug flags
if ( options["disable-wx-debug"] and ( not windows ) ) then
	debug_option = "--debug=no"
	debug_macro = { "NDEBUG" }
else
	debug_option = "--debug=yes"
	debug_macro = { "DEBUG", "_DEBUG", "__WXDEBUG__" }
end

-- Set the default targetName if none is specified.
if ( string.len( targetName ) == 0 ) then
	targetName = package.name
end

-- Set the target names
if ( package.kind == "winexe" or package.kind == "exe" ) then
	package.config["Release"].target = targetName
	package.config["Debug"].target = targetName.."d"
else
	if( windows ) then
		package.config["Release"].target = wx_target .. wx_release .. usign.. "_" .. targetName
		package.config["Debug"].target = wx_target .. wx_release .. usign .. "d_" .. targetName
	else
		package.config["Release"].target = wx_target .. usign .. "_" .. targetName .. "-" .. wx_release
		package.config["Debug"].target = wx_target .. usign .. "d_" .. targetName .. "-" .. wx_release
	end
end

-- Set the build options.
table.insert( package.buildflags, "extra-warnings" )

if( options["static-runtime"] ) then
	table.insert( package.buildflags, "static-runtime" )	
end

table.insert( package.config["Release"].buildflags, "no-symbols" )
table.insert( package.config["Release"].buildflags, "optimize-speed" )

if ( options["unicode"] ) then
	table.insert( package.buildflags, "unicode" )
end

if ( target == "cb-gcc" or target == "gnu" or target == "cl-gcc") then
--	table.insert( package.buildflags, "no-import-lib" )
	table.insert( package.config["Debug"].buildoptions, "-O0" )
	table.insert( package.config["Release"].buildoptions, "-fno-strict-aliasing" )
end

-- Set the defines.
if ( options["with-wx-shared"] ) then
	table.insert( package.defines, "WXUSINGDLL" )
end
if ( options["unicode"] ) then
	table.insert( package.defines, { "UNICODE", "_UNICODE" } )
end
table.insert( package.defines, "__WX__" )
table.insert( package.config["Debug"].defines, debug_macro )
table.insert( package.config["Release"].defines, "NDEBUG" )

if ( windows ) then
--******* WINDOWS SETUP ***********
--*	Settings that are Windows specific.
--*********************************
	-- Set wxWidgets include paths 
	if ( target == "cb-gcc" ) then
		table.insert( package.includepaths, "$(#WX.include)" )
	else
		table.insert( package.includepaths, "$(WXWIN)/include" )
	end
	
	-- Set the correct 'setup.h' include path.
	if ( options["with-wx-shared"] ) then
		if ( options["unicode"] ) then
			if ( target == "cb-gcc" ) then
				table.insert( package.config["Debug"].includepaths, "$(#WX.lib)/gcc_dll/mswud" )
				table.insert( package.config["Release"].includepaths, "$(#WX.lib)/gcc_dll/mswu" )
			elseif ( target == "gnu" or target == "cl-gcc" ) then
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/gcc_dll/mswud" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/gcc_dll/mswu" )
			else
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/vc_dll/mswud" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/vc_dll/mswu" )
			end
		else
			if ( target == "cb-gcc" ) then
				table.insert( package.config["Debug"].includepaths, "$(#WX.lib)/gcc_dll/mswd" )
				table.insert( package.config["Release"].includepaths, "$(#WX.lib)/gcc_dll/msw" )
			elseif ( target == "gnu" or target == "cl-gcc") then
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/gcc_dll/mswd" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/gcc_dll/msw" )
			else
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/vc_dll/mswd" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/vc_dll/msw" )
			end
		end
	else
		if ( options["unicode"] ) then
			if ( target == "cb-gcc" ) then
				table.insert( package.config["Debug"].includepaths, "$(#WX.lib)/gcc_lib/mswud" )
				table.insert( package.config["Release"].includepaths, "$(#WX.lib)/gcc_lib/mswu" )
			elseif ( target == "gnu" or target == "cl-gcc") then
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/gcc_lib/mswud" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/gcc_lib/mswu" )
			else
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/vc_lib/mswud" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/vc_lib/mswu" )
			end
		else
			if ( target == "cb-gcc" ) then
				table.insert( package.config["Debug"].includepaths, "$(#WX.lib)/gcc_lib/mswd" )
				table.insert( package.config["Release"].includepaths, "$(#WX.lib)/gcc_lib/msw" )
			elseif ( target == "gnu" or target == "cl-gcc") then
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/gcc_lib/mswd" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/gcc_lib/msw" )
			else
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/vc_lib/mswd" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/vc_lib/msw" )
			end
		end
	end
	
	-- Set the linker options.
	if ( options["with-wx-shared"] ) then
		if ( target == "cb-gcc" ) then
			table.insert( package.libpaths, "$(#WX.lib)/gcc_dll" )
		elseif ( target == "gnu" or target == "cl-gcc" ) then
			table.insert( package.libpaths, "$(WXWIN)/lib/gcc_dll" )
		else
			table.insert( package.libpaths, "$(WXWIN)/lib/vc_dll" )
		end
	else
		if ( target == "cb-gcc" ) then
			table.insert( package.libpaths, "$(#WX.lib)/gcc_lib" )
		elseif ( target == "gnu" or target == "cl-gcc" ) then
			table.insert( package.libpaths, "$(WXWIN)/lib/gcc_lib" )
		else
			table.insert( package.libpaths, "$(WXWIN)/lib/vc_lib" )
		end
	end
	
	-- Set wxWidgets libraries to link.
	if ( options["unicode"] ) then
		table.insert( package.config["Release"].links, "wxmsw"..wx_release.."u" )
		table.insert( package.config["Debug"].links, "wxmsw"..wx_release.."ud" )
	else
		table.insert( package.config["Release"].links, "wxmsw"..wx_release )
		table.insert( package.config["Debug"].links, "wxmsw"..wx_release.."d" )
	end

	if ( not options["with-wx-shared"] ) then
		if ( options["unicode"] ) then
			table.insert( package.config["Debug"].links, { "wxexpatd", "wxjpegd", "wxpngd", "wxtiffd", "wxregexud" } )
			table.insert( package.config["Release"].links, { "wxexpat", "wxjpeg", "wxpng", "wxtiff", "wxregexu" } )
		else
			table.insert( package.config["Debug"].links, { "wxexpatd", "wxjpegd", "wxpngd", "wxtiffd", "wxregexd" } )
			table.insert( package.config["Release"].links, { "wxexpat", "wxjpeg", "wxpng", "wxtiff", "wxregex" } )
		end

		if ( target == "cb-gcc" or target == "gnu" or target == "cl-gcc" ) then
			table.insert( package.config["Debug"].links, { "winmm", "rpcrt4", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "comctl32", "wsock32", "odbc32" } )
			table.insert( package.config["Release"].links, { "winmm", "rpcrt4", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "comctl32", "wsock32", "odbc32" } )
		else
			table.insert( package.config["Debug"].links, { "rpcrt4", "comctl32" } )
			table.insert( package.config["Release"].links, { "rpcrt4", "comctl32" } )
		end
	end

	
	-- Set the Windows defines.
	table.insert( package.defines, { "__WXMSW__", "WIN32", "_WINDOWS" } )
else
--******* LINUX/MAC SETUP *************
--*	Settings that are Linux/Mac specific.
--*************************************
	-- Ignore resource files in Linux/Mac.
	table.insert( package.excludes, matchrecursive( "*.rc" ) )
	
	-- Add buildflag for proper dll building.
	if ( macosx ) then
		table.insert( package.buildflags, "dylib" )
	end
	
	-- Set wxWidgets build options.
	table.insert( package.config["Debug"].buildoptions, "`wx-config "..debug_option.." --cflags`" )
	table.insert( package.config["Release"].buildoptions, "`wx-config --debug=no --cflags`" )
	
	-- Set the wxWidgets link options.
	table.insert( package.config["Debug"].linkoptions, "`wx-config "..debug_option.." --libs`" )
	table.insert( package.config["Release"].linkoptions, "`wx-config --libs`" )
end

