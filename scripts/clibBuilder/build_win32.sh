#!/bin/sh
IFS=$'\n'

export ROOTDIR='build'
export URL_ZLIB='http://zlib.net/zlib-1.2.7.tar.gz'
export URL_LIBPNG='ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng-1.0.60.tar.gz'

#passing arguments

case $@ in
     CLEAN_ALL) export CLEAN_ALL=1
                rm -rf $ROOTDIR ;;
esac


mkdir -p $ROOTDIR
#                     $URL      $DST_FILE   $DST_DIRECTORY
sh downloadpackage.sh $URL_ZLIB zlib.tar.gz zlib
#sh downloadpackage.sh $URL_LIBPNG libpng.tar.gz libpng

#build zlib
export DST_ZLIB=$ROOTDIR/zlib
cmd /c makeZlib.bat
