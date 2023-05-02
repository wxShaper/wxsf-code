#!/bin/sh

HOST_SYSTEM=`uname -s`
HOST_ARCH=`uname -m`
STARTDIR=`pwd`

CMAKE_BUILDDIR=build-$HOST_ARCH-$HOST_SYSTEM

if [ "$HOST_SYSTEM" = "Linux" ] 
then 
cmake -B $CMAKE_BUILDDIR

cmake --build . --target all --target test --target package 
cmake --build . --target package_source

else 

WXDIR=$STARTDIR/../wxWidgets/3.2.2.1

cmake -B $CMAKE_BUILDDIR -DwxWidgets_CONFIG_EXECUTABLE:FILEPATH=$WXDIR/build-debug/wx-config

cmake --build $CMAKE_BUILDDIR --target all
cmake --build $CMAKE_BUILDDIR --target test
cmake --build $CMAKE_BUILDDIR --target package 
cmake --build $CMAKE_BUILDDIR --target package_source

cmake -B build-Xcode -DwxWidgets_CONFIG_EXECUTABLE:FILEPATH=$WXDIR/build-debug/wx-config \
      -G Xcode

fi

