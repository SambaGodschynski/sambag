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
 * Container for dash information.
 */
class Dash {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Dash> Ptr;
	//-------------------------------------------------------------------------
	typedef std::vector<Coordinate> Dashes;
private:
	//-------------------------------------------------------------------------
	Dashes dashes;
	//-------------------------------------------------------------------------
	Coordinate _offset;
protected:
	//-------------------------------------------------------------------------
	Dash( size_t count ) : dashes(count, 0.), _offset(0.) {
	}
public:
	//-------------------------------------------------------------------------
	Coordinate & operator[] (size_t index) {
		return dashes.at(index);
	}
	//-------------------------------------------------------------------------
	const Coordinate & operator[] (size_t index) const {
		return dashes.at(index);
	}
	//-------------------------------------------------------------------------
	bool operator==(const Dash &b) const {
		if (size() != b.size()) return false;
		if (offset() != b.offset()) return false;
		return std::equal(
			dashes.begin(),
			dashes.end(),
			b.dashes.begin()
		);
		return true;
	}
	//-------------------------------------------------------------------------
	bool operator!=(const Dash &b) const {
		return !(*this==b);
	}
	//-------------------------------------------------------------------------
	~Dash() {}
	//-------------------------------------------------------------------------
	static Ptr create(size_t count = 0) {
		Ptr neu(new Dash(count));
		return neu;
	}
	//-------------------------------------------------------------------------
	const Coordinate & offset() const {
		return _offset;
	}
	//-------------------------------------------------------------------------
	void offset( const Coordinate & v) {
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
	static Ptr createWithValues(const Container &c) {
		using namespace sambag::com;
		Ptr neu(new Dash(c.size()));
		size_t index = 0;
		boost_for_each( const typename Container::value_type &val, c ) {
			(*neu)[index++] = val;
		}
		return neu;
	}
	//-------------------------------------------------------------------------
	size_t size() const {
		return dashes.size();
	}
	//-------------------------------------------------------------------------
	void size(size_t v) {
		dashes.resize(v);
	}
};

}}

#endif /* DASH_HPP_ */
