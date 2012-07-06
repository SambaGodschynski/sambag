/*
 * ScrollPaneLayout.cpp
 *
 *  Created on: Fri Jul  6 09:48:21 2012
 *      Author: Johannes Unger
 */

#include "ScrollPaneLayout.hpp"
#include "Viewport.hpp"
#include "ScrollPane.hpp"
#include "Scrollbar.hpp"
#include <sambag/com/exceptions/IllegalArgumentException.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class ScrollPaneLayout
//=============================================================================
//-----------------------------------------------------------------------------
AComponentPtr ScrollPaneLayout::addSingletonComponent(
		AComponentPtr oldC, AComponentPtr newC)
{
	if (!oldC && oldC != newC) {
		oldC->getParent()->remove(oldC);
	}
	return newC;
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::adjustForVSB(bool wantsVSB, const Rectangle &available,
		const Rectangle &vsbR, const Insets &vpbInsets, bool leftToRight)
{
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::adjustForHSB(bool wantsHSB, const Rectangle &available,
		const Rectangle &hsbR, const Insets &vpbInsets)
{
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::addLayoutComponent(AComponentPtr c,
	ArbitraryType::Ptr constraint)
{
	ScrollPane::Area location;
	com::get(constraint, location);
	switch(location) {
	case ScrollPane::VIEWPORT:
		viewport = boost::shared_dynamic_cast<Viewport>(
			addSingletonComponent(viewport, c));
	case ScrollPane::VERTICAL_SCROLLBAR:
		vsb = boost::shared_dynamic_cast<Scrollbar>(
			addSingletonComponent(vsb, c));
	case ScrollPane::HORIZONTAL_SCROLLBAR:
		hsb = boost::shared_dynamic_cast<Scrollbar>(
			addSingletonComponent(hsb, c));
	case ScrollPane::ROW_HEADER:
		rowHead = boost::shared_dynamic_cast<Viewport>(
			addSingletonComponent(rowHead, c));
	case ScrollPane::COLUMN_HEADER:
		colHead = boost::shared_dynamic_cast<Viewport>(
				addSingletonComponent(colHead, c));
	case ScrollPane::LOWER_LEFT_CORNER:
		lowerLeft = addSingletonComponent(lowerLeft, c);
	case ScrollPane::LOWER_RIGHT_CORNER:
		lowerRight = addSingletonComponent(lowerRight, c);
	case ScrollPane::UPPER_LEFT_CORNER:
		upperLeft = addSingletonComponent(upperLeft, c);
	case ScrollPane::UPPER_RIGHT_CORNER:
		upperRight = addSingletonComponent(upperRight, c);
	default:
		SAMBAG_THROW(com::exceptions::IllegalArgumentException,
				"invalid layout key");
	}
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPaneLayout::getColumnHeader() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return ViewportPtr();
}
//-----------------------------------------------------------------------------
AComponentPtr ScrollPaneLayout::getCorner(const std::string &key) const {
	SAMBA_LOG_NOT_YET_IMPL();
	return AComponentPtr();
}
//-----------------------------------------------------------------------------
ScrollbarPtr ScrollPaneLayout::getHorizontalScrollBar() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return ScrollbarPtr();
}
//-----------------------------------------------------------------------------
ScrollPaneLayout::HorizontalScrollbarPolicy ScrollPaneLayout::
	getHorizontalScrollBarPolicy() const
{
	SAMBA_LOG_NOT_YET_IMPL();
	return ScrollPane::HORIZONTAL_SCROLLBAR_NEVER;
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPaneLayout::getRowHeader() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return ViewportPtr();
}
//-----------------------------------------------------------------------------
ScrollbarPtr ScrollPaneLayout::getVerticalScrollBar() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return ScrollbarPtr();
}
//-----------------------------------------------------------------------------
ScrollPaneLayout::VerticalScrollbarPolicy ScrollPaneLayout::
	getVerticalScrollBarPolicy() const
{
	SAMBA_LOG_NOT_YET_IMPL();
	return ScrollPane::VERTICAL_SCROLLBAR_NEVER;
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPaneLayout::getViewport() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return ViewportPtr();
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::layoutContainer(AContainerPtr parent) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
Dimension ScrollPaneLayout::
	preferredLayoutSize(AContainerPtr parent)
{
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::removeLayoutComponent(AComponentPtr c) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::
	setHorizontalScrollBarPolicy(ScrollPane::HorizontalScrollbarPolicy x)
{
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::
	setVerticalScrollBarPolicy(ScrollPane::VerticalScrollbarPolicy x)
{
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void ScrollPaneLayout::syncWithScrollPane(ScrollPanePtr sp) {
//	viewport = sp->getViewport();
//	vsb = sp->getVerticalScrollBar();
//	hsb = sp>getHorizontalScrollBar();
//	rowHead = sp->getRowHeader();
//	colHead = sp->getColumnHeader();
//	lowerLeft = sp->getCorner(ScrollPane::LOWER_LEFT_CORNER);
//	lowerRight = sp->getCorner(ScrollPane::LOWER_RIGHT_CORNER);
//	upperLeft = sp->getCorner(ScrollPane::UPPER_LEFT_CORNER);
//	upperRight = sp->getCorner(ScrollPane::UPPER_RIGHT_CORNER);
//	vsbPolicy = sp->getVerticalScrollBarPolicy();
//	hsbPolicy = sp->getHorizontalScrollBarPolicy();
}
}}} // ScrollPaneLayout::namespace(s)
