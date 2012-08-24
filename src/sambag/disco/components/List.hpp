/*
 * List.hpp
 *
 *  Created on: Thu Jul  5 16:54:31 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_LIST_H
#define SAMBAG_LIST_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class List.
  */
class List : public AContainer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<List> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	List();
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(List)
}; // List
}}} // namespace(s)

#endif /* SAMBAG_LIST_H */
