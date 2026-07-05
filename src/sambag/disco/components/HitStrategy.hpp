/*
 * HitStrategy.hpp
 *
 *  Created on: Mon Jul 14 19:29:55 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_HITSTRATEGY_H
#define SAMBAG_HITSTRATEGY_H

#include <memory>
#include "Forward.hpp"
#include <sambag/disco/Geometry.hpp>

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class HitStrategy.
  * @brief a strategy pattern about the question: hits a point a component?
  */
class HitStrategy {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef std::shared_ptr<HitStrategy> Ptr;
	typedef std::weak_ptr<HitStrategy> WPtr;
    //-------------------------------------------------------------------------
    virtual bool operator()(AComponentPtr c, const Point2D &p) = 0;
}; // HitStrategy
}}} // namespace(s)

#endif /* SAMBAG_HITSTRATEGY_H */
