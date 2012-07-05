/*
 * ScrollPane.hpp
 *
 *  Created on: Thu Jul  5 16:50:53 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SCROLLPANE_H
#define SAMBAG_SCROLLPANE_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class ScrollPane.
  */
class ScrollPane : public AContainer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ScrollPane> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr getComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	ScrollPane();
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(ScrollPane)
}; // ScrollPane
}}} // namespace(s)

#endif /* SAMBAG_SCROLLPANE_H */
