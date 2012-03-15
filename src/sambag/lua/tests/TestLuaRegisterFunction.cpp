/*
 * TestLuaRegisterFunction.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: samba
 */

#include "TestLuaRegisterFunction.hpp"
#include <sambag/lua/Lua.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestLuaScript );

struct FooFunction_Tag {
	typedef boost::function<void()> Function;
	static const char * name() { return "foo"; }
};

struct WhoAmIFunction_Tag {
	typedef boost::function<std::string()> Function;
	static const char * name() { return "whoAmI"; }
};

struct StrLenFunction_Tag {
	typedef boost::function<size_t(std::string)> Function;
	static const char * name() { return "strLen"; }
};

struct SetIntFunction_Tag {
	typedef boost::function<void(int)> Function;
	static const char * name() { return "setInt"; }
};
typedef std::string Str;

struct FooMonster {
	bool wasCalled;
	int intValue;
	typedef Str S;
	S stringValue;
	FooMonster() : wasCalled(false), intValue(0) {}
	//-------------------------------------------------------------------------
	// 0..1 args
	void foo() { wasCalled = true; }
	size_t strLen(std::string str) { return str.length(); }
	void setInt(int value) { intValue = value; }
	S whoAmI() { return "FooMonster"; }
	//-------------------------------------------------------------------------
	// 2args
	S sum(S a, S b) { return a+b; }
	void add(S a, S b) { stringValue+= a+b; }
	//-------------------------------------------------------------------------
	// 3args
	S sum(S a, S b, S c) { return a+b+c; }
	void add(S a, S b, S c) { stringValue+= a+b+c; }
	//-------------------------------------------------------------------------
	// 4args
	S sum(S a, S b, S c, S d) { return a+b+c+d; }
	void add(S a, S b, S c, S d) { stringValue+= a+b+c+d; }
};

static void executeLuaString(lua_State *L, const std::string &str) {
	try {
		sambag::lua::executeString(L, str);
	} catch (const sambag::lua::ExecutionFailed &ex) {
		CPPUNIT_ASSERT_MESSAGE(ex.errMsg, false);
	}
}

namespace tests {
//-----------------------------------------------------------------------------
void TestLuaScript::setUp() {
	L = luaL_newstate();
	luaL_openlibs(L);
}
//-----------------------------------------------------------------------------
void TestLuaScript::tearDown() {
	lua_close(L);
}
//-----------------------------------------------------------------------------
void TestLuaScript::testRegisterFunction() {
	using namespace sambag::lua;
	FooMonster foo;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<register Fs
	registerFunction<FooFunction_Tag>(
		L,
		boost::bind(&FooMonster::foo, &foo)
	);
	registerFunction<StrLenFunction_Tag>(
		L,
		boost::bind(&FooMonster::strLen, &foo, _1)
	);
	registerFunction<SetIntFunction_Tag>(
		L,
		boost::bind(&FooMonster::setInt, &foo, _1)
	);
	registerFunction<WhoAmIFunction_Tag>(
		L,
		boost::bind(&FooMonster::whoAmI, &foo)
	);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<foo
	executeLuaString(L, "");
	CPPUNIT_ASSERT(!foo.wasCalled);
	executeLuaString(L, "foo()");
	CPPUNIT_ASSERT(foo.wasCalled);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<strLen
	CPPUNIT_ASSERT_EQUAL((int)0, foo.intValue);
	executeLuaString(L, "setInt( strLen('hippelpisse') )");
	CPPUNIT_ASSERT_EQUAL((int)11, foo.intValue);
}/*
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//-----------------------------------------------------------------------------
struct Sum02Function_Tag {
	typedef boost::function<Str(Str, Str)> Function;
	static const char * name() { return "sum"; }
};
struct Add02Function_Tag {
	typedef boost::function<void(Str, Str)> Function;
	static const char * name() { return "add"; }
};
//-----------------------------------------------------------------------------
void TestLuaScript::testRegisterFunction02() {
	using namespace sambag::lua;
	LuaScript script;
	FooMonster foo;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<register Fs
	script.registerFunction<Sum02Function_Tag>(
		boost::bind(&FooMonster::sum, &foo, _1, _2)
	);
	script.registerFunction<Add02Function_Tag>(
		boost::bind(&FooMonster::add, &foo, _1, _2)
	);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<execute
	CPPUNIT_ASSERT_EQUAL( Str(""), foo.stringValue);
	script.execString("add( sum('a', 'b' ), 'c' )");
	CPPUNIT_ASSERT_EQUAL( Str("abc"), foo.stringValue);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//-----------------------------------------------------------------------------
struct Sum03Function_Tag {
	typedef boost::function<Str(Str, Str, Str)> Function;
	static const char * name() { return "sum"; }
};
struct Add03Function_Tag {
	typedef boost::function<void(Str, Str, Str)> Function;
	static const char * name() { return "add"; }
};
//-----------------------------------------------------------------------------
void TestLuaScript::testRegisterFunction03() {
	using namespace sambag::lua;
	LuaScript script;
	FooMonster foo;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<register Fs
	script.registerFunction<Sum03Function_Tag>(
		boost::bind(&FooMonster::sum, &foo, _1, _2, _3)
	);
	script.registerFunction<Add03Function_Tag>(
		boost::bind(&FooMonster::add, &foo, _1, _2, _3)
	);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<execute
	CPPUNIT_ASSERT_EQUAL( Str(""), foo.stringValue);
	script.execString("add( sum('a', 'b', 'c' ), 'd', 'e' )");
	CPPUNIT_ASSERT_EQUAL( Str("abcde"), foo.stringValue);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//-----------------------------------------------------------------------------
struct Sum04Function_Tag {
	typedef boost::function<Str(Str, Str, Str, Str)> Function;
	static const char * name() { return "sum"; }
};
struct Add04Function_Tag {
	typedef boost::function<void(Str, Str, Str, Str)> Function;
	static const char * name() { return "add"; }
};
//-----------------------------------------------------------------------------
void TestLuaScript::testRegisterFunction04() {
	using namespace sambag::lua;
	LuaScript script;
	FooMonster foo;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<register Fs
	//script.registerFunction<Sum04Function_Tag>(
	//	boost::bind(&FooMonster::sum, &foo, _1, _2, _3, _4)
	//);
	//script.registerFunction<Add04Function_Tag>(
	//	boost::bind(&FooMonster::add, &foo, _1, _2, _3, _4)
	//);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<execute
	//CPPUNIT_ASSERT_EQUAL((int)0, foo.intValue);
	//script.execString("add( sum(1, -2, 5, -6), -10, 11 )");
	//CPPUNIT_ASSERT_EQUAL((int)-1, foo.intValue);
}
//-----------------------------------------------------------------------------
void TestLuaScript::testRegisterFunction05() {

}*/
} // namespace
