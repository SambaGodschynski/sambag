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
#include <sambag/disco/svg/Style.hpp>

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
	virtual void installComponents();
	//-------------------------------------------------------------------------
	virtual void installDefaults();
	//-------------------------------------------------------------------------
	svg::Style 
	createStyle(const std::string &str);
private:
public:
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr res(new BasicLookAndFeel());
		res->installLookAndFeel();
		return res;
	}
	//-------------------------------------------------------------------------
	virtual void installLookAndFeel();
}; // BasicLookAndFeel
}}}}} // namespace(s)

#endif /* SAMBAG_BASICLOOKANDFEEL_H */
