add_subdirectory(units)
add_subdirectory(graphicElements)
SET (sambag_disco_svg
	AttributeParser.cpp
	Svg.cpp
	SvgUse.cpp
	SvgLine.cpp
	SvgCompound.cpp
	SvgLinearGradient.cpp
	HtmlColors.cpp
	SvgStyle.cpp
	SvgRadialGradient.cpp
	SvgText.cpp
	SvgEllipse.cpp
	StyleAttributeSetters.cpp
	StyleParser.cpp
	SvgPath.cpp
	SvgBuilder.cpp
	PathInstructions.cpp
	SvgGroup.cpp
	SvgDefs.cpp
	SvgCircle.cpp
	SvgRoot.cpp
)
add_library(sambag_disco_svg ${sambag_disco_svg})
