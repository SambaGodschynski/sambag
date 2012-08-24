/*
 * Panel.hpp
 *
 *  Created on: Mon Jun 11 20:49:43 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_PANEL_H
#define SAMBAG_PANEL_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"
#include "Forward.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class Panel.
  */
class Panel : public AContainer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Panel> Ptr;
	//-------------------------------------------------------------------------
	ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	Panel();
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(Panel)
}; // Panel
}}} // namespace(s)

#endif /* SAMBAG_PANEL_H */
