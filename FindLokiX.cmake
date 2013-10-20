# $Id$
# This module defines:
# LOKI_LIBRARY_DIRS
# LOKI_INCLUDE_DIRS

FIND_PATH(LOKI_INCLUDE_DIRS loki/Typelist.h
	$ENV{CLIBS}/loki/include
	/usr/include/
)

SET(LOKI_FOUND "NO")


IF(LOKI_INCLUDE_DIRS)
	SET(LOKI_FOUND "YES")
	message(STATUS "Detecting LOKI: LOKI found.")
else(LOKI_INCLUDE_DIRS)
	if(LokiX_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested LOKI libraries.")
	endif(LokiX_FIND_REQUIRED)
ENDIF(LOKI_INCLUDE_DIRS)



