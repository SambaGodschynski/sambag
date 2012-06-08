/*
 * AWindow.cpp
 *
 *  Created on: Thu Jun  7 10:44:23 2012
 *      Author: Johannes Unger
 */

#include "AWindow.hpp"
#include "components/RootPane.hpp"

namespace sambag { namespace disco {
//=============================================================================
//  Class AWindow
//=============================================================================
//-----------------------------------------------------------------------------
AWindow::AWindow(AWindow::Ptr parent) : parent(parent) {
	rootPane = components::RootPane::create();
}
//-----------------------------------------------------------------------------
components::RootPane::Ptr AWindow::getRootPane() const {
	return rootPane;
}
//-----------------------------------------------------------------------------
void AWindow::init(ISurface::Ptr surf) {
	rootPane = components::RootPane::create(surf);
}
}} // namespace(s)
