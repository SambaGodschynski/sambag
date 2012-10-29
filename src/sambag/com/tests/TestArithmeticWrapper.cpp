/*
 * TestArithmeticWrapper.cpp
 *
 *  Created on: Fri May 18 14:57:39 2012
 *      Author: Johannes Unger
 */

#include "TestArithmeticWrapper.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/com/ArithmeticWrapper.hpp>

namespace tests {
//=============================================================================
//  Class TestArithmeticWrapper
//=============================================================================
//-----------------------------------------------------------------------------
void TestArithmeticWrapper::testConstructor() {
	using namespace sambag::com;
	ArithmeticWrapper<int> intVal;
	CPPUNIT_ASSERT_EQUAL ((int)0, (int)intVal);
	ArithmeticWrapper<float> fVal;
	CPPUNIT_ASSERT_EQUAL (0.f, (float)fVal);
	ArithmeticWrapper<double> dVal;
	CPPUNIT_ASSERT_EQUAL (0., (double)dVal);
	ArithmeticWrapper<long double> ldVal;
	CPPUNIT_ASSERT_EQUAL ((long double)0., (long double)ldVal);
	// ArithmeticWrapper<float*> pVal; // should occur compiler error
}
//-----------------------------------------------------------------------------
void TestArithmeticWrapper::testMath() {
	using namespace sambag::com;
	ArithmeticWrapper<int> intVal = 5;
	CPPUNIT_ASSERT_EQUAL((int)10, intVal + 5);
	CPPUNIT_ASSERT_EQUAL((int)10, intVal +=5);
	CPPUNIT_ASSERT_EQUAL((int)10, (int)intVal);

	CPPUNIT_ASSERT_EQUAL((int)5, intVal - 5);
	CPPUNIT_ASSERT_EQUAL((int)5, intVal -=5);
	CPPUNIT_ASSERT_EQUAL((int)5, (int)intVal);

	CPPUNIT_ASSERT_EQUAL((int)10, intVal * 2);
	CPPUNIT_ASSERT_EQUAL((int)10, intVal *=2);
	CPPUNIT_ASSERT_EQUAL((int)10, (int)intVal);

	CPPUNIT_ASSERT_EQUAL((int)5, intVal / 2);
	CPPUNIT_ASSERT_EQUAL((int)5, intVal /=2);
	CPPUNIT_ASSERT_EQUAL((int)5, (int)intVal);
}
} //namespace
