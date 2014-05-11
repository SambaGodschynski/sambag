# $Id$
# This module defines:
# LUA52_LIBRARY_DIRS
# LUA52_INCLUDE_DIRS

FIND_PATH(LUA52_INCLUDE_DIRS lua.hpp
	$ENV{CLIBS}/lua/src
	/opt/local/include
	/usr/include/lua5.2
)

IF(WIN32)
	SET(LIBLUA52 "lua52-s-mt.lib")
	SET(LIBLUA52D "lua52-s-mt-d.lib")
ELSE(WIN32)
	SET(LIBLUA52 "liblua.a")
        SET(LIBLUA52D "liblua.a")
ENDIF(WIN32)

FIND_PATH(LUA52_LIBRARY_DIRS ${LIBLUA52}
	$ENV{CLIBS}/lua/src
	$ENV{CLIBS}/lua/win32/lib
	/opt/local/lib/
	/usr/lib
	/usr/lib/powerpc-linux-gnu/
	/usr/lib/i386-linux-gnu
	/usr/lib/x86_64-linux-gnu
)

SET(LUA52_FOUND "NO")


IF(LUA52_LIBRARY_DIRS AND LUA52_INCLUDE_DIRS)
	SET(LUA52_FOUND "YES")
	SET(LUA52_LIBRARIES ${LUA52_LIBRARIES} optimized ${LUA52_LIBRARY_DIRS}/${LIBLUA52})
	SET(LUA52_LIBRARIES ${LUA52_LIBRARIES} debug ${LUA52_LIBRARY_DIRS}/${LIBLUA52D})
	message(STATUS "Detecting LUA52: LUA52 found.")
else(LUA52_LIBRARY_DIRS AND LUA52_INCLUDE_DIRS)
	#if(LUA52X_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested LUA52 libraries.")
	#endif(LUA52X_FIND_REQUIRED)
ENDIF(LUA52_LIBRARY_DIRS AND LUA52_INCLUDE_DIRS)



