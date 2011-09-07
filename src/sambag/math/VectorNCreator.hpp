/*
 * VectorN.h
 *
 *  Created on: 20.08.2010
 *  Author: samba godschynski
 */

#ifndef VECTORN_CREATOR_H_
#define VECTORN_CREATOR_H_
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8


#include "VectorN.hpp"

namespace sambag {
namespace math {
//==============================================================================
//------------------------------------------------------------------------------
template <typename T>
VectorN<T, 1> createVector ( const T &v1 ) {
	typedef VectorN<T, 1> V;	
	T val[] = { v1 };	
	return V::create( val );
}
//------------------------------------------------------------------------------
template <typename T>
VectorN<T, 2> createVector ( const T &v1, const T &v2 ) {
	typedef VectorN<T, 2> V;	
	T val[] = { v1, v2 };	
	return V::create( val );
}
//------------------------------------------------------------------------------
template <typename T>
VectorN<T, 3> createVector ( const T &v1, const T &v2, const T &v3 ) {
	typedef VectorN<T, 3> V;	
	T val[] = { v1, v2, v3 };	
	return V::create( val );
}
//------------------------------------------------------------------------------
template <typename T>
VectorN<T, 4> createVector ( const T &v1, const T &v2, const T &v3, const T &v4 ) {
	typedef VectorN<T, 4> V;	
	T val[] = { v1, v2, v3, v4 };	
	return V::create( val );
}
//------------------------------------------------------------------------------
template <typename T>
VectorN<T, 5> createVector ( const T &v1, 
							 const T &v2, 
							 const T &v3, 
							 const T &v4,
							 const T &v5 ) {
	typedef VectorN<T, 5> V;	
	T val[] = { v1, v2, v3, v4, v5 };	
	return V::create( val );
}
//------------------------------------------------------------------------------
template <typename T>
VectorN<T, 6> createVector ( const T &v1, 
							 const T &v2, 
							 const T &v3, 
							 const T &v4,
							 const T &v5,
							 const T &v6 ) {
	typedef VectorN<T, 6> V;	
	T val[] = { v1, v2, v3, v4, v5, v6 };	
	return V::create( val );
}
//------------------------------------------------------------------------------
template <typename T>
VectorN<T, 7> createVector ( const T &v1, 
							 const T &v2, 
							 const T &v3, 
							 const T &v4,
							 const T &v5,
							 const T &v6,
							 const T &v7 ) {
	typedef VectorN<T, 7> V;	
	T val[] = { v1, v2, v3, v4, v5, v6, v7 };	
	return V::create( val );
}
//------------------------------------------------------------------------------
template <typename T>
VectorN<T, 8> createVector ( const T &v1, 
							 const T &v2, 
							 const T &v3, 
							 const T &v4,
							 const T &v5,
							 const T &v6,
							 const T &v7,
							 const T &v8 ) {
	typedef VectorN<T, 8> V;	
	T val[] = { v1, v2, v3, v4, v5, v6, v7, v8 };	
	return V::create( val );
}
//------------------------------------------------------------------------------
template <typename T>
VectorN<T, 9> createVector ( const T &v1, 
							 const T &v2, 
							 const T &v3, 
							 const T &v4,
							 const T &v5,
							 const T &v6,
							 const T &v7,
							 const T &v8,
						     const T &v9 ) {
	typedef VectorN<T, 9> V;	
	T val[] = { v1, v2, v3, v4, v5, v6, v7, v8, v9 };	
	return V::create( val );
}
//------------------------------------------------------------------------------
template <typename T>
VectorN<T, 10> createVector ( const T &v1, 
							 const T &v2, 
							 const T &v3, 
							 const T &v4,
							 const T &v5,
							 const T &v6,
							 const T &v7,
							 const T &v8,
						     const T &v9,
							 const T &v10 ) {
	typedef VectorN<T, 10> V;	
	T val[] = { v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 };	
	return V::create( val );
}


} // math
} // sambag

#endif /* VECTORN_CREATOR_H_ */
