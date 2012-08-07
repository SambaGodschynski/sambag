#!/bin/sh

check_file()
{
    if ! [ -e $1 ]
    then
        echo Assumed file $1 dosent exists!
        exit 1
    fi
}

check_directory()
{
    if ! [ -d $1 ]
    then
        echo Assumed directory $1 dosent exists!
        exit 1
    fi
}

clean_directory()
{
    if [ -d $1 ]
    then
        echo clean $1
        rm -rf $1
    fi
}

_done()
{
 echo DONE
 echo ----
}

if ! [ -e $2 ]
then
    echo Downloading $1:
    curl $1 -o $2
    _done
fi
check_file $2
clean_directory $3
echo Extract $2:
tar -xzf $2
mv $3-* $3
_done

