/*
 * ColumnBrowser.hpp
 *
 *  Created on: Wed Sep 19 11:20:11 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_COLUMNBROWSER_H
#define SAMBAG_COLUMNBROWSER_H

#include <boost/shared_ptr.hpp>
#include "AColumnBrowser.hpp"
#include "DefaultTreeModel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class ColumnBrowser.
  */
template <typename T>
class ColumnBrowser : public AColumnBrowser<DefaultTreeModel<T> > {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AColumnBrowser<DefaultTreeModel<T> >  Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ColumnBrowser> Ptr;
protected:
	//-------------------------------------------------------------------------
	ColumnBrowser() {
		Super::setName("ColumnBrowser");
	}
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(ColumnBrowser)
	//-------------------------------------------------------------------------
	ui::AComponentUIPtr
	createComponentUI(ui::ALookAndFeelPtr laf) const {
		return laf->getUI<ColumnBrowser<T> >();
	}
}; // ColumnBrowser
}}} // namespace(s)
#endif /* SAMBAG_COLUMNBROWSER_H */
