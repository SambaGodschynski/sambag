#include <cppunit/config/SourcePrefix.h>
#include "VectorNTest.hpp"
#include "sambag/math/VectorN.hpp"
#include "sambag/math/VectorNCreator.hpp"
#include <string>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::VectorNTest );

namespace tests {
//=============================================================================
void VectorNTest::testConstructor() {
//=============================================================================
	using namespace sambag;
	math::VectorN<float, 3 > v;
	math::VectorN<int, 2 > v2;
	math::VectorN<int, 500 > v3;
	CPPUNIT_ASSERT_EQUAL( 0.0f, v.get<1>() );
	CPPUNIT_ASSERT_EQUAL( 0.0f, v.get<2>() );
	CPPUNIT_ASSERT_EQUAL( 0.0f, v.get<3>() );
}
//=============================================================================
void VectorNTest::testToString() {
//=============================================================================
	using namespace sambag;
	using namespace math;
	typedef VectorN<float, 3> Vector3D;
	Vector3D v = createVector<float>( 0.0f , 1.1f, 2.2f  );
	std::string str = v.toString();
	CPPUNIT_ASSERT_EQUAL ( std::string("{0, 1.1, 2.2}"), str );
}
//=============================================================================
void VectorNTest::testOperations() {
//=============================================================================
	using namespace sambag;
	typedef float T;
	typedef math::VectorN<T, 3> Vector3D;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<add
	Vector3D v1 = math::createVector<T>(  1.1f, 2.2f, 3.3f);
	Vector3D v2 = math::createVector<T>(  1.5f, 2.5f, 3.5f);
	Vector3D ist = v1 + v2;
	Vector3D soll = math::createVector<T>( 2.6f, 4.7f, 6.8f);
	bool b = ist == soll;
    //CPPUNIT_ASSERT_EQUAL ( ist, soll );
	CPPUNIT_ASSERT( b );
}
//=============================================================================
void VectorNTest::testCreators() {
//=============================================================================
	using namespace sambag;
	using namespace math;
	std::string str = createVector<int>( 0 ).toString();
	CPPUNIT_ASSERT_EQUAL ( std::string("{0}"), str );
	
	str = createVector<int>( 0, 1 ).toString();
	CPPUNIT_ASSERT_EQUAL ( std::string("{0, 1}"), str );
	
	str = createVector<int>( 0, 1, 2 ).toString();
	CPPUNIT_ASSERT_EQUAL ( std::string("{0, 1, 2}"), str );

	str = createVector<int>( 0, 1, 2, 3 ).toString();
	CPPUNIT_ASSERT_EQUAL ( std::string("{0, 1, 2, 3}"), str );

	str = createVector<int>( 0, 1, 2, 3, 4 ).toString();
	CPPUNIT_ASSERT_EQUAL ( std::string("{0, 1, 2, 3, 4}"), str );
	
	str = createVector<int>( 0, 1, 2, 3, 4, 5 ).toString();
	CPPUNIT_ASSERT_EQUAL ( std::string("{0, 1, 2, 3, 4, 5}"), str );

	str = createVector<int>( 0, 1, 2, 3, 4, 5, 6 ).toString();
	CPPUNIT_ASSERT_EQUAL ( std::string("{0, 1, 2, 3, 4, 5, 6}"), str );

	str = createVector<int>( 0, 1, 2, 3, 4, 5, 6, 7 ).toString();
	CPPUNIT_ASSERT_EQUAL ( std::string("{0, 1, 2, 3, 4, 5, 6, 7}"), str );

	str = createVector<int>( 0, 1, 2, 3, 4, 5, 6, 7, 8 ).toString();
	CPPUNIT_ASSERT_EQUAL ( std::string("{0, 1, 2, 3, 4, 5, 6, 7, 8}"), str );

	str = createVector<int>( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ).toString();
	CPPUNIT_ASSERT_EQUAL ( std::string("{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}"), str );
	
}
} // namespace tests
