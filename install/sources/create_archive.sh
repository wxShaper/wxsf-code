#!/bin/sh

# test whether wxShapeFramework version is given
if [ ! -n "$1" ];
then
	echo "Version string must be defined as the script parameter."
	exit 1
fi

# archive name
out="wxSF"

# create temporary dir
echo "Removing old temporary directory..."
rm -rf $out
mkdir $out

# copy program files
echo "Updating build files..."
cd ../../build
bakefile_gen
./acregen.sh
cd ../install/sources

echo "Copying build files..."
mkdir $out/build
cp -R -L ../../build/autoconf $out/build
cp -R -L ../../build/bakefiles $out/build
cp -R -L ../../build/premake $out/build
cp ../../build/create_build_files*.* $out/build
cp ../../build/premake.lua $out/build
cp ../../build/acregen.sh $out/build
cp ../../build/Bakefiles.bkgen $out/build
cp ../../build/config.guess $out/build
cp ../../build/config.sub $out/build
cp ../../build/configure.ac $out/build
cp ../../build/install.sh $out/build
cp ../../build/wxsf.bkl $out/build
cp ../../configure $out
cp ../../Makefile.in $out

echo "Copying lib files..."
mkdir $out/lib
cp ../../lib/ReadMe.txt $out/lib

echo "Updating documentation files..."
cd ../../doc
doxygen Doxyfile
cd ../install/sources

echo "Copying documentation files..."
mkdir $out/doc
cp -R -L ../../doc/html $out/doc
cp ../../doc/Doxyfile $out/doc
cp ../../doc/wxShapeFramework.chm $out/doc
cp "../../doc/wxSF_WSEAS TRANSACTIONS on COMPUTERS.pdf" $out/doc

echo "Copying source files..."
cp -R -L ../../include $out
cp -R -L ../../src $out
cp ../../license.txt $out
cp ../../changes.txt $out
cp ../../readme_bin.txt $out
cp ../../readme_src.txt $out
cp ../../readme_samples.txt $out

echo "Copying sample projects files..."
cp -R -L ../../samples $out

# create archive
echo "Remove unneeded files and directories..."
rm -rf `find $out -type d -name .svn`
rm -rf `find $out -type d -name .objsu`
rm -rf `find $out -type d -name .objsud`
rm -rf `find $out -type d -name bin`
rm -rf `find $out -type f -name *.project`
rm -rf `find $out -type f -name *.mk`
rm -rf `find $out -type f -name *.gch`
rm -rf `find $out -type f -name *.layout`
rm -rf `find $out -type f -name *.depend`
rm -rf `find $out -type f -name Makefile`

echo "Creating archive..."
tar -cvzf wxSF_src-$1.tgz $out

# remove temporary dir
rm -rf $out

echo "Done."

