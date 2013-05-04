/*
 * Tweens.hpp
 *
 *  Created on: Wed Feb 13 14:07:21 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TWEENS_H
#define SAMBAG_TWEENS_H

#include <boost/shared_ptr.hpp>
#include <string>

namespace sambag { namespace disco { namespace components {

namespace defaultTweens {
//=============================================================================
/** 
  * @class LinTween.
  */
//=============================================================================
template <class T>
struct LinTween { 
	static T calc(T b, long d, T c, long t) { 
		return c*(T)t/(T)d + b;
	} 
};
//=============================================================================
/** 
  * @class QuadTween.
  */
//=============================================================================
template <class T>
struct QuadTween { 
	static T calc(T b, long d, T c, long t) { 
		T p=(T)t/(T)d; 
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
	static T calc(T b, long d, T c, long t) { 
		T p=(T)t/(T)d;
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
	static T calc(T b, long d, T c, long t) { 
		T p=(T)t/(T)d; 
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
	static T calc(T b, long d, T c, long t) { 
		T p=(T)t/(T)d; 
		return c*p*p*p*p*p + b;
	} 
};
//=============================================================================
/** 
  * @class QuintTween.
  */
//=============================================================================
template <class T>
struct DynamicTween {
	enum Type { LIN, QUAD, QUART, QUINT };
	Type type;
	DynamicTween() : type(LIN) {}
	void setTweenType(Type type) {
		this->type = type;
	}
	void setTweenType(const std::string &type) {
		if (type=="lin") {
			this->type = LIN; 
		}
		if (type=="quad") {
			this->type = QUAD; 
		}
		if (type=="quart") {
			this->type = QUART; 
		}
		if (type=="quint") {
			this->type = QUINT; 
		}
	}
	Type getTweenType() const {
		return type;
	}
	T calc(T b, long d, T c, long t) {
		switch (type) {
			case LIN  : return LinTween<T>::calc(b, d, c, t);
			case QUAD : return QuadTween<T>::calc(b, d, c, t);
			case QUART: return QuartTween<T>::calc(b, d, c, t);
			case QUINT: return QuintTween<T>::calc(b, d, c, t);
		}
		return T();
	}
};
} // defaultTweens

}}} // namespace(s)

#endif /* SAMBAG_TWEENS_H */
