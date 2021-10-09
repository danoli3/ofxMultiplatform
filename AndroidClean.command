#!/bin/sh

# The MIT License (MIT)

# Copyright (c) 2014 Daniel Rosser

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

scriptdirectory=$(dirname $0)
cd $scriptdirectory

echo "----------------------------------"
echo "Android Build - Clean Script v1.0"
echo "----------------------------------"

echo "Android :: Cleaning Debug Build"
make CleanDebug PLATFORM_OS=Android -j8
echo "----------------------------------"
echo "Cleaned Debug"
echo "----------------------------------"
echo "Android :: Cleaning Release Build"
make CleanRelease PLATFORM_OS=Android -j8
echo "----------------------------------"
echo "Cleaned Release"
echo "----------------------------------"
echo "Removing any zipped bin/data resources"
rm res/raw/*.*
echo "Completed removing zipped bin/data"
echo "----------------------------------"
echo "Clean Completed"
