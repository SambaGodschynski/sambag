# $Id$
# This module defines:
# TINYXML_LIBRARY_DIRS
# TINYXML_INCLUDE_DIRS

FIND_PATH(TINYXML_INCLUDE_DIRS ticpp/ticpp.h
	$ENV{CLIBS}/tinyxml
)

IF(WIN32)
	SET(LIBTINYXML "ticpp.lib")
ELSE(WIN32)
	SET(LIBTINYXML "libticpp.a")
ENDIF(WIN32)

FIND_PATH(TINYXML_LIBRARY_DIRS ${LIBTINYXML}
	$ENV{CLIBS}/tinyxml/lib
)

SET(TINYXML_FOUND "NO")


IF(TINYXML_LIBRARY_DIRS AND TINYXML_INCLUDE_DIRS)
	SET(TINYXML_FOUND "YES")
	message(STATUS "Detecting TINYXML: TinyXML found.")
else(TINYXML_LIBRARY_DIRS AND TINYXML_INCLUDE_DIRS)
	if(TinyXMLX_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested TINYXML libraries.")
	endif(TinyXMLX_FIND_REQUIRED)
ENDIF(TINYXML_LIBRARY_DIRS AND TINYXML_INCLUDE_DIRS)


