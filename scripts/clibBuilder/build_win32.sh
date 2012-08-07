#!/bin/sh
IFS=$'\n'

export ROOTDIR='build'
export URL_ZLIB='http://zlib.net/zlib-1.2.7.tar.gz'
export URL_LIBPNG='ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng-1.0.60.tar.gz'
export STATIC=1
LIBPNG=libpng
ZLIB=zlib
#passing arguments

case $@ in
     CLEAN_ALL) export CLEAN_ALL=1
                rm -rf $ROOTDIR ;;
     DYNAMIC) export STATIC='' ;;
esac


mkdir -p $ROOTDIR
#                     $URL      $DST_FILE   $DST_DIRECTORY
sh downloadpackage.sh $URL_ZLIB zlib.tar.gz $ZLIB
sh downloadpackage.sh $URL_LIBPNG libpng.tar.gz $LIBPNG

#replaces all -MD to -MT
change_makefile_to_static()
{
 if [ -z $STATIC ]
 then
     return
 fi
 echo change_makefile_to_static
 sed 's/-MD/-MT/g' $1 > $1_
 mv $1_ $1
}

#replaces all RuntimeLibrary=x to RuntimeLibrary=0
change_vscprj_to_static()
{
 if [ -z $STATIC ]
 then
     return
 fi
 echo change_makefile_to_static
 sed '/RuntimeLibrary=/s/[0-9]/0/' $1 > $1_
 mv $1_ $1
}

#build zlib
export DST_ZLIB=$ROOTDIR/$ZLIB
change_makefile_to_static $ROOTDIR/$ZLIB/win32/Makefile.msc
cmd /c makeZlib.bat

#build libpng
export DST_LIBPNG=$ROOTDIR/$LIBPNG #/projects/visualc71
change_makefile_to_static $ROOTDIR/$LIBPNG/scripts/makefile.vcwin32
cp -r $ROOTDIR/$ZLIB $ROOTDIR/$LIBPNG/zlib

cmd /c makeLibpng.bat
