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

# ========== CodeLite project files ==========
#premake/premake-mac --target cl-gcc --wx-version $release $unicode $debug --with-wx-shared --shared
premake/premake-mac --target cl-gcc --wx-version $release $unicode $debug
echo done...
echo 
#
# ========== GNU Makefile ==========
#premake/premake-mac --target gnu --wx-version $release $unicode $debug --with-wx-shared --shared
premake/premake-mac --target gnu --wx-version $release $unicode $debug
echo done...
echo 
#
echo Done generating all project files for wxShapeFramework.
#
