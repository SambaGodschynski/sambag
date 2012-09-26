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
SET(BOOST_ROOT $ENV{CLIBS}/boost)
find_package(Boost 1.49.0 REQUIRED filesystem system thread regex signals locale)

include_directories(${CAIRO_INCLUDE_DIRS})
include_directories(${CPPUNIT_INCLUDE_DIRS})
include_directories(${TINYXML_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LOKI_INCLUDE_DIRS})
include_directories(${LUA52_INCLUDE_DIRS})
include_directories(${SQLITE3_INCLUDE_DIRS})
#target_link_libraries (CmTut ${CAIRO_LIBRARIES})


set (SAMBAG_CLIBS ${SAMBAG_CLIBS}
	${Boost_LIBRARIES}
	${CPPUNIT_LIBRARY_DIRS}/${LIBCPPUNIT}
	${CAIRO_LIBRARY_DIRS}/${LIBCAIRO}
	${TINYXML_LIBRARY_DIRS}/${LIBTINYXML}
	${LUA52_LIBRARY_DIRS}/${LIBLUA52}
	${SQLITE3_LIBRARY_DIRS}/${LIBSQLITE3}
)

add_subdirectory(src)


