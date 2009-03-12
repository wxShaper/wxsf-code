#!/bin/sh
#

# Autodetect wxWidgets settings
if wx-config --unicode >/dev/null 2>/dev/null; then
	unicode="--unicode"
fi
if ! wx-config --debug >/dev/null 2>/dev/null; then
	debug="--disable-wx-debug"
fi
release=`wx-config --release`

# ========== Code::Blocks project files ==========
#premake/premake-linux --target cb-gcc --wx-version $release --shared --with-wx-shared $unicode $debug $1
#premake/premake-linux --target cb-gcc --wx-version $release $unicode $debug $1
echo done...
echo 
#
# ========== CodeLite project files ==========
premake/premake-linux --target cl-gcc --wx-version $release --shared --with-wx-shared $unicode $debug $1
#premake/premake-linux --target cl-gcc --wx-version $release $unicode $debug $1
echo done...
echo 
#
# ========== GNU Makefile ==========
premake/premake-linux --target gnu --wx-version $release --shared --with-wx-shared $unicode $debug $1
#premake/premake-linux --target gnu --wx-version $release $unicode $debug $1
echo done...
echo 
#
echo Done generating all project files for wxShapeFramework
#
