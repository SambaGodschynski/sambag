# $Id$
# This module defines:
# CPPUNIT_LIBRARY_DIRS
# CPPUNIT_INCLUDE_DIRS

FIND_PATH(CPPUNIT_INCLUDE_TMP Test.h
	$ENV{CLIBS}/cppunit/include/cppunit
)

SET(CPPUNIT_INCLUDE_DIRS "${CPPUNIT_INCLUDE_TMP}/..")

IF(WIN32)
	SET(LIBCPPUNIT "cppunit.lib")
ELSE(WIN32)
	SET(LIBCPPUNIT "libcppunit.a")
ENDIF(WIN32)

FIND_PATH(CPPUNIT_LIBRARY_DIRS ${LIBCPPUNIT}
	$ENV{CLIBS}/cppunit/lib/lib
	$ENV{CLIBS}/cppunit/bin/lib
	$ENV{CLIBS}/cppunit/lib
)

SET(CPPUNIT_FOUND "NO")


IF(CPPUNIT_LIBRARY_DIRS AND CPPUNIT_INCLUDE_DIRS)
	SET(CPPUNIT_FOUND "YES")
	message(STATUS "Detecting CPPUNIT: Cppunit found.")
else(CPPUNIT_LIBRARY_DIRS AND CPPUNIT_INCLUDE_DIRS)
	if(CppUnitX_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested CPPUNIT libraries.")
	endif(CppUnitX_FIND_REQUIRED)
ENDIF(CPPUNIT_LIBRARY_DIRS AND CPPUNIT_INCLUDE_DIRS)


