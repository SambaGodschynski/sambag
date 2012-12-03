/*
 * TitledBorder.hpp
 *
 *  Created on: Sun Dec  2 12:56:15 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TITLEDBORDER_H
#define SAMBAG_TITLEDBORDER_H

#include <boost/shared_ptr.hpp>
#include "SolidBorder.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class TitledBorder.
  */
class TitledBorder : public SolidBorder {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef SolidBorder Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<TitledBorder> Ptr;
protected:
	//-------------------------------------------------------------------------
	TitledBorder();
	//-------------------------------------------------------------------------
	Rectangle getTextBounds(AComponentPtr c,
        IDrawContext::Ptr g, const Rectangle &r);
private:
public:
	//-------------------------------------------------------------------------
    static Ptr create() {
        return Ptr(new TitledBorder);
    }
	//-------------------------------------------------------------------------
	virtual void paintBorder(AComponentPtr c,
        IDrawContext::Ptr g, const Rectangle &r);

}; // TitledBorder
}}} // namespace(s)

#endif /* SAMBAG_TITLEDBORDER_H */
