/*
 * ALookAndFeel.hpp
 *
 *  Created on: Sun Jun  3 14:23:18 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_LOOKANDFEEL_H
#define SAMBAG_LOOKANDFEEL_H

#include <boost/shared_ptr.hpp>
#include <loki/LokiTypeInfo.h>
#include <boost/unordered_map.hpp>
#include <map>
#include "AComponentUI.hpp"
#include <sambag/disco/components/Forward.hpp>
#include <sambag/com/ArbitraryType.hpp>

namespace sambag { namespace disco { namespace components { namespace ui {
//=============================================================================
/** 
  * @class ALookAndFeel.
  * Is a set of AComponentUI classes.
  */
class ALookAndFeel {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ALookAndFeel> Ptr;
protected:
private:
	//-------------------------------------------------------------------------
	/**
	 * relation between Component types and their creators
	 */
	typedef std::map<Loki::TypeInfo, AComponentUI::Ptr> UIMap;
	//-------------------------------------------------------------------------
	UIMap uiMap;
protected:
	//-------------------------------------------------------------------------
	template <typename Component, typename ComponentUI>
	void registerComponentUI();
	//-------------------------------------------------------------------------
	ALookAndFeel() {}
public:
	//-------------------------------------------------------------------------
	template <class ConcreteComponent>
	AComponentUI::Ptr getUI() const;
	//-------------------------------------------------------------------------
	virtual ~ALookAndFeel();
}; // ALookAndFeel
//=============================================================================
// impl,
//=============================================================================
//-----------------------------------------------------------------------------
template <typename Component, typename ComponentUI>
void ALookAndFeel::registerComponentUI() {
	Loki::TypeInfo info(typeid(Component));
	uiMap[info] = ComponentUI::create();
}
//-----------------------------------------------------------------------------
template <class ConcreteComponent>
AComponentUI::Ptr ALookAndFeel::getUI() const {
	UIMap::const_iterator it = uiMap.find(typeid(ConcreteComponent));
	if (it==uiMap.end()) {
		return AComponentUI::Ptr();
	}
	return it->second;
}
}}}} // namespace(s)

#endif /* SAMBAG_LOOKANDFEEL_H */
