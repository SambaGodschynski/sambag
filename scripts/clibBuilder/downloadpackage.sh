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

if ! [ -e $ROOTDIR/$2 ]
then
    echo Downloading $1:
    curl $1 -o $ROOTDIR/$2
    _done
fi
check_file $ROOTDIR/$2
clean_directory $ROOTDIR/$3
echo Extract $2:
tar -xzf $ROOTDIR/$2 -C $ROOTDIR/
mv $ROOTDIR/$3-* $ROOTDIR/$3
_done

