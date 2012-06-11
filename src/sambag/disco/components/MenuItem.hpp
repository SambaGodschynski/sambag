/*
 * MenuItem.hpp
 *
 *  Created on: Mon Jun 11 06:33:02 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_MENUITEM_H
#define SAMBAG_MENUITEM_H

#include <boost/shared_ptr.hpp>
#include "DefaultButtonModell.hpp"
#include "AButton.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class MenuItem.
  */
class MenuItem : public AButton<DefaultButtonModell> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<MenuItem> Ptr;
	//-------------------------------------------------------------------------
	typedef DefaultButtonModell Modell;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr getComponentUI(ui::ALookAndFeelPtr laf) const;
private:
protected:
	//-------------------------------------------------------------------------
	MenuItem();
public:
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(MenuItem)

}; // MenuItem
}}} // namespace(s)

#endif /* SAMBAG_MENUITEM_H */
