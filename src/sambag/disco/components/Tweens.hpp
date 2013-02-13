/*
 * Tweens.hpp
 *
 *  Created on: Wed Feb 13 14:07:21 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TWEENS_H
#define SAMBAG_TWEENS_H

#include <boost/shared_ptr.hpp>

namespace sambag { namespace disco { namespace components {

namespace defaultTweens {
//=============================================================================
/** 
  * @class LinTween.
  */
//=============================================================================
template <class T>
struct LinTween { 
	T calc(const T &b, const T &d, const T &c, long t) { 
		return c*t/d + b;
	} 
};
//=============================================================================
/** 
  * @class QuadTween.
  */
//=============================================================================
template <class T>
struct QuadTween { 
	T calc(const T &b, const T &d, const T &c, long t) { 
		T p=t/d; 
		return c*p*p + b;
	} 
};
//=============================================================================
/** 
  * @class CubTween.
  */
//=============================================================================
template <class T>
struct CubTween { 
	T calc(const T &b, const T &d, const T &c, long t) { 
		T p=t/d; 
		return c*p*p*p + b;
	} 
};
//=============================================================================
/** 
  * @class QuartTween.
  */
//=============================================================================
template <class T>
struct QuartTween { 
	T calc(const T &b, const T &d, const T &c, long t) { 
		T p=t/d; 
		return c*p*p*p*p + b;
	} 
};
//=============================================================================
/** 
  * @class QuintTween.
  */
//=============================================================================
template <class T>
struct QuintTween { 
	T calc(const T &b, const T &d, const T &c, long t) { 
		T p=t/d; 
		return c*p*p*p*p*p + b;
	} 
};
} // defaultTweens

}}} // namespace(s)

#endif /* SAMBAG_TWEENS_H */
