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
	echo $0 "-i clib_location [-g generator(cmmake generator default=Unix Makefiles) -d(debug mode)]"
}

mode=""
flags=""
build_type="-D CMAKE_BUILD_TYPE=Release"
#passing arguments
while [ "$1" != "" ]; do
    case $1 in
        -i | --in )             shift
                                in=$1
                                ;;
        -g | --generator )      shift
	                        #TODO: flags="-G '$1'" 
				mode=$1
                                ;;
        -d )                    build_type="-D CMAKE_BUILD_TYPE=Debug"
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
echo use generator: $mode
flags="$flags $build_type"
if [ "$mode" = "" ]
echo "$flags"
then
  cmake $flags .
else
  cmake $flags -G "$mode" .
fi

