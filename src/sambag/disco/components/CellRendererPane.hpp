/*
 * CellRendererPane.hpp
 *
 *  Created on: Wed Sep  5 12:05:09 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_CELLRENDERERPANE_H
#define SAMBAG_CELLRENDERERPANE_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class CellRendererPane.
  */
class CellRendererPane : public AContainer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CellRendererPane> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr getComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	CellRendererPane();
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(CellRendererPane)
}; // CellRendererPane
}}} // namespace(s)

#endif /* SAMBAG_CELLRENDERERPANE_H */
