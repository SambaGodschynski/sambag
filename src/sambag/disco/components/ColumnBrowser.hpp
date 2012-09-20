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
class ColumnBrowser : public AColumnBrowser<DefaultTreeModel<std::string> > {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ColumnBrowser> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	ColumnBrowser();
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(ColumnBrowser)
}; // ColumnBrowser
}}} // namespace(s)
#endif /* SAMBAG_COLUMNBROWSER_H */
