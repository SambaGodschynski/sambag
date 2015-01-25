# $Id$
# This module defines:
# VSTPLUGIN_INTERFACES_DIRS
# VSTSDK_INCLUDE_DIRS

FIND_PATH(VSTSDK3_INCLUDE_DIRS public.sdk/source/vst/vstaudioeffect.h
	$ENV{CLIBS}/VST3
)


IF(UNIX)
        SET(LIBVST3  "libbase_libc++.a")
        SET(LIBVST3D "libbase_libc++.a")
ELSE(WIN32)
        SET(LIBVST3 "base_libc++.lib")
        SET(LIBVST3D "base_libcd++.lib")
ENDIF(UNIX)

FIND_PATH(VST3_LIBRARY_DIRS ${LIBVST3}
        $ENV{CLIBS}/VST3/lib
)




SET(VST3_FOUND "NO")


IF(VSTSDK3_INCLUDE_DIRS)
	SET(VST3_FOUND "YES")
	SET(VST3_LIBRARIES ${VST3_LIBRARIES} optimized ${VST3_LIBRARY_DIRS}/${LIBVST3})
        SET(VST3_LIBRARIES ${VST3_LIBRARIES} debug ${VST3_LIBRARY_DIRS}/${LIBVST3D})
	message(STATUS "Detecting VST3: VST3 found.")
else(VSTSDK3_INCLUDE_DIRS)
	if(VST3_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested VST3 libraries.")
	endif(VST3_FIND_REQUIRED)
ENDIF(VSTSDK3_INCLUDE_DIRS)



