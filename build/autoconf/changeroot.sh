#!/bin/sh

# Author:        Francesco Montorsi
# Creation date: 4/8/2005
# RCS-ID:        $Id: changeroot.sh 504 2007-03-31 10:29:41Z frm $
# Purpose:       changes the user of a CVS local repository altering all 'Root' files.
#                The old 'Root' files are renamed as 'OldRoot'.
#
# NOTE: CVS works storing the user who checked-out the local repository in a file called
#       'Root' which is present inside all 'CVS' folders which typically appear as hidden
#       in the CVS clients.
#       This means that you don't have to specify the user (and all other info which are
#       included in the CVSROOT) each time you perform a CVS operation on the repository;
#       however, this also means that normally you cannot checkout a module with a username 
#       and then do operations on that module with another username.
#       This script just makes this possible changing the username in CVS-controlled files.


# $1 = the name of the folder where Root must be changed
# $2 = the old user name
# $3 = the new user name
function ChangeRoot
{
	
	for d in $(/bin/ls $1)
	do
	
		file=$1/$d		
	
		if [[ -d $file ]]; then

			if [[ -f "$file/Root" ]]; then
			
				echo "Changing the CVS user in folder: $file..."
				mv $file/Root $file/OldRoot
				cmd=s/$2/$3/
				sed "$cmd" $file/OldRoot > $file/Root
			
			fi
		
			echo "Recursing in $file"
			ChangeRoot $file $2 $3
		fi
	
	
	done

}


function PrintUsage {
	echo 1>&2 Invalid argument count !
	echo 1>&2 Usage: $0  pathToTheCVSrepository  olduser newuser
	echo 1>&2        The arguments must be in the specified order and must be space-separed.
	echo 1>&2
	echo 1>&2 Examples:
	echo 1>&2        $0 wxCode frm mySFusername
}




if [ $# -ne 3 ]; then		# we need exactly 3 arguments !
    PrintUsage $0
	exit 1
fi

ChangeRoot $1 $2 $3
