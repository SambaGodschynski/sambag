#!/bin/bash renames occurrences of pattern $1 to $2 using find and sed
# $1 the start path
# $2 a file pattern
# $3 a regex pattern for matching
# $4 a sed replace expression
# performs a sed
perform() 
{
	sed "s/$RGX/$RPC/" $1
}

if [ $# -lt 4 ] 
then
	echo usage: $0 "start-path file-pattern regex-pattern sed-replace"
	exit 0
fi
export -f perform
export RGX=$3
export RPC=$4
find $1 -name $2 -exec bash -c 'perform "$0"' \{\} \;
