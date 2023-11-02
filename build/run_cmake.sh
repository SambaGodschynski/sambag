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
	echo $0 "-i clib_location [-g generator(cmmake generator default=Unix Makefiles) -d(debug mode) -l(uses log)]"
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
       
	-f | --flag )		shift
				flags="$flags $1"
				;;
	
        -l | --use-log)         export SAMBA_USELOG=1
				;;

	 * )                     usage
                                exit 1
    esac
    shift
done
export CLIBS=$(realpath  $in)

echo use generator: $mode
flags="$flags $build_type"
echo $flags
if [ "$mode" = "" ]
then
  cmake $flags ..
else
  cmake $flags -G "$mode" ..
fi

