/*
 * ScrollPane.cpp
 *
 *  Created on: Thu Jul  5 16:50:53 2012
 *      Author: Johannes Unger
 */

#include "ScrollPane.hpp"
#include "ui/ALookAndFeel.hpp"
#include "Viewport.hpp"
#include "ScrollPaneLayout.hpp"


namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class ScrollPane::MyScrollbar
//=============================================================================
class ScrollPane::MyScrollbar : public Scrollbar {
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ScrollPane::MyScrollbar> Ptr;
	//-------------------------------------------------------------------------
	ScrollPane &parent;
	//-------------------------------------------------------------------------
	MyScrollbar(ScrollPane &parent, Scrollbar::Orientation o) :
		Scrollbar(o), parent(parent) {}
public:
	//-------------------------------------------------------------------------
	static Ptr create(ScrollPane &parent, Scrollbar::Orientation o) {
		Ptr res(new MyScrollbar(parent, o));
		res->self = res;
		res->constructorAlt();
		return res;
	}
};
//=============================================================================
//  Class ScrollPane
//=============================================================================
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_VERTICAL_SCROLLBAR_POLICY =
		"vertical scrollbar policy";
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_HORIZONTAL_SCROLLBAR_POLICY =
		"horizontal scrollbar policy";
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_VERTICAL_SCROLLBAR =
		"vertical scrollbar";
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_HORIZONTAL_SCROLLBAR =
		"horizontal scrollbar";
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_VIEWPORT = "viewport";
//-----------------------------------------------------------------------------
ScrollPane::ScrollPane::ScrollPane() :
	verticalScrollBarPolicy(VERTICAL_SCROLLBAR_AS_NEEDED),
	horizontalScrollBarPolicy(HORIZONTAL_SCROLLBAR_AS_NEEDED)
{
}
//-----------------------------------------------------------------------------
ScrollPane::ScrollPane::ScrollPane(AComponentPtr view,
		ScrollPane::VerticalScrollbarPolicy vsbPolicy,
		ScrollPane::HorizontalScrollbarPolicy hsbPolicy):
	verticalScrollBarPolicy(vsbPolicy),
	horizontalScrollBarPolicy(hsbPolicy)
{
	setLayout(ScrollPaneLayout::create());
	setVerticalScrollBarPolicy(vsbPolicy);
	setHorizontalScrollBarPolicy(hsbPolicy);
	setViewport(createViewport());
	setVerticalScrollBar(createVerticalScrollBar());
	setHorizontalScrollBar(createHorizontalScrollBar());
	if (view) {
		setViewportView(view);
	}
}
//-----------------------------------------------------------------------------
ScrollbarPtr ScrollPane::createHorizontalScrollBar() {
	return MyScrollbar::create(*this, Scrollbar::HORIZONTAL);
}
//-----------------------------------------------------------------------------
ScrollbarPtr ScrollPane::createVerticalScrollBar() {
	return MyScrollbar::create(*this, Scrollbar::VERTICAL);
}
//-----------------------------------------------------------------------------
void ScrollPane::setHorizontalScrollBar(ScrollbarPtr scrollbar) {
	ScrollbarPtr old = getHorizontalScrollBar();
	horizontalScrollBar = scrollbar;
	if (horizontalScrollBar) {
		addBack(horizontalScrollBar, HORIZONTAL_SCROLLBAR);
	} else if (!old) {
		remove(old);
	}
	firePropertyChanged(PROPERTY_HORIZONTAL_SCROLLBAR, old, horizontalScrollBar);

	revalidate();
	redraw();
}
//-----------------------------------------------------------------------------
void ScrollPane::setVerticalScrollBar(ScrollbarPtr scrollbar) {
	ScrollbarPtr old = getVerticalScrollBar();
	verticalScrollBar = scrollbar;
	addBack(verticalScrollBar, VERTICAL_SCROLLBAR);
	firePropertyChanged(PROPERTY_VERTICAL_SCROLLBAR, old, verticalScrollBar);

	revalidate();
	redraw();
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPane::createViewport() {
	return Viewport::create();
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPane::getViewport() const {
	return viewport;
}
//-----------------------------------------------------------------------------
void ScrollPane::setViewport(ViewportPtr viewport) {
	ViewportPtr old = getViewport();
	this->viewport = viewport;
	if (viewport) {
		addBack(viewport, VIEWPORT);
	} else if (old) {
		remove(old);
	}
	firePropertyChanged(PROPERTY_VIEWPORT, old, viewport);
	revalidate();
	redraw();
}
//-----------------------------------------------------------------------------
ScrollPane::Ptr ScrollPane::create(AComponentPtr view,
		ScrollPane::VerticalScrollbarPolicy vsbPolicy,
		ScrollPane::HorizontalScrollbarPolicy hsbPolicy)
{
	Ptr res(new ScrollPane(view, vsbPolicy, hsbPolicy));
	res->self = res;
	res->constructorAlt();
	return res;
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr
ScrollPane::ScrollPane::getComponentUI(ui::ALookAndFeelPtr laf) const
{
	return laf->getUI<ScrollPane>();
}
//-----------------------------------------------------------------------------
bool ScrollPane::isValidateRoot() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
ScrollPane::VerticalScrollbarPolicy
ScrollPane::getVerticalScrollBarPolicy() const
{
	SAMBA_LOG_NOT_YET_IMPL();
	return VERTICAL_SCROLLBAR_NEVER;
}
//-----------------------------------------------------------------------------
void ScrollPane::setVerticalScrollBarPolicy(
		ScrollPane::VerticalScrollbarPolicy policy) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
ScrollPane::HorizontalScrollbarPolicy
ScrollPane::getHorizontalScrollBarPolicy() const
{
	SAMBA_LOG_NOT_YET_IMPL();
	return HORIZONTAL_SCROLLBAR_NEVER;
}
//-----------------------------------------------------------------------------
void ScrollPane::setHorizontalScrollBarPolicy(
		ScrollPane::HorizontalScrollbarPolicy p) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
IBorder::Ptr ScrollPane::getViewportBorder() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return IBorder::Ptr();
}
//-----------------------------------------------------------------------------
void ScrollPane::setViewportBorder(IBorder::Ptr viewportBorder) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
Rectangle ScrollPane::getViewportBorderBounds() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return NULL_RECTANGLE;
}
//-----------------------------------------------------------------------------
void ScrollPane::setViewportView(AComponentPtr view) {
	if (!getViewport()) {
		setViewport( createViewport());
	}
	getViewport()->setView(view);
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPane::getRowHeader() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Viewport::Ptr();
}
//-----------------------------------------------------------------------------
void ScrollPane::setRowHeader(ViewportPtr rowHeader) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void ScrollPane::setRowHeaderView(AComponentPtr view) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPane::getColumnHeader() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Viewport::Ptr();
}
//-----------------------------------------------------------------------------
void ScrollPane::setColumnHeader(ViewportPtr columnHeader) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void ScrollPane::setColumnHeaderView(AComponentPtr view) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
AComponentPtr ScrollPane::getCorner(Area loc) {
	SAMBA_LOG_NOT_YET_IMPL();
	return AComponentPtr();
}
//-----------------------------------------------------------------------------
void ScrollPane::setCorner(ScrollPane::Area loc, AComponentPtr corner) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void ScrollPane::setComponentOrientation(Scrollbar::Orientation co) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
std::string ScrollPane::paramString() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return "";
}
//-----------------------------------------------------------------------------
void ScrollPane::resetViewPort() {
	SAMBA_LOG_NOT_YET_IMPL();
}

}}} // ScrollPane::namespace(s)
