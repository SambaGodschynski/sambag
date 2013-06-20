# $Id$
# This module defines:
# SAMBAG_LIBRARY_DIRS
# SAMBAG_INCLUDE_DIRS

FIND_PATH(SAMBAG_INCLUDE_DIRS sambag/disco/components/Forward.hpp
	$ENV{CLIBS}/sambag/src
)

	
IF(APPLE)
	SET(LIBSAMBAG "libsambag.a")
	SET(LIBSAMBAGD "libsambag.a")
ELSEIF(UNIX)
	SET(LIBSAMBAG "libsambag.a")
	SET(LIBSAMBAGD "libsambag.a")
ELSEIF(WIN32)
	SET(LIBSAMBAG "sambag.lib")
	SET(LIBSAMBAGD "sambag.lib")
ENDIF(APPLE)



IF (WIN32)
	FIND_PATH(SAMBAG_LIBRARY_DIRS Debug/${LIBSAMBAG}
		$ENV{CLIBS}/sambag/src
	)
ELSE(WIN32)
	FIND_PATH(SAMBAG_LIBRARY_DIRS ${LIBSAMBAG}
		$ENV{CLIBS}/sambag/src
		$ENV{CLIBS}/sambag/src/Debug
		$ENV{CLIBS}/sambag/src/Release
		$ENV{CLIBS}/sambag/lib	 
	)
ENDIF (WIN32)

SET(SAMBAG_FOUND "NO")


IF(SAMBAG_LIBRARY_DIRS AND SAMBAG_INCLUDE_DIRS)
	SET(SAMBAG_FOUND "YES")
	IF (WIN32)
		SET(SAMBAG_LIBRARIES ${SAMBAG_LIBRARIES} optimized ${SAMBAG_LIBRARY_DIRS}/Release/${LIBSAMBAG})
		SET(SAMBAG_LIBRARIES ${SAMBAG_LIBRARIES} debug ${SAMBAG_LIBRARY_DIRS}/Debug/${LIBSAMBAGD})
	ELSE(WIN32)
		SET(SAMBAG_LIBRARIES ${SAMBAG_LIBRARIES} ${SAMBAG_LIBRARY_DIRS}/${LIBSAMBAG})
	ENDIF (WIN32)
	message(STATUS "Detecting SAMBAG: SAMBAG found.")
else(SAMBAG_LIBRARY_DIRS AND SAMBAG_INCLUDE_DIRS)
	#if(SAMBAG_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested SAMBAG libraries.")
	#endif(SAMBAG_FIND_REQUIRED)
ENDIF(SAMBAG_LIBRARY_DIRS AND SAMBAG_INCLUDE_DIRS)



