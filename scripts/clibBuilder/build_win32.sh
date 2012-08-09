#!/bin/sh
IFS=$'\n'

source ./vs90vcvars32.sh

WHEREAMI=$(pwd)

export ROOTDIR=$WHEREAMI/build
export URL_ZLIB='http://zlib.net/zlib-1.2.7.tar.gz'
export URL_LIBPNG='ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng-1.0.60.tar.gz'
export URL_PIXMAN='http://cairographics.org/releases/pixman-0.20.0.tar.gz'
export URL_CAIRO='http://cairographics.org/releases/cairo-1.10.2.tar.gz'
export LINK='STATIC'
LIBPNG=libpng
ZLIB=zlib
PIXMAN=pixman
CAIRO=cairo

#//////////////////////////////////////////////////////////////////////////////

#replaces all -MD to -MT
change_makefile_to_static()
{
 if [ $LINK = 'DYNAMIC' ]
 then
     return
 fi
 echo change_makefile_to_static
 sed 's/-MD/-MT/g' $1 > $1_
 mv $1_ $1
}

#replaces all zdll.lib to zlib.lib
change_makefile_zlib()
{
 if [ $LINK = 'DYNAMIC' ]
 then
     return
 fi
 echo change_makefile_to_static
 sed 's/zdll.lib/zlib/g' $1 > $1_
 mv $1_ $1
}

#replaces all RuntimeLibrary=x to RuntimeLibrary=0
change_vscprj_to_static()
{
 if [ $LINK = 'DYNAMIC' ]
 then
     return
 fi
 echo change_makefile_to_static
 sed '/RuntimeLibrary=/s/[0-9]/0/' $1 > $1_
 mv $1_ $1
}

#passing arguments
arg()
{
case $1 in
     CLEAN_ALL) export CLEAN_ALL=1
                rm -rf $ROOTDIR ;;
     DYNAMIC) export LINK='DYNAMIC' ;;
esac
}
#//////////////////////////////////////////////////////////////////////////////

arg $@

#download packages
mkdir -p $ROOTDIR
cd $ROOTDIR
#                     $URL      $DST_FILE   $DST_DIRECTORY
sh $WHEREAMI/downloadpackage.sh $URL_ZLIB zlib.tar.gz $ZLIB
sh $WHEREAMI/downloadpackage.sh $URL_LIBPNG libpng.tar.gz $LIBPNG
sh $WHEREAMI/downloadpackage.sh $URL_PIXMAN pixman.tar.gz $PIXMAN
sh $WHEREAMI/downloadpackage.sh $URL_CAIRO cairo.tar.gz $CAIRO

#build zlib
export DST_ZLIB=$ROOTDIR/$ZLIB
change_makefile_to_static $ROOTDIR/$ZLIB/win32/Makefile.msc
#nmake -f win32/Makefile.msc #dosen't work correctly: STATIC.lib not found??
cmd /c makeZlib.bat

#build libpng
export DST_LIBPNG=$ROOTDIR/$PNG
change_makefile_to_static $ROOTDIR/$LIBPNG/scripts/makefile.vcwin32
cp -r $ROOTDIR/$ZLIB $ROOTDIR/$LIBPNG/zlib
#nmake -f scripts/makefile.vcwin32
cmd /c makeLibpng.bat

#build pixman
cd $ROOTDIR/$PIXMAN
change_makefile_to_static $ROOTDIR/$PIXMAN/pixman/Makefile.win32
cd pixman
make -f Makefile.win32 "CFG=release"

#build cairo
cd $ROOTDIR/$CAIRO
export INCLUDE=$INCLUDE:$ROOTDIR\zlib
export INCLUDE=$INCLUDE:$ROOTDIR\libpng
export INCLUDE=$INCLUDE:$ROOTDIR\pixman\pixman
export INCLUDE=$INCLUDE:$ROOTDIR\cairo\boilerplate
export INCLUDE=$INCLUDE:$ROOTDIR\cairo\src
export LIB=$LIB:$ROOTDIR\$ZLIN
export LIB=$LIB:$ROOTDIR\$PNG
change_makefile_to_static $ROOTDIR/$CAIRO/build/Makefile.win32.common
change_makefile_zlib $ROOTDIR/$CAIRO/build/Makefile.win32.common
make -f Makefile.win32 "CFG=release"
