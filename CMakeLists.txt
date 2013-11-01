cmake_minimum_required (VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "./")

project (sambag)

execute_process(COMMAND git log --pretty=format:%h -n 1
                OUTPUT_VARIABLE SAMBAG_VERSION_BUILD)


include_directories ("${PROJECT_SOURCE_DIR}/src")
add_definitions(-DBOOST_UBLAS_TYPE_CHECK=0)
add_definitions(-DBOOST_FILESYSTEM_VERSION=3)
#add_definitions(-DSAMBAG_USE_LOG)
add_definitions(-D_SCL_SECURE_NO_WARNINGS) #disable warning C4996: 'std::uninitialized_copy' caused by boost::ublas
add_definitions(-D_CRT_SECURE_NO_WARNINGS) #disable strcpy warning in audioeffect.h

#add_definitions(-DSAMBA_WARN_MISSING_IMPL) #show xy not impl. warning

#to avoid name conflicts with existing FindSomeLib.cmake scripts
#every finding script has a "X" postfix: FindSomeLibX.cmake
#(concerns scriptname only)

#UNIX is TRUE on all UNIX-like OS's, including Apple OS X and Cygwin.
#WIN32 is TRUE on Windows, including Cygwin.
#APPLE is TRUE on Apple systems.

IF(APPLE)
	#find_package(X11 REQUIRED)
        #include_directories(${X11_INCLUDE_DIRS})
	#find_package(IConvX REQUIRED)
	add_definitions(-DDISCO_USE_COCOA)
	FIND_LIBRARY(COCOA_LIBRARY Cocoa)
	FIND_LIBRARY(CARBON_LIBRARY Carbon)
	set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvmgcc42")
ELSEIF(UNIX)
	find_package(X11 REQUIRED)
	include_directories(${X11_INCLUDE_DIRS})
	add_definitions(-DDISCO_USE_X11)
	add_definitions(-D__cdecl=__attribute__\(\(__cdecl__\)\)) #aeffect.h: unarm __cdecl
	SET (CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS_INIT} "-Wl,--no-as-needed -lrt -lpthread"
             CACHE STRING "Flags used by the linker during the creation of modules.")
ELSEIF(WIN32)
        add_definitions(-DDISCO_USE_WIN32)
	add_definitions(-DWIN32_LEAN_AND_MEAN)
	add_definitions(-DSAMBAG_USE_WINDOWS_SHARED_MEMORY)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /ZI")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
ENDIF(APPLE)

find_package(Cairo REQUIRED)
find_package(CppUnitX REQUIRED)
find_package(TinyXMLX REQUIRED)
find_package(LokiX REQUIRED)
find_package(Lua52X REQUIRED)
find_package(Sqlite3X REQUIRED)
find_package(VST23X REQUIRED)
find_package(BoostX REQUIRED)

include_directories(${CAIRO_INCLUDE_DIRS})
include_directories(${CPPUNIT_INCLUDE_DIRS})
include_directories(${TINYXML_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LOKI_INCLUDE_DIRS})
include_directories(${LUA52_INCLUDE_DIRS})
include_directories(${SQLITE3_INCLUDE_DIRS})

include_directories(${VSTSDK_INCLUDE_DIRS})
include_directories(${VSTSDK_INCLUDE_DIRS}/public.sdk/source/vst2.x)
include_directories(${VSTSDK_INCLUDE_DIRS}/pluginterfaces/vst2.x)

IF(APPLE)
	set (SAMBAG_CLIBS ${SAMBAG_CLIBS}
	    	${COCOA_LIBRARY}
		${CARBON_LIBRARY}
	)
ELSEIF(UNIX)
	set (SAMBAG_CLIBS ${SAMBAG_CLIBS}
		${X11_LIBRARIES}
	)
ELSEIF(WIN32)
ENDIF(APPLE)


set (SAMBAG_CLIBS ${SAMBAG_CLIBS}
	${Boost_LIBRARIES}
	${CPPUNIT_LIBRARIES}
	${CAIRO_LIBRARIES}
	${TINYXML_LIBRARIES}
	${LUA52_LIBRARIES}
)

CONFIGURE_FILE(
  "${PROJECT_SOURCE_DIR}/src/sambag/com/Config.h.in"
  "${PROJECT_BINARY_DIR}/src/sambag/com/Config.h"
)

add_subdirectory(src)
