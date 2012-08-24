/*
 * Scrollbar.hpp
 *
 *  Created on: Mon Jul  2 15:01:47 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SCROLLBAR_H
#define SAMBAG_SCROLLBAR_H

#include <boost/shared_ptr.hpp>
#include "AComponent.hpp"
#include "AScrollbar.hpp"
#include "DefaultBoundedRangeModel.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class Scrollbar.
  */
class Scrollbar : public AScrollbar<DefaultBoundedRangeModel> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AScrollbar<DefaultBoundedRangeModel> Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Scrollbar> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	Scrollbar(Orientation orientation, const Coordinate &value = 0.,
			const Coordinate &extent = 10., const Coordinate &min = 0.,
			const Coordinate &max = 100.);
private:
public:
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return boost::shared_dynamic_cast<Scrollbar>(Super::getPtr());
	}
	//-------------------------------------------------------------------------
	static Ptr create(Scrollbar::Orientation orientation, const Coordinate &value,
			const Coordinate &extent, const Coordinate &min,
			const Coordinate &max);
	//-------------------------------------------------------------------------
	static Ptr create(Orientation orientation);
	//-------------------------------------------------------------------------
	static Ptr create();

}; // Scrollbar
}}} // namespace(s)

#endif /* SAMBAG_SCROLLBAR_H */
