/*
 * TestLuaRegisterFunction.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: samba
 */
#if 0

#include "TestLuaRegisterFunction.hpp"
#include <sambag/lua/Lua.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include "Helper.hpp"

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

struct FooGodMonster {
	bool wasCalled;
	int intValue;
	typedef Str S;
	S stringValue;
	FooGodMonster() : wasCalled(false), intValue(0) {}
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
	int diffArgs(int a, S b) { return b.length() + a; }
	//-------------------------------------------------------------------------
	// 3args
	S sum(S a, S b, S c) { return a+b+c; }
	void add(S a, S b, S c) { stringValue+= a+b+c; }
	//-------------------------------------------------------------------------
	// 4args
	S sum(S a, S b, S c, S d) { return a+b+c+d; }
	void add(S a, S b, S c, S d) { stringValue+= a+b+c+d; }
	//-------------------------------------------------------------------------
	// 5args
	S sum(S a, S b, S c, S d, S e) { return a+b+c+d+e; }
	void add(S a, S b, S c, S d, S e) { stringValue+= a+b+c+d+e; }
	//-------------------------------------------------------------------------
	// return tuple
	boost::tuple<int> returnTuple01() { return boost::make_tuple(100); }
	//-------------------------------------------------------------------------
	boost::tuple<int, std::string> returnTuple02() { 
		return boost::make_tuple(11, "yezz02!"); 
	}
};

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
	FooGodMonster foo, foo2;
	LuaStateRef parallel = createLuaStateRef();
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<register Fs
	registerFunction<FooFunction_Tag>(
		L,
		boost::bind(&FooGodMonster::foo, &foo)
	);
	registerFunction<FooFunction_Tag>( // parallel luastate
		parallel.get(),
		boost::bind(&FooGodMonster::foo, &foo2)
	);
	registerFunction<StrLenFunction_Tag>(
		L,
		boost::bind(&FooGodMonster::strLen, &foo, _1)
	);
	registerFunction<SetIntFunction_Tag>(
		L,
		boost::bind(&FooGodMonster::setInt, &foo, _1)
	);
	registerFunction<WhoAmIFunction_Tag>(
		L,
		boost::bind(&FooGodMonster::whoAmI, &foo)
	);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<foo
	executeLuaString(L, "");
	CPPUNIT_ASSERT(!foo.wasCalled);
	executeLuaString(L, "foo()");
	CPPUNIT_ASSERT(foo.wasCalled);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<parafoo
	CPPUNIT_ASSERT(!foo2.wasCalled);
	executeLuaString(parallel.get(), "foo()");
	CPPUNIT_ASSERT(foo2.wasCalled);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<strLen
	CPPUNIT_ASSERT_EQUAL((int)0, foo.intValue);
	executeLuaString(L, "setInt( strLen('hippelpisse') )");
	CPPUNIT_ASSERT_EQUAL((int)11, foo.intValue);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<force argument exception
	CPPUNIT_ASSERT_THROW( executeString(L, "strLen()"), // no arguments
		ExecutionFailed
	);
}
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
struct DiffArgsFunction_Tag {
	typedef boost::function<int(int, Str)> Function;
	static const char * name() { return "diffArgs"; }
};
//-----------------------------------------------------------------------------
void TestLuaScript::testRegisterFunction02() {
	using namespace sambag::lua;
	FooGodMonster foo;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<register Fs
	registerFunction<Sum02Function_Tag>(
		L,
		boost::bind(&FooGodMonster::sum, &foo, _1, _2)
	);
	registerFunction<Add02Function_Tag>(
		L,
		boost::bind(&FooGodMonster::add, &foo, _1, _2)
	);
	registerFunction<DiffArgsFunction_Tag>(
		L,
		boost::bind(&FooGodMonster::diffArgs, &foo, _1, _2)
	);
	registerFunction<SetIntFunction_Tag>(
		L,
		boost::bind(&FooGodMonster::setInt, &foo, _1)
	);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<execute
	CPPUNIT_ASSERT_EQUAL( Str(""), foo.stringValue);
	executeLuaString(L, "add( sum('a', 'b' ), 'c' )");
	CPPUNIT_ASSERT_EQUAL( Str("abc"), foo.stringValue);

	CPPUNIT_ASSERT_EQUAL( (int)0, foo.intValue);
	executeLuaString(L, "setInt( diffArgs(-1, 'ab') )");
	CPPUNIT_ASSERT_EQUAL( (int)1, foo.intValue);
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
	FooGodMonster foo;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<register Fs
	registerFunction<Sum03Function_Tag>(
		L,
		boost::bind(&FooGodMonster::sum, &foo, _1, _2, _3)
	);
	registerFunction<Add03Function_Tag>(
		L,
		boost::bind(&FooGodMonster::add, &foo, _1, _2, _3)
	);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<execute
	CPPUNIT_ASSERT_EQUAL( Str(""), foo.stringValue);
	executeLuaString(L, "add( sum('a', 'b', 'c' ), 'd', 'e' )");
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
	FooGodMonster foo;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<register Fs
	registerFunction<Sum04Function_Tag>(
		L,
		boost::bind(&FooGodMonster::sum, &foo, _1, _2, _3, _4)
	);
	registerFunction<Add04Function_Tag>(
		L,
		boost::bind(&FooGodMonster::add, &foo, _1, _2, _3, _4)
	);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<execute
	CPPUNIT_ASSERT_EQUAL( Str(""), foo.stringValue);
	executeLuaString(L, "add( sum('a', 'b', 'c', 'd' ), 'e', 'f', 'g' )");
	CPPUNIT_ASSERT_EQUAL( Str("abcdefg"), foo.stringValue);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//-----------------------------------------------------------------------------
struct Sum05Function_Tag {
	typedef boost::function<Str(Str, Str, Str, Str, Str)> Function;
	static const char * name() { return "sum"; }
};
struct Add05Function_Tag {
	typedef boost::function<void(Str, Str, Str, Str, Str)> Function;
	static const char * name() { return "add"; }
};
//-----------------------------------------------------------------------------
void TestLuaScript::testRegisterFunction05() {
	using namespace sambag::lua;
	FooGodMonster foo;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<register Fs
	registerFunction<Sum05Function_Tag>(
		L,
		boost::bind(&FooGodMonster::sum, &foo, _1, _2, _3, _4, _5)
	);
	registerFunction<Add05Function_Tag>(
		L,
		boost::bind(&FooGodMonster::add, &foo, _1, _2, _3, _4, _5)
	);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<execute
	CPPUNIT_ASSERT_EQUAL( Str(""), foo.stringValue);
	executeLuaString(L, "add( sum('a', 'b', 'c', 'd', 'e' ), 'f', 'g', 'h', 'i' )");
	CPPUNIT_ASSERT_EQUAL( Str("abcdefghi"), foo.stringValue);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//-----------------------------------------------------------------------------
struct RTuple01_Tag {
	typedef boost::function<boost::tuple<int>()> Function;
	static const char * name() { return "returnTuple01"; }
};
//-----------------------------------------------------------------------------
struct RTuple02_Tag {
	typedef boost::function<boost::tuple<int, std::string>()> Function;
	static const char * name() { return "returnTuple02"; }
};
//-----------------------------------------------------------------------------
void TestLuaScript::testReturnTuple01() {
	using namespace sambag::lua;
	FooGodMonster foo;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<register Fs
	registerFunction<RTuple01_Tag>(
		L,
		boost::bind(&FooGodMonster::returnTuple01, &foo)
	);
	registerFunction<RTuple02_Tag>(
		L,
		boost::bind(&FooGodMonster::returnTuple02, &foo)
	);
	registerFunction<SetIntFunction_Tag>(
		L,
		boost::bind(&FooGodMonster::setInt, &foo, _1)
	);
	registerFunction<Add02Function_Tag>(
		L,
		boost::bind(&FooGodMonster::add, &foo, _1, _2)
	);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<execute
	// 01
	CPPUNIT_ASSERT_EQUAL( (int)0, foo.intValue);
	executeLuaString(L, "setInt( returnTuple01() )");
	CPPUNIT_ASSERT_EQUAL( (int)100, foo.intValue);
	// 02
	CPPUNIT_ASSERT_EQUAL( Str(""), foo.stringValue);
	executeLuaString(L, "a, b = returnTuple02(); add(a, b)");
	CPPUNIT_ASSERT_EQUAL( std::string("11yezz02!"), foo.stringValue);
	
}
} // namespace

#endif