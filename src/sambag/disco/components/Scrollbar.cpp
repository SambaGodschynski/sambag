/*
 * Scrollbar.cpp
 *
 *  Created on: Mon Jul  2 15:01:47 2012
 *      Author: Johannes Unger
 */

#include "Scrollbar.hpp"
#include "ui/ALookAndFeel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Scrollbar
//=============================================================================
//-----------------------------------------------------------------------------
Scrollbar::Scrollbar(Scrollbar::Orientation orientation, const Coordinate &value,
		const Coordinate &extent, const Coordinate &min,
		const Coordinate &max)
{
	setName("Scrollbar");
	checkOrientation(orientation);
	this->unitIncrement = 1;
	this->blockIncrement = (extent == 0) ? (Coordinate)1. : extent;
	this->orientation = orientation;
	//setRequestFocusEnabled(false);
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr Scrollbar::getComponentUI(ui::ALookAndFeelPtr laf) const
{
	return laf->getUI<Scrollbar>();
}
//-----------------------------------------------------------------------------
Scrollbar::Ptr Scrollbar::create(Scrollbar::Orientation orientation,
		const Coordinate &value, const Coordinate &extent,
		const Coordinate &min, const Coordinate &max)
{
	Ptr res(new Scrollbar(orientation, value, extent, min, max));
	res->self = res;
	res->constructorAlt();
	return res;
}
//-----------------------------------------------------------------------------
Scrollbar::Ptr Scrollbar::create(Scrollbar::Orientation orientation) {
	return create(orientation, 0, 10, 0, 100);

}
//-----------------------------------------------------------------------------
Scrollbar::Ptr Scrollbar::create() {
	return create(VERTICAL);

}
}}} // namespace(s)
