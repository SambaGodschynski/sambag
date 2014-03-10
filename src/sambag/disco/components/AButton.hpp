/*
 * AButton.hpp
 *
 *  Created on: Mon Jun  4 12:18:30 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ABUTTON_H
#define SAMBAG_ABUTTON_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"
#include <string>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class AButton.
  */
template <class ButtonModell>
class AButton : public AContainer, public ButtonModell {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AButton<ButtonModell> AbstractButton;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AbstractButton> Ptr;
	//-------------------------------------------------------------------------
	typedef ButtonModell Model;
protected:
	//-------------------------------------------------------------------------
	AButton() {
		font.size = 15;
	}
private:
	//-------------------------------------------------------------------------
	std::string text;
public:
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return boost::dynamic_pointer_cast<AbstractButton>(AComponent::getPtr());
	}
	//-------------------------------------------------------------------------
	/**
	 * sets the buttons text
	 * @param txt
	 */
	void setText(const std::string &txt);
	//-------------------------------------------------------------------------
	/**
	 * @return the buttons text
	 */
	const std::string & getText() const;
}; // AButton
//=============================================================================
//-----------------------------------------------------------------------------
template <class BM>
void AButton<BM>::setText(const std::string &_txt) {
	 text = _txt;
}
//-----------------------------------------------------------------------------
template <class BM>
const std::string & AButton<BM>::getText() const {
	return text;
}
}}} // namespace(s)

#endif /* SAMBAG_ABUTTON_H */
