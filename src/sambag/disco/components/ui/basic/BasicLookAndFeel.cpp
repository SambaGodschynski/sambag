/*
 * BasicLookAndFeel.cpp
 *
 *  Created on: Tue Jun  5 07:33:20 2012
 *      Author: Johannes Unger
 */

#include "BasicLookAndFeel.hpp"
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


namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
//  Class BasicLookAndFeel
//=============================================================================
//-----------------------------------------------------------------------------
BasicLookAndFeel::BasicLookAndFeel() {
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
void BasicLookAndFeel::installDefaults() {
	using namespace sambag::disco;
	using namespace sambag::disco::svg;
	UIManager &m = getUIManager();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<global
	m.putProperty("global.background", HtmlColors::getColor("red"));
	m.putProperty("global.foreground", HtmlColors::getColor("white"));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<ScrollBar
	m.putProperty("ScrollBar.minimumThumbSize", Dimension(15., 15.));
	m.putProperty("ScrollBar.maximumThumbSize", Dimension(20., 20.));
	m.putProperty("ScrollBar.incrementButtonGap", Coordinate(5.));
	m.putProperty("ScrollBar.decrementButtonGap", Coordinate(5.));
	m.putProperty("ScrollBar.thumbHighlight", HtmlColors::getColor("lightblue"));
	m.putProperty("ScrollBar.thumbShadow", HtmlColors::getColor("darkgrey"));
	m.putProperty("ScrollBar.thumbDarkShadow", HtmlColors::getColor("black"));
	m.putProperty("ScrollBar.thumb", HtmlColors::getColor("grey"));
	m.putProperty("ScrollBar.track", HtmlColors::getColor("lightgrey"));
	m.putProperty("ScrollBar.trackHighlight", HtmlColors::getColor("lightblue"));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<StringList
	m.putProperty("StringList.background", HtmlColors::getColor("azure"));
	m.putProperty("StringList.foreground", HtmlColors::getColor("black"));
	m.putProperty("StringList.selectionBackground", HtmlColors::getColor("blue"));
	m.putProperty("StringList.selectionForeground", HtmlColors::getColor("white"));
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
	m.putProperty("ColumnView.fixedColumnWidth", (Coordinate)120.);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<ColumnBrowser
	m.putProperty("ColumnBrowser.numInitLists", (int)4);
	m.putProperty("ColumnBrowser.fontSize", Coordinate(13.));

}

}}}}} // namespace(s)
