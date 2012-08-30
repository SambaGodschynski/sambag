/*
 * Knob.hpp
 *
 *  Created on: Wed Aug 29 20:20:42 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_KNOB_H
#define SAMBAG_KNOB_H

#include <boost/shared_ptr.hpp>
#include "AKnob.hpp"
#include "DefaultBoundedRangeModel.hpp"


namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class Knob.
  */
class Knob : public AKnob<DefaultBoundedRangeModel> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Knob> Ptr;
protected:
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(Knob)
	//-------------------------------------------------------------------------
	ui::AComponentUIPtr Knob::createComponentUI(ui::ALookAndFeelPtr laf) const;
}; // Knob
}}} // namespace(s)

#endif /* SAMBAG_KNOB_H */
