add_subdirectory(components)
add_subdirectory(tests)
add_subdirectory(svg)
SET (sambag_disco
	IPattern.cpp
	IDiscoFactory.cpp
	Coordinate.cpp
	CairoDiscoFactory.cpp
	Geometry.cpp
	CairoDrawContext.cpp
	CairoPattern.cpp
	ColorRGBA.cpp
)
add_library(sambag_disco ${sambag_disco})
