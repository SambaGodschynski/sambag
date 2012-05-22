/*
 * AButton.hpp
 *
 *  Created on: Mon Jun  4 12:18:30 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ABUTTON_H
#define SAMBAG_ABUTTON_H

#include <boost/shared_ptr.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class AButton.
  */
template <class ButtonModell>
class AButton : public ButtonModell {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AButton> Ptr;
protected:
	//-------------------------------------------------------------------------
	AButton();
private:
public:
}; // AButton
}}} // namespace(s)

#endif /* SAMBAG_ABUTTON_H */
