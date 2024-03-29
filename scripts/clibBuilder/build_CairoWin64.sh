#!/bin/sh
IFS=$'\n'

WHEREAMI=$(pwd)

export ROOTDIR=$WHEREAMI/build
export URL_ZLIB='https://github.com/madler/zlib/releases/download/v1.2.13/zlib-1.2.13.tar.gz'
export URL_LIBPNG='http://prdownloads.sourceforge.net/libpng/libpng-1.6.37.tar.gz?download'
export URL_PIXMAN='https://cairographics.org/releases/pixman-0.40.0.tar.gz'
export URL_CAIRO='https://cairographics.org/releases/cairo-1.16.0.tar.xz'
export _LIBLINK='MD'
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

#replaces all -MD to -MT pixman workaround: pixman has no MD -> MD is default?
change_makefile_to_static_pixman()
{
 if [ $_LIBLINK = 'MD' ]
 then
     return
 fi
 echo change_makefile_to_static_pixman
 sed 's/PIXMAN_CFLAGS +=\(.*\)/PIXMAN_CFLAGS += \1 -MT -DPIXMAN_NO_TLS/g' $1 > $1_
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
 sed 's/zdll.lib/zlib.lib/g' $1 > $1_
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
for x in $@
do
  case $x in
       CLEAN_ALL) export CLEAN_ALL=1
                  rm -rf $ROOTDIR ;;
       MT) export _LIBLINK='MT' ;;
       DEBUG) export _CONFIG=debug;;
  esac
done
if [ -z $_CONFIG ]
then
    _CONFIG=release
fi
echo build options: $_CONFIG, $_LIBLINK
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
#exit 0

#build pixman
cd $ROOTDIR/$PIXMAN
change_makefile_to_static_pixman $ROOTDIR/$PIXMAN/pixman/Makefile.win32
cd pixman
make -f Makefile.win32 "CFG=$_CONFIG" "MMX=off"

#source $WHEREAMI/vs90vcvars32.sh UNIX

#build cairo
cd $ROOTDIR/$CAIRO
change_makefile_to_static $ROOTDIR/$CAIRO/build/Makefile.win32.common
change_makefile_zlib $ROOTDIR/$CAIRO/build/Makefile.win32.common
missing_seperator_workaround $ROOTDIR/$CAIRO/src/Makefile.sources

make -f Makefile.win32 CFG=$_CONFIG

#deploy
cd $ROOTDIR
mkdir -p out/cairo
cp $CAIRO/cairo-version.h out/cairo/cairo-version.h
cp $CAIRO/src/$_CONFIG/cairo.lib out/cairo/cairo-$_LIBLINK-$_CONFIG.lib
cp $CAIRO/src/$_CONFIG/cairo-static.lib out/cairo/cairo_static-$_LIBLINK-$_CONFIG.lib
cp $CAIRO/src/$_CONFIG/cairo.dll out/cairo/cairo-$_LIBLINK-$_CONFIG.dll
cp $CAIRO/src/cairo-features.h out/cairo
cp $CAIRO/src/cairo.h out/cairo
cp $CAIRO/src/cairo-deprecated.h out/cairo
cp $CAIRO/src/cairo-win32.h out/cairo
cp $CAIRO/src/cairo-script.h out/cairo
cp $CAIRO/src/cairo-ps.h out/cairo
cp $CAIRO/src/cairo-pdf.h out/cairo
cp $CAIRO/src/cairo-svg.h out/cairo

mkdir -p out/zlib
cp $ZLIB/zlib.lib out/zlib/zlib-$_LIBLINK-$_CONFIG.lib
cp $ZLIB/*.h out/zlib

mkdir -p out/libpng
cp $LIBPNG/libpng.lib out/libpng/libpng-$_LIBLINK-$_CONFIG.lib
cp $LIBPNG/*.h out/libpng

mkdir -p out/pixman
cp $PIXMAN/pixman/$_CONFIG/pixman-1.lib out/pixman/pixman-1-$_LIBLINK-$_CONFIG.lib
cp $PIXMAN/pixman/*.h out/pixman

