/*
 * Button.hpp
 *
 *  Created on: Mon Jun  4 12:21:08 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BUTTON_H
#define SAMBAG_BUTTON_H

#include <boost/shared_ptr.hpp>
#include "DefaultButtonModell.hpp"
#include "AButton.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class Button.
  */
class Button : public AButton<DefaultButtonModell> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Button> Ptr;
protected:
private:
public:
}; // Button
}}} // namespace(s)

#endif /* SAMBAG_BUTTON_H */
