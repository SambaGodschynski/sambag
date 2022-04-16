/*
 * Knob.cpp
 *
 *  Created on: Wed Aug 29 20:20:42 2012
 *      Author: Johannes Unger
 */

#include "Knob.hpp"
#include <sambag/disco/components/ui/ALookAndFeel.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Knob
//=============================================================================
//-----------------------------------------------------------------------------
ui::AComponentUIPtr Knob::createComponentUI(ui::ALookAndFeelPtr laf) const 
{
	return laf->getUI<Knob>();
}

}}} // namespace(s)
