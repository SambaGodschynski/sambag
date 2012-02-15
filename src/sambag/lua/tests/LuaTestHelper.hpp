/*
 * LuaTestHelper.hpp
 *
 *  Created on: 13 Feb 2012
 *      Author: samba
 */

#ifndef LUATESTHELPER_HPP_
#define LUATESTHELPER_HPP_

#include <cppunit/extensions/HelperMacros.h>
#include <lua.hpp>
#include <string>

namespace tests {
inline void executeString(lua_State *L, const std::string &str) {
	luaL_loadstring (L, str.c_str());
	if (lua_pcall(L, 0, 0, 0)) {
		// call lua_tostring
		CPPUNIT_ASSERT_MESSAGE(lua_tostring(L, -1), false);
	}
}
}// namespace




#endif /* LUATESTHELPER_HPP_ */
