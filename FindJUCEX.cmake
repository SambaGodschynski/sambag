# $Id$

FIND_PATH(JUCE_INCLUDE_DIRS modules/juce_audio_processors/juce_audio_processors.h
	$ENV{CLIBS}/JUCE
)


SET(JUCE_FOUND "NO")


IF(JUCE_INCLUDE_DIRS)
	SET(JUCE_FOUND "YES")
	message(STATUS "Detecting JUCE: JUCE found.")
else(JUCE_INCLUDE_DIRS)
	if(JUCE_FIND_REQUIRED)
      		message(SEND_ERROR "Unable to find the requested JUCE libraries.")
	endif(JUCE_FIND_REQUIRED)
ENDIF(JUCE_INCLUDE_DIRS)



