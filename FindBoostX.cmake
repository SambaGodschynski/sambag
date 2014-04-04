# $Id$

IF(WIN32)
        SET(BOOST_ROOT $ENV{CLIBS}/boost)
        SET(Boost_USE_STATIC_LIBS        ON)
        SET(Boost_USE_STATIC_RUNTIME    ON)
	SET(xboost_libs ${xboost_libs} "locale")
ELSEIF(APPLE)
        #SET(BOOST_ROOT $ENV{CLIBS})
        #SET(Boost_NO_SYSTEM_PATHS       ON)
        SET(Boost_USE_STATIC_LIBS       ON)
        SET(Boost_USE_MULTITHREADED     ON)
        #SET(Boost_USE_STATIC_RUNTIME    ON)
ELSEIF(UNIX)
	SET(BOOST_ROOT $ENV{CLIBS}/boost)
        SET(Boost_NO_SYSTEM_PATHS       ON)
        SET(Boost_USE_STATIC_LIBS       ON)
        SET(Boost_USE_MULTITHREADED     ON)
        #SET(Boost_USE_STATIC_RUNTIME    ON)

ENDIF(WIN32)


IF($ENV{SAMBA_USELOG})
  find_package(Boost 1.49.0 REQUIRED
  	${xboost_libs}
        filesystem
        system
        thread
        regex
        signals 
        serialization
        date_time
        timer
        chrono
	program_options        
	log
        log_setup
  )
ELSE($ENV{SAMBA_USELOG})
  find_package(Boost 1.49.0 REQUIRED
        ${xboost_libs}
        filesystem
        system
        thread
        regex
        signals
        serialization
        date_time
        timer
        chrono
        program_options
  )
ENDIF($ENV{SAMBA_USELOG})
