# $Id$
# This module defines:
# CAIRO_LIBRARY_DIRS
# CAIRO_INCLUDE_DIRS

FIND_PATH(CAIRO_INCLUDE_DIRS cairo.h
	$ENV{CAIRODIR}/include/cairo
	$ENV{CAIRODIR}/include
	$ENV{CAIRODIR}
	$ENV{CLIBS}/cairo/src
	$ENV{CLIBS}/cairo/include/cairo
	~/Library/Frameworks
	/Library/Frameworks
	/usr/include/cairo
	/usr/local/include/cairo
	/sw/include # Fink
	/opt/local/include # DarwinPorts
	/opt/csw/include # Blastwave
	/opt/include
)

IF(APPLE)
	SET(LIBCAIRO "libcairo.dylib")
	SET(LIBCAIROD "libcairo.dylib")
ELSEIF(UNIX)
	SET(LIBCAIRO "libcairo.so")
	SET(LIBCAIROD "libcairo.so")
ELSEIF(WIN32)
        SET(LIBCAIRO "cairo-static.lib")
        SET(LIBCAIROD "cairo.lib")
ENDIF(APPLE)

FIND_PATH(CAIRO_LIBRARY_DIRS ${LIBCAIRO}
	$ENV{CAIRODIR}/lib
	$ENV{CAIRODIR}
	$ENV{CLIBS}/cairo/lib
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

IF(WIN32)
         find_package(LibPngX REQUIRED)
         find_package(ZlibX REQUIRED)
         find_package(PixmanX REQUIRED)
         SET(CAIRO_LIBRARIES ${CAIRO_LIBRARIES} optimized ${LIBPNG_LIBRARY_DIRS}/${LIBPNG})
         SET(CAIRO_LIBRARIES ${CAIRO_LIBRARIES} optimized ${LIBZLIB_LIBRARY_DIRS}/${LIBZLIB})
         SET(CAIRO_LIBRARIES ${CAIRO_LIBRARIES} optimized ${LIBPIXMAN_LIBRARY_DIRS}/${LIBPIXMAN})
ENDIF(WIN32)

IF(CAIRO_LIBRARY_DIRS AND CAIRO_INCLUDE_DIRS)
	SET(CAIRO_FOUND "YES")
	SET(CAIRO_LIBRARIES ${CAIRO_LIBRARIES} optimized ${CAIRO_LIBRARY_DIRS}/${LIBCAIRO})
        SET(CAIRO_LIBRARIES ${CAIRO_LIBRARIES} debug ${CAIRO_LIBRARY_DIRS}/${LIBCAIROD})
	message(STATUS "Detecting cairo: cairo found.")
else(CAIRO_LIBRARY_DIRS AND CAIRO_INCLUDE_DIRS)
	if(Cairo_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested Cairo libraries.")
	endif(Cairo_FIND_REQUIRED)
ENDIF(CAIRO_LIBRARY_DIRS AND CAIRO_INCLUDE_DIRS)



