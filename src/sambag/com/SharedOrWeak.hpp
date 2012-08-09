/*
 * SharedOrWeak.hpp
 *
 *  Created on: Fr Aug 03 13:30:29 2012
 *      Author: Johannes Unger
 */


#ifndef SAMBAG_SHAREDORWEAK_H
#define SAMBAG_SHAREDORWEAK_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
namespace sambag { namespace com {
//=============================================================================
/**
 * Helper class to specify a Ptr for SharedOrWeak as Weak.
 * (avoids amagious error cause implicite shared/weak conv.)  
 */
template <class T>
struct AsWeakPtr {
//=============================================================================
	typedef boost::shared_ptr<T> Ptr;
	Ptr wptr;
	AsWeakPtr(Ptr p) : wptr(p){}
};
//=============================================================================
/** 
 * @class SharedOrWeak.
 * Can be either a shared_ptr or a weak_ptr.
 * This is useful e.g. for container which have to acceppt both variants. 
 * See AContainer: in special cases it is necessary to add an AComponent Object
 * as weak pointer to avoid memory leaks.
 */
template <class T>
struct SharedOrWeak {
//=============================================================================
private:
	class Tester { // for save if (xy) operations.
				   // see: "Modern C++ Design, Chapter: Smart Pointer"
		void operator delete(void*){}
	};
public:
	typedef boost::shared_ptr<T> Ptr;
	typedef boost::weak_ptr<T> WPtr;
	Ptr ptr;
	WPtr wptr;
	SharedOrWeak(){}
	SharedOrWeak(Ptr ptr) : ptr(ptr) {}
	template <class Derivated>
	SharedOrWeak(AsWeakPtr<Derivated> wptr) : wptr(wptr.wptr) {}
	template <class Derivated>
	SharedOrWeak(boost::shared_ptr<Derivated> ptr) {
		this->ptr = ptr;
	}

	operator Ptr() const {
		return ptr ? ptr : wptr.lock();
	}
	Ptr operator->() const {
		return *this;
	}
	template <class B>
	bool operator==(const SharedOrWeak<B> &_b) const {
		Ptr a = *this;
		Ptr b = _b;
		return a==b;
	}
	template <class B>
	bool operator==(boost::shared_ptr<B> b) const {
		Ptr a = *this;
		return a==b;
	}
	template <class B>
	bool operator!=(const SharedOrWeak<B> &_b) const {
		Ptr a = *this;
		Ptr b = _b;
		return a!=b;
	}
	template <class B>
	bool operator!=(boost::shared_ptr<B> b) const {
		Ptr a = *this;
		return a!=b;
	}
	bool operator < (const SharedOrWeak<T> &_b) const {
		Ptr a = *this;
		Ptr b = _b;
		return a<b;
	}
	bool operator !() const {
		Ptr p = *this;
		return !(bool)p;
	}
	operator Tester*() const {
		Ptr p = *this;
		if (!p)
			return NULL;
		static Tester test;
		return &test;
	}
	void reset() {
		if (ptr)
			ptr.reset();
	}
};
}}
template <class A, class B>
bool operator==(boost::shared_ptr<A> a, const sambag::com::SharedOrWeak<B> &_b) {
	boost::shared_ptr<B> b = _b;
	return a==b;
}
template <class A, class B>
bool operator!=(boost::shared_ptr<A> a, const sambag::com::SharedOrWeak<B> &_b) {
	boost::shared_ptr<B> b = _b;
	return a!=b;
}
#endif // SAMBAG_SHAREDORWEAK_H