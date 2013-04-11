/*
 * BasicLookAndFeel.cpp
 *
 *  Created on: Tue Jun  5 07:33:20 2012
 *      Author: Johannes Unger
 */

#include "BasicLookAndFeel.hpp"
#include <sambag/disco/svg/StyleParser.hpp>
// button
#include "BasicButtonUI.hpp"
#include "BasicMenuItemUI.hpp"
#include "BasicPopupMenuUI.hpp"
#include "BasicLabelUI.hpp"
#include "BasicMenuUI.hpp"
#include "BasicScrollbarUI.hpp"
#include "BasicPanelUI.hpp"
#include "BasicScrollPaneUI.hpp"
#include "BasicKnobUI.hpp"
#include "BasicListUI.hpp"
#include "BasicColumnViewUI.hpp"
#include "BasicCheckBoxUI.hpp"
#include "BasicArrowButtonUI.hpp"
#include "BasicArrowButton.hpp"
#include <sambag/disco/components/Panel.hpp>
#include <sambag/disco/components/Button.hpp>
#include <sambag/disco/components/MenuItem.hpp>
#include <sambag/disco/components/PopupMenu.hpp>
#include <sambag/disco/components/Label.hpp>
#include <sambag/disco/components/Menu.hpp>
#include <sambag/disco/components/Scrollbar.hpp>
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/components/ui/UIManager.hpp>
#include <sambag/disco/svg/HtmlColors.hpp>
#include <sambag/disco/components/Viewport.hpp>
#include <sambag/disco/components/ScrollPane.hpp>
#include <sambag/disco/components/Knob.hpp>
#include <sambag/disco/components/ColumnBrowser.hpp>
#include <sambag/disco/components/List.hpp>
#include <sambag/disco/components/ColumnView.hpp>
#include <sambag/disco/components/CheckBox.hpp>


namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
//  Class BasicLookAndFeel
//=============================================================================
//-----------------------------------------------------------------------------
BasicLookAndFeel::BasicLookAndFeel() {
}
//-----------------------------------------------------------------------------
void BasicLookAndFeel::installLookAndFeel() {
	installDefaults(); // <= always before installComponents
	installComponents();
}
//-----------------------------------------------------------------------------
void BasicLookAndFeel::installComponents() {
	registerComponentUI<Button, BasicButtonUI<Button::Model> >();
	registerComponentUI<MenuItem, BasicMenuItemUI<MenuItem::Model> >();
	registerComponentUI<PopupMenu, BasicPopupMenuUI<PopupMenu::Model> >();
	registerComponentUI<Label, BasicLabelUI>();
	registerComponentUI<Menu, BasicMenuUI<Menu::Model> >();
	registerComponentUI<Scrollbar, BasicScrollbarUI<Scrollbar::Model> >();
	registerComponentUI<Panel, BasicPanelUI >();
	registerComponentUI<Viewport, BasicPanelUI >();
	registerComponentUI<ScrollPane, BasicScrollPaneUI>();
	registerComponentUI<Knob, BasicKnobUI<Knob::Model> >();
	registerComponentUI<CheckBox, BasicCheckBoxUI<CheckBox::Model> >();
	registerComponentUI<BasicArrowButton, BasicArrowButtonUI<BasicArrowButton::Model> >();

	/*
	 * Unfortunately it is not
	 * possible to register a generic class with a LookAndFeel.
	 * So every specific template class type has to be registered manually.
	 */
	registerComponentUI<StringList, BasicListUI<StringList> >();
	typedef ColumnView<StringList> StringListColumn;
	registerComponentUI<StringListColumn, BasicColumnViewUI<StringListColumn> >();

	typedef ColumnBrowser<std::string> CBrowser;
	registerComponentUI<CBrowser::ColumnViewClass,
		BasicColumnViewUI<CBrowser::ColumnViewClass> >();
	registerComponentUI<CBrowser::ListType, BasicListUI<CBrowser::ListType> >();
}
//-----------------------------------------------------------------------------
svg::graphicElements::Style BasicLookAndFeel::createStyle(const std::string &str) 
{
	return ::createStyle(str);
} 
//-----------------------------------------------------------------------------
void BasicLookAndFeel::installDefaults() {
	using namespace sambag::disco;
	using namespace sambag::disco::svg;
	using namespace sambag::disco::svg::graphicElements;
	UIManager &m = getUIManager();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<install twice guard
	bool alreadyInstalled = false;
	m.getProperty("BasicLookAndFeel.installed", alreadyInstalled);
	if (alreadyInstalled) {
		return;
	}
	alreadyInstalled = true;
	m.putProperty("BasicLookAndFeel.installed", alreadyInstalled);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<global
	Style style = createStyle("stroke: black; fill: lavender;");
	//style.fillPattern( createPattern("lin(0,1,[D3D3D3FF:0, B0C4DEFF:1])") );
	m.putProperty("global.style", style);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Button
	using namespace svg;
	style = createStyle("stroke: black; fill: lavender;");
	style.fillPattern( createPattern("lin(0,1,[b7e6ffff:0, b7e6ffff:0.5, 92e8fdff:0.6, 92e8fdff:1])") );
	m.putProperty("Button.normal", style);
	
	style = createStyle("stroke: black; fill: lightblue;");
	style.fillPattern( createPattern("lin(0,1,[c4e4f6ff:0, c4e4f6ff:0.5, 9fd5f5ff:0.6, 9fd5f5ff:1])") );
	m.putProperty("Button.rollover",  style);

	style = createStyle("stroke: black; fill: slategrey;");
	style.fillPattern( createPattern("lin(0,1,[506a7aff:0, 506a7aff:0.5, 37515fff:0.6, 37515fff:1])") );
	m.putProperty("Button.pressed",  style);
	
	style = createStyle("stroke: darkgrey; fill: lightgrey;font-style: italic");
	m.putProperty("Button.disabled", style);

	m.putProperty("Button.cornerRadius", Coordinate(3.5));
	m.putProperty("Button.iconGap", Coordinate(2.));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<ScrollBar
	m.putProperty("ScrollBar.minimumThumbSize", Dimension(10., 10.));
	m.putProperty("ScrollBar.maximumThumbSize", Dimension(9999., 9999.));
	m.putProperty("ScrollBar.incrementButtonGap", Coordinate(5.));
	m.putProperty("ScrollBar.decrementButtonGap", Coordinate(5.));
	m.putProperty("ScrollBar.thumbHighlight", HtmlColors::getColor("lightblue"));
	m.putProperty("ScrollBar.thumbShadow", HtmlColors::getColor("darkgrey"));
	m.putProperty("ScrollBar.thumbDarkShadow", HtmlColors::getColor("black"));
	m.putProperty("ScrollBar.thumb", HtmlColors::getColor("grey"));
	m.putProperty("ScrollBar.track", HtmlColors::getColor("lightgrey"));
	m.putProperty("ScrollBar.trackHighlight", HtmlColors::getColor("lightblue"));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<list
	m.putProperty("List.minCellWidth", Coordinate(60.));
	m.putProperty("List.minCellHeight", Coordinate(20.));
	style = createStyle("stroke: black; fill: lavender;");
	//style.fillPattern( createPattern("lin(1,0,[f0ffffff:0, D0D0D0ff:0.3, D0D0D0ff:0.6, f0ffffff:1])") );
	m.putProperty("List.style", style);
	style = createStyle("stroke: white; fill: blue;");
	style.fillColor( style.fillPattern()->getColor().setA(0.5) );
	m.putProperty("List.selectionStyle", style);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<knobs
	m.putProperty("Knob.range", (Coordinate)200.);
	m.putProperty("Knob.mode", std::string("circular"));
	m.putProperty("Knob.size", Dimension(45., 45.));
	m.putProperty("Knob.strokeColor", HtmlColors::getColor("black"));
	m.putProperty("Knob.fillColor", HtmlColors::getColor("white"));
	m.putProperty("Knob.colorHandler", HtmlColors::getColor("grey"));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<ColumnView
	m.putProperty("ColumnView.background", HtmlColors::getColor("lightblue"));
	m.putProperty("ColumnView.foreground", HtmlColors::getColor("black"));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<ColumnBrowser
	m.putProperty("ColumnBrowser.numInitLists", (int)4);
	m.putProperty("ColumnBrowser.fontStyle", createStyle("font-size: 16; font-family: arial; font-style:normal"));
	m.putProperty("ColumnBrowser.fixedColumnWidth", (Coordinate)230.);
	m.putProperty("ColumnBrowser.scrollanimation.duration", (long)350);
	m.putProperty("ColumnBrowser.scrollanimation.refreshRate", (long)30);
	m.putProperty("ColumnBrowser.scrollanimation.tweenType", std::string("quad"));
}

}}}}} // namespace(s)
