# $Id$
# This module defines:
# LIBPIXMAN_LIBRARY_DIRS
# LIBPIXMAN_INCLUDE_DIRS

FIND_PATH(LIBPIXMAN_INCLUDE_DIRS pixman.h
	$ENV{CLIBS}/pixman
)

IF(WIN32)
	SET(LIBPIXMAN "pixman-1-MT-release.lib")
	SET(LIBPIXMAND "pixman-1-MT-debug.lib")
ELSE(WIN32)
	SET(LIBPIXMAN "libpixman.a")
	SET(LIBPIXMAND "libpixman.a")
ENDIF(WIN32)

FIND_PATH(LIBPIXMAN_LIBRARY_DIRS ${LIBPIXMAN}
	$ENV{CLIBS}/pixman
)

SET(LIBPIXMAN_FOUND "NO")


IF(LIBPIXMAN_LIBRARY_DIRS AND LIBPIXMAN_INCLUDE_DIRS)
	SET(LIBPIXMAN_FOUND "YES")
	message(STATUS "Detecting LIBPIXMAN: LIBPIXMAN found.")
	#SET(LIBPIXMAN_LIBRARIES ... see FindCairo.cmake
	#SET(LIBPIXMAN_LIBRARIES ${LIBPIXMAN_LIBRARIES} optimized ${LIBPIXMAN_LIBRARY_DIRS}/${LIBPIXMAN})
	#SET(LIBPIXMAN_LIBRARIES ${LIBPIXMAN_LIBRARIES} debug ${LIBPIXMAN_LIBRARY_DIRS}/${LIBPIXMAND})
else(LIBPIXMAN_LIBRARY_DIRS AND LIBPIXMAN_INCLUDE_DIRS)
	if(LIBPIXMANX_FIND_REQUIRED)
     		message(SEND_ERROR "Unable to find the requested LIBPIXMAN libraries.")
	endif(LIBPIXMANX_FIND_REQUIRED)
ENDIF(LIBPIXMAN_LIBRARY_DIRS AND LIBPIXMAN_INCLUDE_DIRS)



