cmake_minimum_required (VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "./")

project (sambag)

include_directories ("${PROJECT_SOURCE_DIR}/src")
add_definitions(-DBOOST_UBLAS_TYPE_CHECK=0)
add_definitions(-DBOOST_FILESYSTEM_VERSION=3)

#to avoid name conflicts with existing FindSomeLib.cmake scripts
#every finding script has a "X" postfix: FindSomeLibX.cmake
#(concerns scriptname only)

#UNIX is TRUE on all UNIX-like OS's, including Apple OS X and Cygwin.
#WIN32 is TRUE on Windows, including Cygwin.
#APPLE is TRUE on Apple systems.

IF(APPLE)
ELSEIF(UNIX)
	find_package(X11 REQUIRED)
	include_directories(${X11_INCLUDE_DIRS})
ELSEIF(WIN32)
ENDIF(APPLE)

find_package(Cairo REQUIRED)
find_package(CppUnitX REQUIRED)
find_package(TinyXMLX REQUIRED)
find_package(LokiX REQUIRED)
find_package(Lua52X REQUIRED)
find_package(Sqlite3X REQUIRED)
#sqlite
#lua
#x11
#win32
#SET(Boost_DIR $ENV{CLIBS}/boost)
find_package(Boost 1.49.0 REQUIRED)
include_directories(${CAIRO_INCLUDE_DIRS})
include_directories(${CPPUNIT_INCLUDE_DIRS})
include_directories(${TINYXML_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LOKI_INCLUDE_DIRS})
include_directories(${LUA52_INCLUDE_DIRS})
include_directories(${SQLITE3_INCLUDE_DIRS})
#target_link_libraries (CmTut ${CAIRO_LIBRARIES})

set (UNITTEST_LIBS ${EXTRA_LIBS} 
	sambag_com
	sambag_com_tests
	sambag_com_events
	sambag_com_exceptions
)

add_subdirectory(src)
add_subdirectory(testApps)
