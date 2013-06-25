# $Id$

IF(WIN32)
        SET(BOOST_ROOT $ENV{CLIBS}/boost)
        SET(Boost_USE_STATIC_LIBS        ON)
        SET(Boost_USE_STATIC_RUNTIME    ON)
ELSE(WIN32)
        SET(BOOST_ROOT $ENV{CLIBS})
        SET(Boost_NO_SYSTEM_PATHS       ON)
        SET(Boost_USE_STATIC_LIBS       ON)
        SET(Boost_USE_MULTITHREADED     OFF)
        SET(Boost_USE_STATIC_RUNTIME    ON)
ENDIF(WIN32)

find_package(Boost 1.49.0 REQUIRED
        filesystem
        system
        thread
        regex
        signals 
        serialization
        date_time
        timer
        chrono 
        #log
        #log_setup
)

