#!/bin/sh
IFS=$'\n'

source ./vs90vcvars32.sh

WHEREAMI=$(pwd)

export ROOTDIR=$WHEREAMI/build
export URL_ZLIB='http://zlib.net/zlib-1.2.7.tar.gz'
export URL_LIBPNG='ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng-1.0.60.tar.gz'
export URL_PIXMAN='http://cairographics.org/releases/pixman-0.26.2.tar.gz'
export URL_CAIRO='http://cairographics.org/releases/cairo-1.12.0.tar.gz'
export _LIBLINK='MT'
LIBPNG=libpng
ZLIB=zlib
PIXMAN=pixman
CAIRO=cairo

#//////////////////////////////////////////////////////////////////////////////

#replaces all -MD to -MT
change_makefile_to_static()
{
 if [ $_LIBLINK = 'MD' ]
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
 if [ $_LIBLINK = 'MD' ]
 then
     return
 fi
 echo change_makefile_zlib
 sed 's/zdll.lib/zlib/g' $1 > $1_
 mv $1_ $1
}

#replaces all RuntimeLibrary=x to RuntimeLibrary=0
change_vscprj_to_static()
{
 if [ $_LIBLINK = 'MD' ]
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
     MD) export _LIBLINK='MD' ;;
esac
}

to_dos_path()
{
 echo $1 | sed 's/\//\\/g' | sed 's/\\*cygdrive\\\([a-z]\)/\1:/i'
}

#replace: if XY -> ifeq ($(XY), 1)
missing_seperator_workaround()
{
 echo missing_seperator_workaround
 sed 's/if \([a-Z_]*\)/ifeq ($(\1), 1)/gi' $1 > $1_
 mv $1_ $1
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

cd $WHEREAMI

#build zlib
cd $ROOTDIR/$ZLIB
change_makefile_to_static $ROOTDIR/$ZLIB/win32/Makefile.msc
nmake -f win32/Makefile.msc #dosen't work correctly: STATIC.lib not found ?? => bat file even dosen't work!

#build libpng
cd $ROOTDIR/$LIBPNG
change_makefile_to_static $ROOTDIR/$LIBPNG/scripts/makefile.vcwin32
cp -r $ROOTDIR/$ZLIB $ROOTDIR/$LIBPNG/zlib
nmake -f scripts/makefile.vcwin32

#build pixman
cd $ROOTDIR/$PIXMAN
change_makefile_to_static $ROOTDIR/$PIXMAN/pixman/Makefile.win32
cd pixman
make -f Makefile.win32 "CFG=release"

source $WHEREAMI/vs90vcvars32.sh UNIX

#build cairo
cd $ROOTDIR/$CAIRO
#export INCLUDE=$INCLUDE;$(to_dos_path $ROOTDIR\zlib)
#export INCLUDE=$INCLUDE;$(to_dos_path $ROOTDIR\libpng)
#export INCLUDE=$INCLUDE;$(to_dos_path $ROOTDIR\pixman\pixman)
#export INCLUDE=$INCLUDE;$(to_dos_path $ROOTDIR\cairo\boilerplate)
#export INCLUDE=$INCLUDE;$(to_dos_path $ROOTDIR\cairo\src)
#export LIB=$LIB;$(to_dos_path $ROOTDIR\$ZLIB)
#export LIB=$LIB;$(to_dos_path $ROOTDIR\$PNG)
change_makefile_to_static $ROOTDIR/$CAIRO/build/Makefile.win32.common
change_makefile_zlib $ROOTDIR/$CAIRO/build/Makefile.win32.common
missing_seperator_workaround $ROOTDIR/$CAIRO/src/Makefile.sources

make -f Makefile.win32 "CFG=release"
