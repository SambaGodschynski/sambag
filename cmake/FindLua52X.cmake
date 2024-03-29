# $Id$
# This module defines:
# LUA52_LIBRARY_DIRS
# LUA52_INCLUDE_DIRS

FIND_PATH(LUA52_INCLUDE_DIRS lua.hpp
	$ENV{CLIBS}/lua/src
	/opt/local/include
	/usr/include/lua5.2
)

SET(LUA52_FOUND "NO")


IF(LUA52_INCLUDE_DIRS)
	SET(LUA52_FOUND "YES")
	message(STATUS "Detecting LUA52: LUA52 found.")
else(LUA52_LIBRARY_DIRS AND LUA52_INCLUDE_DIRS)
	#if(LUA52X_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested LUA52 libraries.")
	#endif(LUA52X_FIND_REQUIRED)
ENDIF(LUA52_LIBRARY_DIRS AND LUA52_INCLUDE_DIRS)



