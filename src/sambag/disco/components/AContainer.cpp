/*
 * AContainer.cpp
 *
 *  Created on: Wed May 16 12:00:29 2012
 *      Author: Johannes Unger
 */

#include "AContainer.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class AContainer
//=============================================================================
//-----------------------------------------------------------------------------
AContainer::AContainer() {

}
//-----------------------------------------------------------------------------
AContainer::~AContainer() {

}
//-----------------------------------------------------------------------------
AComponent::Ptr AContainer::add(AComponent::Ptr comp, int index) {
	SAMBA_LOG_NOT_YET_IMPL();
	return AComponent::Ptr();
}
//-----------------------------------------------------------------------------
void AContainer::doLayout() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
AComponent::Ptr AContainer::getComponentAt(const Point2D &p) const {
	SAMBA_LOG_NOT_YET_IMPL();
	return AComponent::Ptr();
}
//-----------------------------------------------------------------------------
Coordinate AContainer::getAlignmentX() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Coordinate();
}
//-----------------------------------------------------------------------------
Coordinate AContainer::getAlignmentY() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Coordinate();
}
//-----------------------------------------------------------------------------
AComponent::Ptr AContainer::getComponent(size_t n) const {
	SAMBA_LOG_NOT_YET_IMPL();
	return AComponent::Ptr();
}
//-----------------------------------------------------------------------------
size_t AContainer::getComponentCount() const {
	return components.size();
}
//-----------------------------------------------------------------------------
AContainer::Components & AContainer::getComponents() {
	return components;
}
//-----------------------------------------------------------------------------
const AContainer::Components & AContainer::getComponents() const {
	return components;
}
//-----------------------------------------------------------------------------
Insets AContainer::getInsets() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Insets();
}
//-----------------------------------------------------------------------------
ALayoutManager::Ptr AContainer::getLayout() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return ALayoutManager::Ptr();
}
//-----------------------------------------------------------------------------
Dimension AContainer::getMaximumSize() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
Dimension AContainer::getMinimumSize() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
Dimension AContainer::getPreferredSize() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return Dimension();
}
//-----------------------------------------------------------------------------
bool AContainer::isAncestorOf(AComponent::Ptr c) const {
	SAMBA_LOG_NOT_YET_IMPL();
	return false;
}
//-----------------------------------------------------------------------------
void AContainer::draw(IDrawContext::Ptr context) {
}
//-----------------------------------------------------------------------------
void AContainer::drawComponents(IDrawContext::Ptr context) {
}
//-----------------------------------------------------------------------------
void AContainer::remove(AComponent::Ptr comp) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AContainer::remove(size_t index) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AContainer::removeAll() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AContainer::setLayout(ALayoutManager::Ptr mgr) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AContainer::transferFocusBackward() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AContainer::validate() {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AContainer::update(IDrawContext::Ptr cn) {
	SAMBA_LOG_NOT_YET_IMPL();
}
//-----------------------------------------------------------------------------
void AContainer::validateTree() {
	SAMBA_LOG_NOT_YET_IMPL();
}

}}} // namespace(s)
