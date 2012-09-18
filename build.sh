#!/bin/sh

if [ $# -eq 0 ]
then
	echo "missing clib dir location."
	exit 0
fi

export CLIBS=$1
export Boost_ROOT=$CLIBS/boost
cmake .
make install
