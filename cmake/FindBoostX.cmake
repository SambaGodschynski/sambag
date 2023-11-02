# $Id$

IF(WIN32)
        SET(Boost_USE_STATIC_LIBS        ON)
        SET(Boost_USE_STATIC_RUNTIME    ON)
	SET(xboost_libs ${xboost_libs} "locale")
ELSEIF(APPLE)
        SET(Boost_USE_STATIC_LIBS       ON)
        SET(Boost_USE_MULTITHREADED     ON)
ELSEIF(UNIX)
        SET(Boost_NO_SYSTEM_PATHS       OFF)
        SET(Boost_USE_STATIC_LIBS       ON)
        SET(Boost_USE_MULTITHREADED     ON)

ENDIF(WIN32)


IF($ENV{SAMBA_USELOG})
  find_package(Boost 1.55.0 REQUIRED
  	${xboost_libs}
        filesystem
        system
        thread
        regex
        serialization
        date_time
        timer
        chrono
	program_options        
	log
        log_setup
  )
ELSE($ENV{SAMBA_USELOG})
  find_package(Boost 1.55.0 REQUIRED
        ${xboost_libs}
        filesystem
        system
        thread
        regex
        serialization
        date_time
        timer
        chrono
        program_options
  )
ENDIF($ENV{SAMBA_USELOG})
