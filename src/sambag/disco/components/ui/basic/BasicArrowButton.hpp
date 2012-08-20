/*
 * BasicArrowButton.hpp
 *
 *  Created on: Tue Jul  3 15:29:39 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICARROWBUTTON_H
#define SAMBAG_BASICARROWBUTTON_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/Button.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicArrowButton.
  */
class BasicArrowButton : public Button {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef Button Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicArrowButton> Ptr;
	//-------------------------------------------------------------------------
	enum Direction {
		NORTH, WEST, SOUTH, EAST
	};
protected:
	//-------------------------------------------------------------------------
	Direction direction;
	//-------------------------------------------------------------------------
	BasicArrowButton(Direction direction);
private:
public:
	//-------------------------------------------------------------------------
	static Ptr create(Direction direction) {
		Ptr res(new BasicArrowButton(direction));
		res->self = res;
		res->postConstructor();
		return res;
	}
}; // BasicArrowButton
}}}}} // namespace(s)

#endif /* SAMBAG_BASICARROWBUTTON_H */
