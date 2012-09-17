add_subdirectory(basic)
SET (sambag_disco_components_ui
	DefaultMenuLayout.cpp
	AComponentUI.cpp
	UIManager.cpp
	ALookAndFeel.cpp
)
add_library(sambag_disco_components_ui ${sambag_disco_components_ui})
