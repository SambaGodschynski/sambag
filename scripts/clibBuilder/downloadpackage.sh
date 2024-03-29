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
    curl -L $1 -o $2
    _done
fi
check_file $2
clean_directory $3
echo Extract $2:
if [ $(echo $1 | grep '\.xz$') ]  # tar.xz
then
    tar -Jxf $2
else
    tar -xzf $2
fi
sleep 1   #permission problems when move starts directly
mv -f $3-* $3
_done

