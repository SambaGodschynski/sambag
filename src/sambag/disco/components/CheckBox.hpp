/*
 * CheckBox.hpp
 *
 *  Created on: Mon Dec  3 12:58:05 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_CHECKBOX_H
#define SAMBAG_CHECKBOX_H

#include <boost/shared_ptr.hpp>
#include "AComponent.hpp"
#include "DefaultButtonModell.hpp"
#include "AButton.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class CheckBox.
  */
class CheckBox : public AButton<DefaultButtonModell> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef DefaultButtonModell Model;
	//-------------------------------------------------------------------------
	typedef AButton Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CheckBox> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	CheckBox();
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(CheckBox)
}; // CheckBox
}}} // namespace(s)

#endif /* SAMBAG_CHECKBOX_H */
