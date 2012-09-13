/*
 * IListUI.hpp
 *
 *  Created on: Tue Sep 11 18:03:31 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ILISTUI_H
#define SAMBAG_ILISTUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/Forward.hpp>

namespace sambag { namespace disco { namespace components { namespace ui {

//=============================================================================
/** 
  * @class IListUI.
  */
class IListUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IListUI> Ptr;
protected:
private:
public:
	//-------------------------------------------------------------------------
	virtual Rectangle
	getCellBounds(AComponentPtr list, int index1, int index2) = 0;
	//-------------------------------------------------------------------------
	virtual int
	locationToIndex(AComponentPtr c, const Point2D &p) = 0;
	//-------------------------------------------------------------------------
	virtual
	Point2D indexToLocation(AComponentPtr c, int index) = 0;
}; // IListUI
}}}} // namespace(s)

#endif /* SAMBAG_ILISTUI_H */
