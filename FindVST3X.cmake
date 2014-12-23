# $Id$
# This module defines:
# VSTPLUGIN_INTERFACES_DIRS
# VSTSDK_INCLUDE_DIRS

FIND_PATH(VSTSDK3_INCLUDE_DIRS public.sdk/source/vst/vstaudioeffect.h
	$ENV{CLIBS}/VST3
)

SET(VST3_FOUND "NO")


IF(VSTSDK3_INCLUDE_DIRS)
	SET(VST3_FOUND "YES")
	message(STATUS "Detecting VST3: VST3 found.")
else(VSTSDK3_INCLUDE_DIRS)
	if(VST3_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested VST3 libraries.")
	endif(VST3_FIND_REQUIRED)
ENDIF(VSTSDK3_INCLUDE_DIRS)



