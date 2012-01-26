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

}}

#endif /* HELPER_HPP_ */
