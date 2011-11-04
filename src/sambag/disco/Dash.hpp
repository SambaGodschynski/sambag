/*
 * Dash.hpp
 *
 *  Created on: Dec 6, 2011
 *      Author: samba
 */

#ifndef DASH_HPP_
#define DASH_HPP_

#include "sambag/com/Common.hpp"
#include <algorithm>
#include <vector>

namespace sambag { namespace disco {
//=============================================================================
/**
 * @class Dash
 * Container for dash information
 */
class Dash {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Dash> Ptr;
private:
	//-------------------------------------------------------------------------
	Number *vals;
	//-------------------------------------------------------------------------
	size_t count;
	//-------------------------------------------------------------------------
	Number _offset;
protected:
	//-------------------------------------------------------------------------
	Dash( size_t count ) : count(count), _offset(0.) {
		vals = count > 0 ? new Number[count] : NULL;
	}
public:
	//-------------------------------------------------------------------------
	bool operator==(const Dash &b) const {
		if (size() != b.size()) return false;
		if (offset() != b.offset()) return false;
		return std::equal(
			values(),
			&(values()[size()-1]),
			b.values()
		);
		return true;
	}
	//-------------------------------------------------------------------------
	bool operator!=(const Dash &b) const {
		return !(*this==b);
	}
	//-------------------------------------------------------------------------
	~Dash() {
		if (vals)
			delete[] vals;
	}
	//-------------------------------------------------------------------------
	static Ptr create(size_t count) {
		Ptr neu(new Dash(count));
		return neu;
	}
	//-------------------------------------------------------------------------
	const Number & offset() const {
		return _offset;
	}
	//-------------------------------------------------------------------------
	void offset( const Number & v) {
		_offset = v;
	}
	//-------------------------------------------------------------------------
	/**
	 * creates dash using container values.
	 * it is recommend to use it with boost::assign.
	 * @param c
	 * @return
	 */
	template <typename Container>
	static Ptr create(const Container &c) {
		using namespace sambag::com;
		Ptr neu(new Dash(c.size()));
		size_t index = 0;
		boost_for_each( const typename Container::value_type &val, c ) {
			neu->values()[index++] = val;
		}
		return neu;
	}
	//-------------------------------------------------------------------------
	size_t size() const {
		return count;
	}
	//-------------------------------------------------------------------------
	Number * values() const {
		return vals;
	}
};

}}

#endif /* DASH_HPP_ */
