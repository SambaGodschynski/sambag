/*
 * RectangleAccess.hpp
 *
 *  Created on: Sat Dec  8 15:52:47 2012
 *      Author: Johannes Unger
 */
#ifndef SAMBAG_RECTANGLEACCESS_H
#define SAMBAG_RECTANGLEACCESS_H

#include <sambag/disco/Geometry.hpp>
#include <vector>

namespace sambag { namespace disco { namespace genFormatter {
//=============================================================================
/** 
  * @class RectangleAccess.
  */
template <class Rectangle>
struct RectangleAccess {
//=============================================================================
	//-------------------------------------------------------------------------
	typedef Rectangle ComponentType;
	//-------------------------------------------------------------------------
	typedef std::vector<ComponentType*> ComponentContainer;
	//-------------------------------------------------------------------------
	typedef Coordinate ValueType;
	//-------------------------------------------------------------------------
	void getX(const ComponentType &obj, ValueType &out) const;
	//-------------------------------------------------------------------------
	void getY(const ComponentType &obj, ValueType &out) const;
	//-------------------------------------------------------------------------
	void getWidth(const ComponentType &obj, ValueType &out) const;
	//-------------------------------------------------------------------------
	void getHeight(const ComponentType &obj, ValueType &out) const;
	//-------------------------------------------------------------------------
	void getAlignmentX(const ComponentType &obj, ValueType &out) const;
	//-------------------------------------------------------------------------
	void getAlignmentY(const ComponentType &obj, ValueType &out) const;
	//-------------------------------------------------------------------------
	void setLocation(ComponentType &obj, const ValueType &x, const ValueType &y);
	//-------------------------------------------------------------------------
	void setSize(ComponentType &obj, const ValueType &w, const ValueType &h);
	//-------------------------------------------------------------------------
	void add(ComponentContainer &container, ComponentType &obj);
	//-------------------------------------------------------------------------
	void remove(ComponentContainer &container, const ComponentType &obj);
	//-------------------------------------------------------------------------
	size_t getSize(const ComponentContainer &container) const;
	//-------------------------------------------------------------------------
	const ComponentType * 
	get(const ComponentContainer &container, size_t index) const;
	//-------------------------------------------------------------------------
	ComponentType * get(const ComponentContainer &container, size_t index);
	//-------------------------------------------------------------------------
	/**
	 * @note non-concept
	 */	
	void reserve(ComponentContainer &container, size_t num) {
		container.reserve(num);	
	}
}; // RectangleAccess
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class DC>
void RectangleAccess<DC>::
getX(const ComponentType &obj, ValueType &out) const
{
	out = obj.x();
}
//-----------------------------------------------------------------------------
template <class DC>
void RectangleAccess<DC>::
getY(const ComponentType &obj, ValueType &out) const
{
	out = obj.y();
}
//-----------------------------------------------------------------------------
template <class DC>
void RectangleAccess<DC>::
getWidth(const ComponentType &obj, ValueType &out) const
{
	out = obj.width();
}
//-----------------------------------------------------------------------------
template <class DC>
void RectangleAccess<DC>::
getHeight(const ComponentType &obj, ValueType &out) const
{
	out = obj.height();
}
//-----------------------------------------------------------------------------
template <class DC>
void RectangleAccess<DC>::
getAlignmentX(const ComponentType &obj, ValueType &out) const
{
	out = 0.5;
}
//-----------------------------------------------------------------------------
template <class DC>
void RectangleAccess<DC>::
getAlignmentY(const ComponentType &obj, ValueType &out) const
{
	out = 0.5;
}
//-----------------------------------------------------------------------------
template <class DC>
void RectangleAccess<DC>::
setLocation(ComponentType &obj, const ValueType &x, const ValueType &y) 
{
	obj.x(x);
	obj.y(y);
}
//-----------------------------------------------------------------------------
template <class DC>
void RectangleAccess<DC>::
setSize(ComponentType &obj, const ValueType &w, const ValueType &h)
{
	obj.width(w);
	obj.height(h);
}
//-----------------------------------------------------------------------------
template <class DC>
void RectangleAccess<DC>::
add(ComponentContainer &container, ComponentType &obj)
{
	container.push_back(&obj);
}
//-----------------------------------------------------------------------------
template <class DC>
void RectangleAccess<DC>::
remove(ComponentContainer &container, const ComponentType &obj)
{
	typename ComponentContainer::iterator it = container.begin();
	for (; it!=container.end(); ++it) {
		if (*(*it) == obj) {
			container.erase(it);		
			break;	
		}	
	}
}
//-----------------------------------------------------------------------------
template <class DC>
size_t RectangleAccess<DC>::
getSize(const ComponentContainer &container) const
{
	return container.size();
}
//-----------------------------------------------------------------------------
template <class DC>
const typename RectangleAccess<DC>::ComponentType * RectangleAccess<DC>::
get(const ComponentContainer &container, size_t index) const
{
	if (index > container.size()) {
		return NULL;	
	}
	return container[index];
}
//-----------------------------------------------------------------------------
template <class DC>
typename RectangleAccess<DC>::ComponentType * RectangleAccess<DC>::
get(const ComponentContainer &container, size_t index)
{
	if (index > container.size()) {
		return NULL;	
	}
	return container[index];
}
}}} // namespace(s)
#endif /* SAMBAG_RECTANGLEACCESS_H */
