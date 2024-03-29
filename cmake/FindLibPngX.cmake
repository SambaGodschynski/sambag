# $Id$
# This module defines:
# LIBPNG_LIBRARY_DIRS
# LIBPNG_INCLUDE_DIRS

FIND_PATH(LIBPNG_INCLUDE_DIRS png.h
	$ENV{CLIBS}/libpng
	/opt/local/include
	$ENV{CLIBS}/include
)

IF(WIN32)
	SET(LIBPNG "libpng-MT-release.lib")
	SET(LIBPNGD "libpng-MT-debug.lib")
ELSE(WIN32)
	SET(LIBPNG "libpng.a")
	SET(LIBPNGD "libpng.a")
ENDIF(WIN32)

FIND_PATH(LIBPNG_LIBRARY_DIRS ${LIBPNG}
	$ENV{CLIBS}/libpng
	/opt/local/lib
	$ENV{CLIBS}/lib
)

SET(LIBPNG_FOUND "NO")

IF(LIBPNG_LIBRARY_DIRS AND LIBPNG_INCLUDE_DIRS)
	SET(LIBPNG_FOUND "YES")
	message(STATUS "Detecting LIBPNG: LIBPNG found.")
	#SET(LIBPNG_LIBRARIES ... see FindCairo.cmake
	#SET(LIBPNG_LIBRARIES ${LIBPNG_LIBRARIES} optimized ${LIBPNG_LIBRARY_DIRS}/${LIBPNG})
	#SET(LIBPNG_LIBRARIES ${LIBPNG_LIBRARIES} debug ${LIBPNG_LIBRARY_DIRS}/${LIBPNGD})
else(LIBPNG_LIBRARY_DIRS AND LIBPNG_INCLUDE_DIRS)
	message(SEND_ERROR "Unable to find the requested LIBPNG libraries.")
ENDIF(LIBPNG_LIBRARY_DIRS AND LIBPNG_INCLUDE_DIRS)



