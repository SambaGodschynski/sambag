/*
 * AKnob.hpp
 *
 *  Created on: Wed Aug 29 20:15:58 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_AKNOB_H
#define SAMBAG_AKNOB_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/AComponent.hpp>
namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class AKnob.
  */
template <class ComponentModel>
class AKnob : public AComponent, public ComponentModel {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef ComponentModel Model;
	//-------------------------------------------------------------------------
	typedef AComponent Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AKnob> Ptr;
protected:
private:
public:
}; // AKnob
}}} // namespace(s)

#endif /* SAMBAG_AKNOB_H */
