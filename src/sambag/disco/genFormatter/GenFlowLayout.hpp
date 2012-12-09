/*
 * GenFlowLayout.hpp
 *
 *  Created on: Sat Dec  8 13:37:16 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_GENFLOWLAYOUT_H
#define SAMBAG_GENFLOWLAYOUT_H

namespace sambag { namespace disco { namespace genFormatter {
//=============================================================================
/** 
  * @class GenFlowLayout.
  */
template <class ComponentAccessPolicy>
class GenFlowLayout : public ComponentAccessPolicy {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 	
	typedef ComponentAccessPolicy AccessPolicy;
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 	
	typedef typename ComponentAccessPolicy::ComponentType ComponentType;
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	typedef typename ComponentAccessPolicy::ComponentContainer ComponentContainer;
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	typedef typename ComponentAccessPolicy::ValueType ValueType;
private:
	//-------------------------------------------------------------------------
	ComponentContainer objects;
public:
	//-------------------------------------------------------------------------
	const ComponentContainer & getObjects() const { return objects; }
	//-------------------------------------------------------------------------
	size_t getNumObjects() const { 
		return AccessPolicy::getSize(objects);
	}
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	void addObject(ComponentType &obj);
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	const ComponentType * getObject(size_t index) const {
		return AccessPolicy::get(objects, index);	
	}
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	ComponentType * getObject(size_t index) {
		return AccessPolicy::get(objects, index);	
	}
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	void removeObject(const ComponentType &obj);
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	void layout();
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	void getSize(ValueType &outWidth, ValueType &outHeight) const;	
}; // GenFlowLayout
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class CA>
void GenFlowLayout<CA>::addObject(ComponentType &obj)
{
	AccessPolicy::add(objects, obj);
}
//-----------------------------------------------------------------------------
template <class CA>
void GenFlowLayout<CA>::removeObject(const ComponentType &obj)
{
	AccessPolicy::remove(objects, obj);
}
//-----------------------------------------------------------------------------
template <class CA>
void GenFlowLayout<CA>::layout()
{
}
//-----------------------------------------------------------------------------
template <class CA>
void GenFlowLayout<CA>::getSize(ValueType &outWidth, ValueType &outHeight) const
{
}
}}} // namespace(s)

#endif /* SAMBAG_GENFLOWLAYOUT_H */
