/*
 * Shape.hpp
 *
 *  Created on: Sun Feb 23 10:21:25 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SHAPE_H
#define SAMBAG_SHAPE_H

#include <boost/shared_ptr.hpp>
#include "IDrawable.hpp"
namespace sambag { namespace disco {
//=============================================================================
/** 
  * @class Shape.
  */
class Shape : public virtual IDrawable {
//=============================================================================
public:
    //-------------------------------------------------------------------------
	typedef boost::shared_ptr<Shape> Ptr;
    //-------------------------------------------------------------------------
    /**
     * @brief put shapes into context
     */
    virtual void shape(IDrawContext::Ptr context) = 0;
	//-------------------------------------------------------------------------
    virtual void draw(IDrawContext::Ptr context);

}; // Shape
}} // namespace(s)

#endif /* SAMBAG_SHAPE_H */
