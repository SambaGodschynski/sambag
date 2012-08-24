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
	typedef AButton<DefaultButtonModell> Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Button> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
private:
protected:
	//-------------------------------------------------------------------------
	Button();
	//-------------------------------------------------------------------------
	virtual std::string parameterString() const;
public:
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(Button)
}; // Button
}}} // namespace(s)

#endif /* SAMBAG_BUTTON_H */
