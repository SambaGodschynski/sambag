/*
 * Helper.hpp
 *
 *  Created on: 28.09.2011
 *      Author: samba
 */

#ifndef HELPER_HPP_
#define HELPER_HPP_

#include <boost/tuple/tuple.hpp>

namespace sambag { namespace com {
//=============================================================================
/**
 * Helper: TupleForeach
 * Visitor synopsis:
 * struct Visitor {
 * 		template<typename T>
 * 		void operator()(const T &t);
 * };
 *
 */
//=============================================================================
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace {
	template <typename Tuple, class Visitor, int N>
	struct _TupleForeach {
		enum {
			SIZE = boost::tuples::length<Tuple>::value,
			INDEX = SIZE-N
		};
		static void visit(const Tuple &t, Visitor &v) {
			v(boost::get<INDEX>(t));
			_TupleForeach<Tuple, Visitor, N-1>::visit(t, v);
		}
		static void visit(Tuple &t, Visitor &v) {
			v(boost::get<INDEX>(t));
			_TupleForeach<Tuple, Visitor, N-1>::visit(t, v);
		}
	};
	template <typename Tuple, class Visitor>
	struct _TupleForeach<Tuple, Visitor, 0> {
		static void visit(const Tuple &t, Visitor &v) {}
		static void visit(Tuple &t, Visitor &v) {}
	};
}
//-----------------------------------------------------------------------------
/**
 * put values from stack into tuple and pop it from stack.
 * @param L
 * @param t
 */
template <typename Tuple, class Visitor>
void foreach(const Tuple &t, Visitor &v) {
	enum {N = boost::tuples::length<Tuple>::value};
	_TupleForeach<Tuple, Visitor, N>::visit(t, v);

}
/**
 * put values from stack into tuple and pop it from stack.
 * @param L
 * @param t
 */
template <typename Tuple, class Visitor>
void foreach(Tuple &t, Visitor &v) {
	enum {N = boost::tuples::length<Tuple>::value};
	_TupleForeach<Tuple, Visitor, N>::visit(t, v);

}
//=============================================================================
// Helper: extractPointContainer
//=============================================================================
namespace {
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template <typename Tuple, typename Container, int I>
struct _CopyToTuple {
	enum {N = boost::tuples::length<Tuple>::value};
	static void copyTo(
			Tuple &t,
			typename Container::const_iterator &it,
			const typename Container::const_iterator &end )
	{
		enum { X = N - I };
		if (it!=end) {
			boost::get<X>(t) = *(it++);
		}
		_CopyToTuple<Tuple, Container, I-1>::copyTo(t, it, end);
	}
};
//-----------------------------------------------------------------------------
template <typename Tuple, typename Container>
struct _CopyToTuple<Tuple, Container, 0> {
	enum {N = boost::tuples::length<Tuple>::value};
	static void copyTo(
			Tuple &t,
			typename Container::const_iterator &it,
			const typename Container::const_iterator &end )
	{

	}
};
} // namespace
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
 * copies values from Container into tuple.
 * All tuple types has fit to Container type.
 * purpose:
 * Number x0=0, x1=0, z=0;
 * tie(x0, x1, z) = fromContainer<Tuple<Number, Number, Number>, Container<Number> > (container);
 *
 * !! Urgent Advice:
 *    don't forget to initialize the tuple values. Otherwise you have foobar in it when
 *    N(container) not fits to N(tuple).
 * @param c container
 * @return a tuple with containing N(tuple) container values.
 */
template <typename Tuple, typename Container>
Tuple fromContainer(const Container &c) {
	Tuple t;
	enum {N = boost::tuples::length<Tuple>::value};
	typename Container::const_iterator it = c.begin();
	typename Container::const_iterator end = c.end();
	_CopyToTuple<Tuple, Container, N>::copyTo(t, it, end);
	return t;
}
/**
 * Such as fromContainer(const Container &c) but with to copy range, setted by
 * iterators.
 * @param it: iterator of container. will be increased for every copied value.
 * @param end: iterator of container. points to end of copy range.
 * @return a tuple with containing N(tuple) container values.
 */
template <typename Tuple, typename Container>
Tuple fromContainer(
	 typename Container::const_iterator &it,
	 const typename Container::const_iterator &end)
{
	Tuple t;
	enum {N = boost::tuples::length<Tuple>::value};
	_CopyToTuple<Tuple, Container, N>::copyTo(t, it, end);
	return t;
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//=============================================================================
// Int2Value
//=============================================================================
template <int I>
struct Int2Type {
	enum {Value=I};
};
}}

#endif /* HELPER_HPP_ */
