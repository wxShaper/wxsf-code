#!/bin/bash

echo
echo This script file will install the 'wxcodecheck.m4' file in your aclocal path.
echo If you do not want to proceed, press CTRL+C, otherwise just press a key.
read

cmd="cp wxcodecheck.m4 `aclocal --print-ac-dir`"
if [ `whoami` != "root" ]; then
    echo You need superuser rights to proceed...
    su -c "$cmd"
else
    $cmd
fi
if [ "$?" = "0" ]; then
    echo File installed successfully.
fi
