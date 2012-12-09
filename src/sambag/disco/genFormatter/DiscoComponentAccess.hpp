/*
 * DiscoComponentAccess.hpp
 *
 *  Created on: Sat Dec  8 13:41:55 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DISCOCOMPONENTACCESS_H
#define SAMBAG_DISCOCOMPONENTACCESS_H

#include <sambag/disco/Geometry.hpp>
#include <vector>

namespace sambag { namespace disco { namespace genFormatter {
//=============================================================================
/** 
  * @class DiscoComponentAccess.
  */
template <class DiscoComponent>
struct DiscoComponentAccess {
//=============================================================================
	//-------------------------------------------------------------------------
	typedef DiscoComponent ComponentType;
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
	ComponentType * get(ComponentContainer &container, size_t index);
	//-------------------------------------------------------------------------
	/**
	 * @note non-concept
	 */	
	void reserve(ComponentContainer &container, size_t num);
}; // DiscoComponentAccess
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class DC>
void DiscoComponentAccess<DC>::
getX(const ComponentType &obj, ValueType &out) const
{
	out = obj.getX();
}
//-----------------------------------------------------------------------------
template <class DC>
void DiscoComponentAccess<DC>::
getY(const ComponentType &obj, ValueType &out) const
{
	out = obj.getY();
}
//-----------------------------------------------------------------------------
template <class DC>
void DiscoComponentAccess<DC>::
getWidth(const ComponentType &obj, ValueType &out) const
{
	out = obj.getWidth();
}
//-----------------------------------------------------------------------------
template <class DC>
void DiscoComponentAccess<DC>::
getHeight(const ComponentType &obj, ValueType &out) const
{
	out = obj.getHeight();
}
//-----------------------------------------------------------------------------
template <class DC>
void DiscoComponentAccess<DC>::
getAlignmentX(const ComponentType &obj, ValueType &out) const
{
	out = obj.getAlignmentX();
}
//-----------------------------------------------------------------------------
template <class DC>
void DiscoComponentAccess<DC>::
getAlignmentY(const ComponentType &obj, ValueType &out) const
{
	out = obj.getAlignmentY();
}
//-----------------------------------------------------------------------------
template <class DC>
void DiscoComponentAccess<DC>::
setLocation(ComponentType &obj, const ValueType &x, const ValueType &y) 
{
	obj.setLocation(x, y);
}
//-----------------------------------------------------------------------------
template <class DC>
void DiscoComponentAccess<DC>::
setSize(ComponentType &obj, const ValueType &w, const ValueType &h)
{
	obj.setSize(w, h);
}
//-----------------------------------------------------------------------------
template <class DC>
void DiscoComponentAccess<DC>::
add(ComponentContainer &container, ComponentType &obj)
{
	container.push_back(&obj);
}
//-----------------------------------------------------------------------------
template <class DC>
void DiscoComponentAccess<DC>::
remove(ComponentContainer &container, const ComponentType &obj)
{
	typename ComponentContainer::iterator it = container.begin();
	for (; it!=container.end(); ++it) {
		if (*it == &obj) {
			container.erase(it);		
			break;	
		}	
	}
}
//-----------------------------------------------------------------------------
template <class DC>
size_t DiscoComponentAccess<DC>::
getSize(const ComponentContainer &container) const
{
	return container.size();
}
//-----------------------------------------------------------------------------
template <class DC>
const typename DiscoComponentAccess<DC>::ComponentType * DiscoComponentAccess<DC>::
get(const ComponentContainer &container, size_t index) const
{
	if (index > container.size()) {
		return NULL;	
	}
	return container[index];
}
//-----------------------------------------------------------------------------
template <class DC>
typename DiscoComponentAccess<DC>::ComponentType * DiscoComponentAccess<DC>::
get(ComponentContainer &container, size_t index)
{
	if (index > container.size()) {
		return NULL;	
	}
	return container[index];
}
//-----------------------------------------------------------------------------
template <class DC>
void DiscoComponentAccess<DC>::
reserve(ComponentContainer &container, size_t num)
{
}
}}} // namespace(s)

#endif /* SAMBAG_DISCOCOMPONENTACCESS_H */
