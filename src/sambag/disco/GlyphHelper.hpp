/*
 * GlyphHelper.hpp
 *
 *  Created on: Fri Jan 25 11:52:06 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_GLYPHHELPER_H
#define SAMBAG_GLYPHHELPER_H

#include <boost/shared_ptr.hpp>
#include <vector>
#include <limits>
#include <string>

namespace sambag { namespace disco {
//=============================================================================
/** 
  * @class GlyphHelper.
  * Helper to use Glyphes with Generic Formatter
  */
struct GlyphHelper {
//=============================================================================
	typedef unsigned char Glyph;
	typedef double Coordinate;
	Glyph glyph;
	Coordinate x, y, w, h;
	GlyphHelper() : glyph(0), x(0), y(0), w(0), h(0) {}
	std::string str() const {
		const char res[] = {glyph, '\0'};
		return &res[0];
	}
}; // GlyphHelper


//=============================================================================
/** 
  * @class GlyphHelperAccess.
  */
template <class _GlyphHelper>
struct GlyphHelperAccess {
//=============================================================================
	//-------------------------------------------------------------------------
	typedef _GlyphHelper ComponentType;
	//-------------------------------------------------------------------------
	typedef std::vector<ComponentType*> ComponentContainer;
	//-------------------------------------------------------------------------
	typedef typename ComponentType::Coordinate ValueType;
	//-------------------------------------------------------------------------
	void getX(const ComponentType &obj, ValueType &out) const {
		out = obj.x;
	}
	//-------------------------------------------------------------------------
	void getY(const ComponentType &obj, ValueType &out) const {
		out= obj.y;
	}
	//-------------------------------------------------------------------------
	void getWidth(const ComponentType &obj, ValueType &out) const {
		out = obj.w;
	}
	//-------------------------------------------------------------------------
	void getHeight(const ComponentType &obj, ValueType &out) const {
		out = obj.h;
	}
	//-------------------------------------------------------------------------
	void getAlignmentX(const ComponentType &obj, ValueType &out) const {
		out = 0.;
	}
	//-------------------------------------------------------------------------
	void getAlignmentY(const ComponentType &obj, ValueType &out) const {
		out = 0.;
	}
	//-------------------------------------------------------------------------
	void setLocation(ComponentType &obj, const ValueType &x, const ValueType &y)
	{
		obj.x = x;
		obj.y = y;
	}
	//-------------------------------------------------------------------------
	void setSize(ComponentType &obj, const ValueType &w, const ValueType &h) {
		obj.w = w;
		obj.h = h;
	}
	//-------------------------------------------------------------------------
	void add(ComponentContainer &container, ComponentType *obj) {
		container.push_back(obj);
	}
	//-------------------------------------------------------------------------
	void remove(ComponentContainer &container, const ComponentType *obj) {
		container.remove(obj);
	}
	//-------------------------------------------------------------------------
	size_t getSize(const ComponentContainer &container) const {
		return container.size();
	}
	//-------------------------------------------------------------------------
	const ComponentType * 
	get(const ComponentContainer &container, size_t index) const {
		return container[index];
	}
	//-------------------------------------------------------------------------
	ComponentType * get(const ComponentContainer &container, size_t index) {
		return container[index];
	}
	//-------------------------------------------------------------------------
	/**
	 * @note non-concept
	 */	
	void reserve(ComponentContainer &container, size_t num) {
		container.reserve(num);	
	}
	//-------------------------------------------------------------------------
	static ValueType maxValue() {
		return std::numeric_limits<ValueType>::max();
	}
	//-------------------------------------------------------------------------
	static ValueType minValue() {
		return std::numeric_limits<ValueType>::min();
	}
}; // GlyphHelperAccess

}} // namespace(s)

#endif /* SAMBAG_GLYPHHELPER_H */
