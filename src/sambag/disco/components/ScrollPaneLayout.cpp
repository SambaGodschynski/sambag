/*
 * ScrollPaneLayout.cpp
 *
 *  Created on: Fri Jul  6 09:48:21 2012
 *      Author: Johannes Unger
 */

#include "ScrollPaneLayout.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class ScrollPaneLayout
//=============================================================================
//-----------------------------------------------------------------------------
AComponentPtr ScrollPaneLayout::addSingletonAComponentPtr(
		AComponentPtr oldC, AComponentPtr newC)
{
	SAMBA_LOG_NOT_YET_IMPL();
	return AComponentPtr();
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
void ScrollPaneLayout::addLayoutComponent(const std::string &s,
		AComponentPtr c)
{
	SAMBA_LOG_NOT_YET_IMPL();
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
	SAMBA_LOG_NOT_YET_IMPL();
}
}}} // ScrollPaneLayout::namespace(s)
