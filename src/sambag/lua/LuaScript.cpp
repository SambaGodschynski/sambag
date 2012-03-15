/*
 * LuaScript.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: samba
 */

#include "LuaScript.hpp"

namespace sambag { namespace lua {
//=============================================================================
// class LuaScript
//=============================================================================
//-----------------------------------------------------------------------------
LuaScript::LuaScript() {
	luaState = createLuaStateRef();
	luaL_openlibs(luaState.get());
}
//-----------------------------------------------------------------------------
LuaScript::~LuaScript() {
}
//-----------------------------------------------------------------------------
void LuaScript::execFile(const std::string &filename) {
	executeFile(luaState.get(), filename.c_str());
}
//-----------------------------------------------------------------------------
void LuaScript::execString(const std::string &str) {
	executeString(luaState.get(), str.c_str());
}
}} // namespace
