/*
 * TestPattern.cpp
 *
 *  Created on: Dec 1, 2011
 *      Author: samba
 */

#include "TestLuaHelper.hpp"
#include "sambag/lua/LuaHelper.hpp"
#include "sambag/lua/LuaSequence.hpp"
#include "sambag/lua/LuaMap.hpp"
#include <boost/foreach.hpp>
#include <string>
#include <sstream>


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestLuaHelper );

namespace tests {
//-----------------------------------------------------------------------------
void TestLuaHelper::setUp() {
	L = luaL_newstate();
	luaL_openlibs(L);
}
//-----------------------------------------------------------------------------
void TestLuaHelper::tearDown() {
	lua_close(L);
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testLuaStateRef() {
	using namespace sambag::lua;
	LuaStateRef ref = createLuaStateRef();
	CPPUNIT_ASSERT(ref);
	{ // extra scope
		LuaStateRef neu = ref;
		CPPUNIT_ASSERT(neu);
		CPPUNIT_ASSERT_EQUAL(neu.get(), ref.get());
	}
	CPPUNIT_ASSERT(ref);
	ref.reset();
	CPPUNIT_ASSERT(!ref);
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testLuaMap() {
	using namespace sambag::lua;
	typedef LuaMap<std::string, int> Map;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<insert values
	{ // extra scope
		Map map;
		map["1"] = 10;
		map["2"] = 20;
		map["3"] = 30;
		BOOST_FOREACH(const Map::value_type &it, map) {
			std::stringstream ss;
			ss << it.first;
			int i;
			ss >> i;
			CPPUNIT_ASSERT_EQUAL(i * 10, it.second);
		}
		push(map, L);
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<get Values
	Map map;
	CPPUNIT_ASSERT(get(map, L, -1));
	BOOST_FOREACH(const Map::value_type &it, map) {
		std::stringstream ss;
		ss << it.first;
		int i;
		ss >> i;
		CPPUNIT_ASSERT_EQUAL(i * 10, it.second);
	}


}
//-----------------------------------------------------------------------------
void TestLuaHelper::testLuaSequenceEx() {
	using namespace sambag::lua;
	{ // extra scope
		float data[] = {1.f, 2.f, 3.f, 4.f};
		LuaSequenceEx<float> seq(&data[0], 4);
		CPPUNIT_ASSERT_EQUAL((size_t)4, seq.size());
		for (int i=0; i<4;++i) {
			CPPUNIT_ASSERT_EQUAL((float)i+1.f, seq[i]);
		}
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<bounds
		CPPUNIT_ASSERT_THROW(seq[4], OutOfBoundsEx);
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<push
		push(seq, L);
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<get
	}
	float *data = new float[4];
	LuaSequenceEx<float> seq(&data[0], 4);
	CPPUNIT_ASSERT(get(seq, L, -1));
	for (int i=0; i<4;++i) {
		CPPUNIT_ASSERT_EQUAL((float)i+1.f, seq[i]);
	}
	delete[] data;

}
//-----------------------------------------------------------------------------
void TestLuaHelper::testGet() {
	using namespace sambag::lua;
	lua_pushstring(L, "abc");
	lua_pushnumber(L, 2.5);

	// abc
	int a = 0;
	CPPUNIT_ASSERT(!get(a, L, -2));
	// 2.5
	CPPUNIT_ASSERT(get(a, L, -1));
	CPPUNIT_ASSERT_EQUAL((int)2, a);
	double b = 0;
	CPPUNIT_ASSERT(get(b, L, -1));
	CPPUNIT_ASSERT_EQUAL((double)2.5, b);
	std::string c;
	CPPUNIT_ASSERT(get<std::string>(c, L, -1));
	CPPUNIT_ASSERT_EQUAL(std::string("2.5"), c);
	LuaSequence<float> d;
	CPPUNIT_ASSERT(!get(d, L, -2));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<checktable
	{ // extra scope
		LuaSequence<int> arr(10);
		for (size_t i=0; i<arr.size(); ++i) arr[i] = i*10;
		push(arr,L);
	}
	LuaSequence<int> arr;
	CPPUNIT_ASSERT(check<ILuaTable>(L, -1));
	CPPUNIT_ASSERT(get(arr, L, -1));
	for (size_t i=0; i<arr.size(); ++i) {
		CPPUNIT_ASSERT_EQUAL((int)i*10, arr[i]);
	}
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testMultiPushGet() {
	using namespace sambag::lua;
	typedef LuaMap<std::string, int> Map;
	{ // extra scope
		int a = 1;
		double b = 2.0;
		std::string c = "abc";
		LuaSequence<int> d(10);
		for (size_t i=0; i<d.size(); ++i) d[i] = i*10;
		Map e;
		e["1"] = 10;
		e["2"] = 20;
		e["3"] = 30;
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<push content
		push(a,b,c,d,e,L);
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<get content
	int a; double b; std::string c; LuaSequence<int> d; Map e;
	CPPUNIT_ASSERT(get(e,d,c,b,a,L,-1));
	CPPUNIT_ASSERT_EQUAL((int)1, a);
	CPPUNIT_ASSERT_EQUAL((double)2.0, b);
	CPPUNIT_ASSERT_EQUAL(std::string("abc"), c);
	for (size_t i=0; i<d.size(); ++i) {
		CPPUNIT_ASSERT_EQUAL((int)i*10, d[i]);
	}
	BOOST_FOREACH(const Map::value_type &it, e) {
		std::stringstream ss;
		ss << it.first;
		int i;
		ss >> i;
		CPPUNIT_ASSERT_EQUAL(i * 10, it.second);
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<test failure
	CPPUNIT_ASSERT(!get(a,d,c,b,a,L,-1)); // 1st wrong
	CPPUNIT_ASSERT(!get(e,a,c,b,a,L,-1)); // 2nd wrong
	CPPUNIT_ASSERT(!get(a,d,e,b,a,L,-1)); // 3th wrong
	CPPUNIT_ASSERT(!get(a,d,c,e,a,L,-1)); // 4th wrong
	CPPUNIT_ASSERT(!get(a,d,c,b,e,L,-1)); // 5th wrong
	CPPUNIT_ASSERT(!get(a,d,c,b,e,L,-2)); // wrong index
	CPPUNIT_ASSERT(!get(a,d,c,b,e,L,-3)); // wrong index
	CPPUNIT_ASSERT(!get(a,d,c,b,e,L,-4)); // wrong index
	CPPUNIT_ASSERT(!get(a,d,c,b,e,L,-5)); // wrong index
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testCheckType() {
	using namespace sambag::lua;
	lua_pushstring(L, "abc");
	lua_pushnumber(L, 0.5);
	// abc
	CPPUNIT_ASSERT(!check<int>(L, -2));
	CPPUNIT_ASSERT(check<std::string>(L, -2));
	// 0.5
	CPPUNIT_ASSERT(check<int>(L, -1));
	CPPUNIT_ASSERT(check<double>(L, -1));
	CPPUNIT_ASSERT(check<std::string>(L, -1)); // number is string convertible
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<checktable
	LuaSequence<int> seq(10);
	for (int i=0; i<10;++i) seq[i] = i;
	push(seq,L);
	CPPUNIT_ASSERT(check<ILuaTable>(L, -1));
	CPPUNIT_ASSERT(!check<ILuaTable>(L, -2));
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testNestedSequences() {
	using namespace sambag::lua;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<nested sequences
	{ // extra scope
		LuaSequence<float> l(3, 1.);
		LuaSequence<float> r(3, -1);
		LuaSequence< LuaSequence<float> > nSeq;
		nSeq.push_back(l); nSeq.push_back(r);
		push(nSeq, L);
	}
	LuaSequence< LuaSequence<float> > nSeq;
	CPPUNIT_ASSERT(get(nSeq, L, -1));
	CPPUNIT_ASSERT_EQUAL((size_t)2, nSeq.size());
	CPPUNIT_ASSERT_EQUAL((size_t)3, nSeq[0].size());
	CPPUNIT_ASSERT_EQUAL((size_t)3, nSeq[1].size());
	for (size_t i=0; i<3; ++i) {
		CPPUNIT_ASSERT_EQUAL((float)1., nSeq[0][i]);
		CPPUNIT_ASSERT_EQUAL((float)-1., nSeq[1][i]);
	}

}
//-----------------------------------------------------------------------------
void TestLuaHelper::testNestedSequencesEx() {
	using namespace sambag::lua;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<nested sequences
	{ // extra scope
		float datal[] = {1., 2., 3.};
		float datar[] = {-1., -2., -3.};
		LuaSequenceEx<float> rl[] = {
				LuaSequenceEx<float>(&datal[0], 3),
				LuaSequenceEx<float>(&datar[0], 3)
		};
		LuaSequenceEx< LuaSequenceEx<float> > nSeq(&rl[0],2);
		push(nSeq, L);
	}
	float datal[3] = {0};
	float datar[3] = {0};
	LuaSequenceEx<float> rl[] = {
			LuaSequenceEx<float>(&datal[0], 3),
			LuaSequenceEx<float>(&datar[0], 3)
	};
	LuaSequenceEx< LuaSequenceEx<float> > nSeq(&rl[0],2);
	CPPUNIT_ASSERT(get(nSeq, L, -1));
	CPPUNIT_ASSERT_EQUAL((size_t)2, nSeq.size());
	CPPUNIT_ASSERT_EQUAL((size_t)3, nSeq[0].size());
	CPPUNIT_ASSERT_EQUAL((size_t)3, nSeq[1].size());
	for (size_t i=0; i<3; ++i) {
		CPPUNIT_ASSERT_EQUAL((float)(i+1), nSeq[0][i]);
		CPPUNIT_ASSERT_EQUAL(-(float)(i+1), nSeq[1][i]);
	}
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testPop() {
	using namespace sambag;
	lua::push(10, L);
	boost::tuple<int> t01;
	// assume value;
	CPPUNIT_ASSERT(lua::check<int>(L, -1));
	CPPUNIT_ASSERT(lua::pop(L, t01));
	CPPUNIT_ASSERT_EQUAL((int)10, boost::get<0>(t01));
	// assume that value is removed
	CPPUNIT_ASSERT(!lua::check<int>(L, -1));


	lua::push(1.0f, 100, "no", L);
	boost::tuple<std::string, int, float> t02;
	CPPUNIT_ASSERT(lua::pop(L, t02));
	CPPUNIT_ASSERT_EQUAL(std::string("no"), boost::get<0>(t02));
	CPPUNIT_ASSERT_EQUAL((int)100, boost::get<1>(t02));
	CPPUNIT_ASSERT_EQUAL(1.0f, boost::get<2>(t02));

}
//-----------------------------------------------------------------------------
void TestLuaHelper::testCallF() {
	using namespace sambag::lua;
	static const std::string LCODE = "function A()\n"
			"	return 2\n"
			"end\n";
	executeString(L, LCODE);
	CPPUNIT_ASSERT_THROW(callLuaFunc(L, "NoF", 1), NoFunction);
	try {
		callLuaFunc(L, "A",1);
	} catch (const NoFunction &ex) {
		CPPUNIT_ASSERT_MESSAGE("function assertion.", false);
	} catch (const ExecutionFailed &ex) {
		CPPUNIT_ASSERT_MESSAGE(ex.errMsg, false);
	}
	int ret = 0;
	CPPUNIT_ASSERT(get(ret, L, -1));
	CPPUNIT_ASSERT_EQUAL((int)2, ret);
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testCallF01() {
	using namespace sambag::lua;
	static const std::string LCODE = "function A(x)\n"
			"	return x+1\n"
			"end\n";
	executeString(L, LCODE);
	CPPUNIT_ASSERT_THROW(callLuaFunc(L, "NoF", 1), NoFunction);
	try {
		callLuaFunc(L, "A", 1, 9);
	} catch (const NoFunction &ex) {
		CPPUNIT_ASSERT_MESSAGE("function assertion.", false);
	} catch (const ExecutionFailed &ex) {
		CPPUNIT_ASSERT_MESSAGE(ex.errMsg, false);
	}
	int ret = 0;
	CPPUNIT_ASSERT(get(ret, L, -1));
	CPPUNIT_ASSERT_EQUAL((int)10, ret);
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testCallF02() {
	using namespace sambag::lua;
	static const std::string LCODE = "function A(x,y)\n"
			"	return x-y\n"
			"end\n";
	executeString(L, LCODE);
	CPPUNIT_ASSERT_THROW(callLuaFunc(L, "NoF", 1), NoFunction);
	try {
		callLuaFunc(L, "A", 1, 3, 2);
	} catch (const NoFunction &ex) {
		CPPUNIT_ASSERT_MESSAGE("function assertion.", false);
	} catch (const ExecutionFailed &ex) {
		CPPUNIT_ASSERT_MESSAGE(ex.errMsg, false);
	}
	int ret = 0;
	CPPUNIT_ASSERT(get(ret, L, -1));
	CPPUNIT_ASSERT_EQUAL((int)1, ret);
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testCallF03() {
	using namespace sambag::lua;
	static const std::string LCODE = "function A(x,y,z)\n"
			"   res={}\n"
			"	for i = 1, z, 1 do \n"
			"       res[x[i]] = y[i] * 2\n"
			"   end\n"
			"   return res\n"
			"end\n";
	CPPUNIT_ASSERT_THROW(callLuaFunc(L, "NoF", 1), NoFunction);
	int idata[] = {100,101,102};
	std::string sdata[] = {"a", "b", "c"};
	LuaSequenceEx<int> iseq(&idata[0],3);
	LuaSequenceEx<std::string> sseq(&sdata[0],3);
	try {
		executeString(L, LCODE);
		callLuaFunc(L, "A", 1, sseq, iseq, 3);
	} catch (const NoFunction &ex) {
		CPPUNIT_ASSERT_MESSAGE("function assertion.", false);
	} catch (const ExecutionFailed &ex) {
		CPPUNIT_ASSERT_MESSAGE(ex.errMsg, false);
	}
	LuaMap<std::string, int> ret;
	CPPUNIT_ASSERT(get(ret, L, -1));
	CPPUNIT_ASSERT_EQUAL((size_t)3, ret.size());
	CPPUNIT_ASSERT_EQUAL((int)200, ret["a"]);
	CPPUNIT_ASSERT_EQUAL((int)202, ret["b"]);
	CPPUNIT_ASSERT_EQUAL((int)204, ret["c"]);
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testCallF04() {
	using namespace sambag::lua;
	static const std::string LCODE = "function A(table1,table2,fak,num)\n"
			"   res={table1, table2}\n"
			"	for i = 1, num, 1 do \n"
			"       res[1][i] = res[1][i] * fak \n"
			"       res[2][i] = res[2][i] * fak \n"
			"   end\n"
			"   return res\n"
			"end\n";
	CPPUNIT_ASSERT_THROW(callLuaFunc(L, "NoF", 1), NoFunction);
	int dtable1[] = {100,101,102};
	int dtable2[] = {-100,-101,-102};
	LuaSequenceEx<int> t1(&dtable1[0],3);
	LuaSequenceEx<int> t2(&dtable2[0],3);
	try {
		executeString(L, LCODE);
		callLuaFunc(L, "A", 1, t1, t2, 2, 3);
	} catch (const NoFunction &ex) {
		CPPUNIT_ASSERT_MESSAGE("function assertion.", false);
	} catch (const ExecutionFailed &ex) {
		CPPUNIT_ASSERT_MESSAGE(ex.errMsg, false);
	}
	LuaSequence<LuaSequence<int> > ret;
	CPPUNIT_ASSERT(get(ret, L, -1));
	CPPUNIT_ASSERT_EQUAL((size_t)2, ret.size());
	CPPUNIT_ASSERT_EQUAL((size_t)3, ret[0].size());
	CPPUNIT_ASSERT_EQUAL((size_t)3, ret[1].size());

	CPPUNIT_ASSERT_EQUAL((int)200, ret[0][0]);
	CPPUNIT_ASSERT_EQUAL((int)-200, ret[1][0]);
	CPPUNIT_ASSERT_EQUAL((int)202, ret[0][1]);
	CPPUNIT_ASSERT_EQUAL((int)-202, ret[1][1]);
	CPPUNIT_ASSERT_EQUAL((int)204, ret[0][2]);
	CPPUNIT_ASSERT_EQUAL((int)-204, ret[1][2]);
}
//-----------------------------------------------------------------------------
void TestLuaHelper::testCallF05() {
	using namespace sambag::lua;
	static const std::string LCODE = "function A(a,b,c,d,e)\n"
			"   return a+1, b+2, c+3, d+4, e+5 \n"
			"end\n";
	CPPUNIT_ASSERT_THROW(callLuaFunc(L, "NoF", 1), NoFunction);
	try {
		executeString(L, LCODE);
		callLuaFunc(L, "A", 5, 1,2,3,4,5);
	} catch (const NoFunction &ex) {
		CPPUNIT_ASSERT_MESSAGE("function assertion.", false);
	} catch (const ExecutionFailed &ex) {
		CPPUNIT_ASSERT_MESSAGE(ex.errMsg, false);
	}
	int a,b,c,d,e;
	CPPUNIT_ASSERT(get(e,d,c,b,a,L,-1));
	CPPUNIT_ASSERT_EQUAL((int)2, a);
	CPPUNIT_ASSERT_EQUAL((int)4, b);
	CPPUNIT_ASSERT_EQUAL((int)6, c);
	CPPUNIT_ASSERT_EQUAL((int)8, d);
	CPPUNIT_ASSERT_EQUAL((int)10, e);
}
} // namespace
