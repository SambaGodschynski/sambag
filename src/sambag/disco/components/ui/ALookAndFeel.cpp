/*
 * ALookAndFeel.cpp
 *
 *  Created on: Sun Jun  3 14:23:18 2012
 *      Author: Johannes Unger
 */

#include "ALookAndFeel.hpp"
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

namespace sambag { namespace disco { namespace components { namespace ui {
//=============================================================================
//  Class ALookAndFeel
//=============================================================================
//-----------------------------------------------------------------------------
ALookAndFeel::~ALookAndFeel() {
}
//-----------------------------------------------------------------------------
AbstractType::Ptr ALookAndFeel::getProperty(const std::string &key) const
{
	PropertyMap::const_iterator it = propertyMap.find(key);
	if (it==propertyMap.end())
		return AbstractType::Ptr();
	return it->second;
}
//-----------------------------------------------------------------------------
void ALookAndFeel::putProperty(const std::string &key,
		AbstractType::Ptr value)
{
	PropertyMap::iterator it = propertyMap.find(key);
	if (it==propertyMap.end()) {
		if (!value)
			return;
		bool inserted;
		boost::tie(it, inserted) =
			propertyMap.insert(std::make_pair(key, value));
		SAMBAG_ASSERT(inserted);
		return;
	}
	AbstractType::Ptr old = it->second;
	if (!value) {
		propertyMap.erase(it);
		return;
	}
	it->second = value;
}
}}}} // namespace(s)
