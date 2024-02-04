#!/bin/sh

# The MIT License (MIT)

# Copyright (c) 2014 Daniel Rosser & Colin Friend

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

echo "----------------------------------"
echo "ofxAddonScript v1.21 (OSX Edition)."
echo "----------------------------------"



# Global Variables (only addons path at the moment)
# Variable: relativeaddonpath - Make this the relative location to addons folder
# Default: "../../addons" however this very likely could be "../../../addons/"
# ---------------------------------------------------------------------------------
#relativeaddonpath="../../addons"      # normal oF addons->addons location
relativeaddonpath="../../../addons"  # normal oF Project->addons location

##########################################################################################
####### ADD YOUR ADDONS AT THE BOTTOM OF THE SCRIPT. SCROLL-DOWN ;D! Line >190 ###########
##########################################################################################
echo "Global Addons Path: $relativeaddonpath"
echo "----------------------------------"

# Check if Git's installed (yet to be tested if Git's not installed)
if test "$(git --version 2>&1 | echo)" = "\n"
then
	echo "----------------------------------"
    echo "FATAL ERROR: Git not installed. Script terminating..."
    echo "----------------------------------"
    exit 1
fi
# Get script directory (needed to run the script from any location!)
scriptdirectory=$(dirname $0)

#-------------------------------------------------------------- GetAddon Function
# Function to get an addon
# Param1: Addon folder name
# Param2: Github address
# Param3: Branch or SHA commit
GetAddon(){

# NOTE: Change the following string if the addons folder is not two levels up from the directory of this script.
addonsdirectory="$scriptdirectory/$relativeaddonpath"
branch="$3"

# if a branch is not specified - Default to master
if [ "$branch" == "" ]
then
	branch=""
fi
if [ "$1" == "" ]
then
 	echo "----------------------"
	echo "FATAL ERROR! Parameter 1 - 'Addon Folder Name' No specified!"
	echo "Example usage: 'GetAddon \"ofxAddonScript\" \"https://github.com/danoli3/ofxAddonScript.git\"' "
	echo "----------------------"
	exit;
fi
if [ "$2" == "" ]
then
 	echo "----------------------"
	echo "FATAL ERROR! Parameter 2 - 'Github address' not specified"
	echo "Example usage: 'GetAddon \"ofxAddonScript\" \"https://github.com/danoli3/ofxAddonScript.git\"' "
	echo "----------------------"
	exit;
fi
echo "========================"
echo "Name:    $1"
echo "Origin:  $2"
if [ "$branch" != "" ]
then
echo "Branch/SHA:  $branch"
fi

echo "------------------------"
# Check the addon location
if [ -d $addonsdirectory ]
then
	echo "openFrameworks Addon Directory Found!"
	echo "------------------------"
	addondirectory="$addonsdirectory/$1/"
	if [ -d $addondirectory ]
	then
    	# Update if the directory exists
    	cd "$addondirectory"
    	echo "Addon directory already exists! '$1'!"
    	echo "------------------------"
#    	echo "Verbose: Addon Full Path: \n $addondirectory" # verbose
		if [ "$branch" == "" ]
		then
			branch=$(git branch | sed -n -e 's/^\* \(.*\)/\1/p')
			echo "Current branch is set to: $branch"
		else
			echo "Setting branch/commit to: $branch"
			# checkout a particular branch or SHA commit
			git checkout $branch
		fi
		echo "------------------------"
    	echo "Now attempting update on $branch..."
   		git pull
   		echo "------------------------"
    	echo "$1 successfully updated"  #assuming git pull does not fail actually...
    else
    	# Clone if the directory doesn't exist
   	 	echo "$1 does not exist in the addons directory."
#  	 	echo "$relativeaddonpath/$relativeaddonpath" # verbose
    	cd "$addonsdirectory"
	    echo "Checking out $1 into $relativeaddonpath/$1"
    	echo "------------------------"
    	git clone $2 $1
    	echo "------------------------"
    	echo "$1 cloned successfully!"
    	echo "------------------------"
    	cd "$addondirectory"
    	echo "Setting branch/commit to: $branch"
		git checkout $branch
		echo "------------------------"
    	echo "Now attempting update on $branch..."
   		git pull
    fi
else
    # Addons Folder not in location
    echo "------------------------"
    echo "ERROR! Addons directory not found at: $relativeaddonpath"
#   echo "Verbose: Addons Full Path: \n $addonsdirectory" # verbose
    echo "------------------------"
fi

# Reset environment location back to script directory.
cd $scriptdirectory
echo "========================"
}

##----------------------------- Example Script Information -------------------------------
#########################################################################################
# GetAddon Function to get an addon
# Param1: Addon folder name
# Param2: Github address
# Param3: Branch or SHA commit 
#
# Example Usage:
## Normal way using two parameters to master and latest commit
# GetAddon "ofxAddonScript" "https://github.com/danoli3/ofxAddonScript.git"  
## Advanced way using 
# GetAddon "ofxAddonScript" "https://github.com/danoli3/ofxAddonScript.git" "master" 
## Advanced way using a specific SHA commit code (only use if you want a repo set to a specific commit).
# GetAddon "ofxAddonScript" "https://github.com/danoli3/ofxAddonScript.git" "78fd6f27cf82743644f0c12f926ae053a42a7aa3"
#########################################################################################
##------------------------------- MODIFY BELOW HERE!!! ----------------------------------

###### --- Get following Addons --- Add your addons below! ;D! -- ######
#GetAddon "ofxAddonScript" "https://github.com/danoli3/ofxAddonScript.git" 


