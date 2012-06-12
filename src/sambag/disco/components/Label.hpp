/*
 * Label.hpp
 *
 *  Created on: Tue Jun 12 17:37:30 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_LABEL_H
#define SAMBAG_LABEL_H

#include <boost/shared_ptr.hpp>
#include "AComponent.hpp"
#include <string>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class Label.
  */
class Label : public AComponent {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Label> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr getComponentUI(ui::ALookAndFeelPtr laf) const;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_LABELTEXT;
protected:
	//-------------------------------------------------------------------------
	Label();
private:
	//-------------------------------------------------------------------------
	std::string text;
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(Label)
	//-------------------------------------------------------------------------
	virtual void setText(const std::string &txt);
	//-------------------------------------------------------------------------
	virtual const std::string & getText() const {
		return text;
	}
}; // Label
}}} // namespace(s)

#endif /* SAMBAG_LABEL_H */
