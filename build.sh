#!/bin/sh

os=$(uname -s)
echo building on $os:

if [ $# -eq 0 ]
then
	echo "missing clib dir location."
	exit 0
fi

#usage
usage() 
{
	echo $0 "-i clib_location [-g generator(cmmake generator default=Unix Makefiles)]"
}

mode="Unix Makefiles"

#passing arguments
while [ "$1" != "" ]; do
    case $1 in
        -i | --in )             shift
                                in=$1
                                ;;
        -m | --mode )         shift
				mode=$1							
                                ;;
        -h | --help )           usage
                                exit
                                ;;
        * )                     usage
                                exit 1
    esac
    shift
done

export CLIBS=$in
cmake .
make
