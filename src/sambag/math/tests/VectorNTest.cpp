#include <cppunit/config/SourcePrefix.h>
#include "VectorNTest.hpp"
#include "sambag/math/VectorN.hpp"
#include "sambag/math/VectorNCreator.hpp"
#include <string>
#include <ostream>
#include <sstream>
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::VectorNTest );

namespace tests {
//=============================================================================
void VectorNTest::testConstructor() {
//=============================================================================
	using namespace sambag;
	math::VectorN<float, 3 > v;
	math::VectorN<int, 2 > v2;
	math::VectorN<int, 50 > v3;
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
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	std::stringstream ss;
	ss<<v;
	CPPUNIT_ASSERT_EQUAL ( std::string("{0, 1.1, 2.2}"), ss.str() );
}
//=============================================================================
void VectorNTest::testOperations() {
//=============================================================================
	using namespace sambag;
	typedef float T;
	typedef math::VectorN<T, 3> Vector3D;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<VectorType operator+(const VectorType &v) 
	Vector3D v1 = math::createVector<T>(  1.1f, 2.2f, 3.3f);
	Vector3D v2 = math::createVector<T>(  1.5f, 2.5f, 3.5f);
	Vector3D ist = v1 + v2;
	Vector3D soll = math::createVector<T>( 2.6f, 4.7f, 6.8f);
    CPPUNIT_ASSERT_EQUAL (soll, ist);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<VectorType operator-(const VectorType &v)
	v1 = math::createVector<T>(  5.f, 6.f, 7.f);
	v2 = math::createVector<T>(  3.f, 4.f, 5.f);
	ist = v1 - v2;
	soll = math::createVector<T>( 2.f, 2.f, 2.f);
    CPPUNIT_ASSERT_EQUAL (soll, ist);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<VectorType & operator+=(const VectorType &v)
	ist=math::createVector<T>(  1.1f, 2.2f, 3.3f);
	ist+=ist;
	soll = math::createVector<T>( 2.2f, 4.4f, 6.6f);
	CPPUNIT_ASSERT_EQUAL (soll, ist);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<VectorType & operator-=(const VectorType &v)
	ist=math::createVector<T>(  1.1f, 2.2f, 3.3f);
	v1=math::createVector<T>(  .1f, .2f, .3f);
	ist-=v1;
	soll = math::createVector<T>( 1.f, 2.f, 3.f);
	CPPUNIT_ASSERT_EQUAL (soll, ist);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<VectorType & operator*=(const ContentType &v)
	ist=math::createVector<T>(  1.1f, 2.2f, 3.3f);
	ist*=2.f;
	soll = math::createVector<T>( 2.2f, 4.4f, 6.6f);
	CPPUNIT_ASSERT_EQUAL (soll, ist);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<VectorType operator*(const ContentType &v)
	ist=math::createVector<T>(  1.1f, 2.2f, 3.3f);
	ist = ist * 2.f;
	soll = math::createVector<T>( 2.2f, 4.4f, 6.6f);
	CPPUNIT_ASSERT_EQUAL (soll, ist);
	//
	//ist=math::createVector<T>(  1.1f, 2.2f, 3.3f);
	//ist = 2.f * ist; // not possible
	//soll = math::createVector<T>( 2.2f, 4.4f, 6.6f);
	//CPPUNIT_ASSERT_EQUAL (soll, ist);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<VectorType & operator/=(const ContentType &v)
	ist=math::createVector<T>(  1.1f, 2.2f, 3.3f);
	ist /= 0.5f;
	soll = math::createVector<T>( 2.2f, 4.4f, 6.6f);
	CPPUNIT_ASSERT_EQUAL (soll, ist);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<VectorType operator/(const ContentType &v)  
	ist=math::createVector<T>(  1.1f, 2.2f, 3.3f);
	ist = ist / 0.5f;
	soll = math::createVector<T>( 2.2f, 4.4f, 6.6f);
	CPPUNIT_ASSERT_EQUAL (soll, ist);
	//
	//ist=math::createVector<T>(  1.1f, 2.2f, 3.3f);
	//ist = 0.5f / ist; // not possible
	//soll = math::createVector<T>( 2.2f, 4.4f, 6.6f);
	//CPPUNIT_ASSERT_EQUAL (soll, ist);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<ContentType abs() 
	v1= math::createVector<T>(1.f, 2.f, 2.f);
	CPPUNIT_ASSERT_EQUAL (3.f, v1.abs());
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<dot product
	v1= math::createVector<T>( 1.f, 3.f, 5.f);
	v2= math::createVector<T>( 2.f, 0.f, 4.f);
	CPPUNIT_ASSERT_EQUAL (22.f, v1 * v2);
	CPPUNIT_ASSERT_EQUAL (22.f, v2 * v1);

}
//=============================================================================
void VectorNTest::testCompare() {
//=============================================================================
	using namespace sambag;
	typedef float T;
	typedef math::VectorN<T, 3> Vector3D;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<add
	Vector3D v1 = math::createVector<T>(  1.1f, 2.2f, 3.3f);
	Vector3D v11 = math::createVector<T>(  1.1f, 2.2f, 3.3f);
	Vector3D v2 = math::createVector<T>(  1.5f, 2.5f, 3.5f);

	CPPUNIT_ASSERT ( v1 == v11 );
	CPPUNIT_ASSERT ( v1 != v2 );

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
