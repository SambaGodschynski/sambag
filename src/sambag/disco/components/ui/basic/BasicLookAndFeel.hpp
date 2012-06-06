/*
 * BasicLookAndFeel.hpp
 *
 *  Created on: Tue Jun  5 07:33:20 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICLOOKANDFEEL_H
#define SAMBAG_BASICLOOKANDFEEL_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/ALookAndFeel.hpp>
namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicLookAndFeel.
  */
class BasicLookAndFeel : public ALookAndFeel {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicLookAndFeel> Ptr;
protected:
	//-------------------------------------------------------------------------
	BasicLookAndFeel();
	//-------------------------------------------------------------------------
	void installComponents();
private:
public:
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new BasicLookAndFeel());
	}
}; // BasicLookAndFeel
}}}}} // namespace(s)

#endif /* SAMBAG_BASICLOOKANDFEEL_H */
