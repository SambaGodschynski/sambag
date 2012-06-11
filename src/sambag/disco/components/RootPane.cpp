/*
 * RootPane.cpp
 *
 *  Created on: Wed May 30 14:16:30 2012
 *      Author: Johannes Unger
 */

#include "RootPane.hpp"
#include <sambag/disco/IDiscoFactory.hpp>
#include "FlowLayout.hpp"
#include "ui/UIManager.hpp"
#include "ui/ALookAndFeel.hpp"
#include "ui/basic/BasicLookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class RootPane
//=============================================================================
//-----------------------------------------------------------------------------
const std::string RootPane::PROPERTY_SURFACE = "surface";
//-----------------------------------------------------------------------------
RootPane::RootPane(ISurface::Ptr surface) : surface(surface) {
	setName("RootPane");
}
//-----------------------------------------------------------------------------
void RootPane::redrawParentIfNeeded(const Rectangle &r) {
	if (isShowing()) {
		redraw();
	}
}
//-----------------------------------------------------------------------------
ui::ALookAndFeelPtr RootPane::getCurrentLookAndFeel() const {
	return ui::UIManager::instance().getLookAndFeel(getPtr());
}
//-----------------------------------------------------------------------------
void RootPane::setSurface(ISurface::Ptr _surface) {
	ISurface::Ptr old = _surface;
	surface = _surface;
	firePropertyChanged(PROPERTY_SURFACE, old, _surface);
}
//-----------------------------------------------------------------------------
IDrawContext::Ptr RootPane::getDrawContext() const {
	if (!surface) {
		getDiscoFactory()->createRecordingSurface();
	}
	return getDiscoFactory()->createContext(surface);
}
//-----------------------------------------------------------------------------
RootPane::Ptr RootPane::create(ISurface::Ptr surface)
{
	Ptr res(new RootPane(surface));
	res->self = res;
	ui::UIManager::instance().installLookAndFeel(res,
		ui::basic::BasicLookAndFeel::create()
	);
	return res;
}
//-----------------------------------------------------------------------------
void RootPane::draw(IDrawContext::Ptr cn) {
	cn->setFillColor(getBackground());
	cn->rect(getBounds());
	cn->fill();
	AContainer::draw(cn);
}
//-----------------------------------------------------------------------------
bool RootPane::isShowing() const {
	return true;
}
//-----------------------------------------------------------------------------
void RootPane::__processMouseEvent_(const events::MouseEvent &ev) {
	using namespace events;
	AContainer::__processMouseEvent_(MouseEvent(getPtr(), ev));
}
//-----------------------------------------------------------------------------
bool RootPane::isValidateRoot() const {
	return true;
}
//-----------------------------------------------------------------------------
bool RootPane::isDisplayable() const {
	return true;
}
}}} // namespace(s)
