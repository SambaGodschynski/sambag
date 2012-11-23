/*
 * WindowToolkit.cpp
 *
 *  Created on: Thu Jun  7 10:44:45 2012
 *      Author: Johannes Unger
 */

#include "WindowToolkit.hpp"
#include <boost/unordered_map.hpp>

namespace sambag { namespace disco { namespace components {
namespace {
	WindowToolkit * currFactory = NULL;
	typedef boost::unordered_map<std::string, sambag::com::ArbitraryType::Ptr> UserDataMap;
	UserDataMap userDataMap;
}
//=============================================================================
//  Class WindowToolkit
//=============================================================================
//-----------------------------------------------------------------------------
WindowToolkit * getWindowToolkit() {
	if (currFactory)
		return currFactory;
	return _getWindowToolkitImpl();
}
//-----------------------------------------------------------------------------
void setWindowToolkit(WindowToolkit *wf) {
	currFactory = wf;
}
//-----------------------------------------------------------------------------
void setGlobalUserData(const std::string &key, sambag::com::ArbitraryType::Ptr data)
{
	userDataMap[key] = data;
}
//-----------------------------------------------------------------------------
sambag::com::ArbitraryType::Ptr getGlobalUserData(const std::string &key) {
	UserDataMap::const_iterator it = userDataMap.find(key);
	if (it==userDataMap.end())
		return sambag::com::ArbitraryType::Ptr();
	return it->second;
}
}}} // namespace(s)
