/*
 * Helper.hpp
 *
 *  Created on: Mar 15, 2012
 *      Author: samba
 */

#ifndef LUA_HELPER_FOR_TEST_HPP_
#define LUA_HELPER_FOR_TEST_HPP_

#include <sambag/lua/LuaHelper.hpp>

inline void executeLuaString(lua_State *L, const std::string &str) {
	try {
		sambag::lua::executeString(L, str);
	} catch (const sambag::lua::ExecutionFailed &ex) {
		CPPUNIT_ASSERT_MESSAGE(ex.errMsg, false);
	}
}

#endif /* LUA_HELPER_FOR_TEST_HPP_ */
