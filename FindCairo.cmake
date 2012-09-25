# $Id$
# This module defines:
# CAIRO_LIBRARY_DIRS
# CAIRO_INCLUDE_DIRS

FIND_PATH(CAIRO_INCLUDE_DIRS cairo.h
	$ENV{CAIRODIR}/include/cairo
	$ENV{CAIRODIR}/include
	$ENV{CAIRODIR}
	~/Library/Frameworks
	/Library/Frameworks
	/usr/include/cairo
	/usr/local/include/cairo
	/sw/include # Fink
	/opt/local/include # DarwinPorts
	/opt/csw/include # Blastwave
	/opt/include
)

IF(WIN32)
	SET(LIBCAIRO "cairo.lib")
ELSE(WIN32)
	SET(LIBCAIRO "libcairo.so")
ENDIF(WIN32)

FIND_PATH(CAIRO_LIBRARY_DIRS ${LIBCAIRO}
	$ENV{CAIRODIR}/lib
	$ENV{CAIRODIR}
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/lib
	/usr/lib
	/usr/lib/i386-linux-gnu
	/sw/lib
	/opt/local/lib
	/opt/csw/lib
	/opt/lib
	/usr/freeware/lib64
)

SET(CAIRO_FOUND "NO")


IF(CAIRO_LIBRARY_DIRS AND CAIRO_INCLUDE_DIRS)
	SET(CAIRO_FOUND "YES")
	message(STATUS "Detecting cairo: cairo found.")
else(CAIRO_LIBRARY_DIRS AND CAIRO_INCLUDE_DIRS)
	if(Cairo_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested Cairo libraries.")
	endif(Cairo_FIND_REQUIRED)
ENDIF(CAIRO_LIBRARY_DIRS AND CAIRO_INCLUDE_DIRS)



