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

premake/premake-mac --target cb-gcc --wx-version $release $unicode $debug --with-wx-shared --shared
echo done...
echo 
#
premake/premake-mac --target gnu --wx-version $release $unicode $debug --with-wx-shared --shared
echo done...
echo 
#
echo Done generating all project files for wxShapeFramework.
#