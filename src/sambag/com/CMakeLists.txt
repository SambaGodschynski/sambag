add_subdirectory(exceptions)
add_subdirectory(events)
add_subdirectory(tests)
SET (sambag_com
	Exception.cpp
	FileSystem.cpp
	FileHandler.cpp
	Common.cpp
)
add_library(sambag_com ${sambag_com})
