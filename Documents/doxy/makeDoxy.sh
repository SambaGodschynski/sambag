#!/bin/sh

VERSION=$(svnversion ../. | grep -o "[0-9:]*")
sed 's/^PROJECT_NUMBER.*$/PROJECT_NUMBER = '$VERSION'/g' Doxyfile.org > Doxyfile
doxygen Doxyfile
