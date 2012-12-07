# $Id$
# This module defines:
# VSTPLUGIN_INTERFACES_DIRS
# VSTSDK_INCLUDE_DIRS

FIND_PATH(VSTSDK_INCLUDE_DIRS public.sdk/source/vst2.x/audioeffect.h
	$ENV{CLIBS}/vstsdk/
	$ENV{CLIBS}/vstsdk2.4/
)

SET(VST23_FOUND "NO")


IF(VSTSDK_INCLUDE_DIRS)
	SET(VST23_FOUND "YES")
	message(STATUS "Detecting VST23: VST23 found.")
else(VSTSDK_INCLUDE_DIRS)
	if(VST23X_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested VST23 libraries.")
	endif(VST23X_FIND_REQUIRED)
ENDIF(VSTSDK_INCLUDE_DIRS)



