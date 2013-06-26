# $Id$
# This module defines:
# LIBZLIB_LIBRARY_DIRS
# LIBZLIB_INCLUDE_DIRS

FIND_PATH(LIBZLIB_INCLUDE_DIRS zlib.h
	$ENV{CLIBS}/zlib
	/opt/local/include
)

IF(WIN32)
	SET(LIBZLIB "zlib-MT-release.lib")
	SET(LIBZLIBD "zlib-MT-debug.lib")
ELSE(WIN32)
	SET(LIBZLIB "libz.a")
	SET(LIBZLIBD "libz.a")
ENDIF(WIN32)

FIND_PATH(LIBZLIB_LIBRARY_DIRS ${LIBZLIB}
	$ENV{CLIBS}/zlib
	/opt/local/lib
)

SET(LIBZLIB_FOUND "NO")


IF(LIBZLIB_LIBRARY_DIRS AND LIBZLIB_INCLUDE_DIRS)
	SET(LIBZLIB_FOUND "YES")
	message(STATUS "Detecting LIBZLIB: LIBZLIB found.")
	#SET(LIBZLIB_LIBRARIES ... see FindCairo.cmake
	#SET(LIBZLIB_LIBRARIES  optimized ${LIBZLIB_LIBRARY_DIRS}/${LIBZLIB})
	#SET(LIBZLIB_LIBRARIES ${LIBZLIB_LIBRARIES} debug ${LIBZLIB_LIBRARY_DIRS}/${LIBZLIBD})
else(LIBZLIB_LIBRARY_DIRS AND LIBZLIB_INCLUDE_DIRS)
	message(SEND_ERROR "Unable to find the requested LIBZLIB libraries.")
ENDIF(LIBZLIB_LIBRARY_DIRS AND LIBZLIB_INCLUDE_DIRS)



