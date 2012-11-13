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
	virtual ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_LABELTEXT;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_ICON;
protected:
	//-------------------------------------------------------------------------
	ISurface::Ptr icon;
	//-------------------------------------------------------------------------
	Label();
private:
	//-------------------------------------------------------------------------
	std::string text;
public:
	//-------------------------------------------------------------------------
	ISurface::Ptr getIcon() const;
	//-------------------------------------------------------------------------
	void setIcon(ISurface::Ptr icon);
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
