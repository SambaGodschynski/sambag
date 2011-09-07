/*
 * VectorN.h
 *
 *  Created on: 20.08.2010
 *  Author: samba godschynski
 */

#ifndef SAMBA_G_VECTOR_N_H_
#define SAMBA_G_VECTOR_N_H_
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#include <string>
#include <sstream>
#include "math.h"
#include <sstream>
namespace sambag {
namespace math {
//==============================================================================
template < typename T, int N >
class VectorN : public VectorN<T, N-1>{
//==============================================================================
public:
	//--------------------------------------------------------------------------
	typedef T ContentType;
	//--------------------------------------------------------------------------
	typedef VectorN<ContentType, N> VectorType;
	//--------------------------------------------------------------------------
	typedef VectorN<ContentType, N-1> PrevVectorType;
protected:
	//--------------------------------------------------------------------------
	std::string _toString() const {
		std::stringstream ss;
		if (N==1) {
			ss<<get<N>();
			return ss.str();
		}
		ss<<PrevVectorType::_toString()<<", "<<get<N>();
		return ss.str();
	}
	//--------------------------------------------------------------------------
	ContentType val;
	//--------------------------------------------------------------------------
	void _set ( ContentType *values ) {
		set<N>( values[N-1] );
		PrevVectorType::_set( values );
	}
public:
	//--------------------------------------------------------------------------
	static VectorType create( ContentType values[N] ) {
		VectorType v;
		v._set ( &values[0] );
		return v;
	}
        //--------------------------------------------------------------------------
	VectorN() : val(0) {}
	//--------------------------------------------------------------------------
	virtual ~VectorN() {}
	//--------------------------------------------------------------------------
	template < int X >
	const ContentType & get() const { return VectorN<ContentType, X>::val; }
	//--------------------------------------------------------------------------
	template < int X >
	void set( const ContentType &v ) { VectorN<ContentType, X>::val = v; }
	//--------------------------------------------------------------------------
	std::string toString() const {
		return "{" + VectorType::_toString() + "}";
	}
	//--------------------------------------------------------------------------
	VectorType & operator+=(const VectorType &v) {
		set<N>( get<N>() + v.get<N>() );
		PrevVectorType::operator+=(v);
		return *this;
	}
	//--------------------------------------------------------------------------
	VectorType operator+(const VectorType &v) const {
		VectorType neu = *this;
		neu+=v;
		return neu;
	}
	//--------------------------------------------------------------------------
	VectorType & operator-=(const VectorType &v) {
		set<N>( get<N>() - v.get<N>() );
		PrevVectorType::operator-=(v);
		return *this;
	}
	//--------------------------------------------------------------------------
	VectorType operator-(const VectorType &v) const {
		VectorType neu = *this;
		neu-=v;
		return neu;
	}
	//--------------------------------------------------------------------------
	VectorType & operator*=(const ContentType &v) {
		set<N>( get<N>() * v );
		PrevVectorType::operator*=(v);
		return *this;
	}
	//--------------------------------------------------------------------------
	VectorType operator*(const ContentType &v) const {
		VectorType neu = *this;
		neu*=v;
		return neu;
	}
	//--------------------------------------------------------------------------
	VectorType & operator/=(const ContentType &v) {
		set<N>( get<N>() / v );
		PrevVectorType::operator/=(v);
		return *this;
	}
	//--------------------------------------------------------------------------
	VectorType operator/(const ContentType &v) const {
		VectorType neu = *this;
		neu/=v;
		return neu;
	}
	//--------------------------------------------------------------------------
	bool operator==(const VectorN<ContentType, N> &v) const {
		return get<N>() == v.get<N>() && PrevVectorType::operator==(v);
	}
	//--------------------------------------------------------------------------
	ContentType abs() const {
		return sqrt( *this * *this ); 
	}
	//--------------------------------------------------------------------------
	// Skalarprodukt
	ContentType operator*( const VectorType &v ) const {
		return get<N>() * v.get<N>() + PrevVectorType::operator*(v);
	}

};
//==============================================================================
template <typename T>
class VectorN<T, 0> {
//==============================================================================
public:
	//--------------------------------------------------------------------------
	typedef T ContentType;
	//--------------------------------------------------------------------------
	typedef VectorN<ContentType, 0> VectorType;
	//--------------------------------------------------------------------------
	void _set ( ContentType *values ) {}
protected:
	//--------------------------------------------------------------------------
	ContentType sum() const { return 0; }
	//--------------------------------------------------------------------------
	std::string _toString() const { return ""; }
	//--------------------------------------------------------------------------
	VectorType & operator+= ( const VectorType &v ) { 
		return *this; 
	}
	//--------------------------------------------------------------------------
	VectorType & operator-= ( const VectorType &v ) { return *this; }
	//--------------------------------------------------------------------------
	VectorType & operator*= ( const ContentType &v ) { return *this; }
	//--------------------------------------------------------------------------
	VectorType & operator/= ( const ContentType &v ) { return *this; }
	//--------------------------------------------------------------------------
	ContentType operator*( const VectorType &v ) const { return 0.0; }
	//--------------------------------------------------------------------------
	bool operator==(const VectorType &v) const { return true; }
};
} // math
} // sambag

#endif /* VECTORN_H_ */
