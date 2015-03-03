/*
 * ALookAndFeel.cpp
 *
 *  Created on: Sun Jun  3 14:23:18 2012
 *      Author: Johannes Unger
 */

#include "ALookAndFeel.hpp"
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <sstream>

namespace sambag { namespace disco { namespace components { namespace ui {
//=============================================================================
//  Class ALookAndFeel
//=============================================================================
//-----------------------------------------------------------------------------
ALookAndFeel::~ALookAndFeel() {
}
//-----------------------------------------------------------------------------
std::string ALookAndFeel::toString() const {
	std::stringstream ss;
	BOOST_FOREACH(const UIMap::value_type &v, uiMap) {
		ss<<v.first.name() << " => " << v.second->toString() << std::endl;
	}
	return ss.str();
}
}}}} // namespace(s)
