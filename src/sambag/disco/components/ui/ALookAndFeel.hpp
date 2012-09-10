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
// UICreator helper classes
struct IUICreator {
	typedef boost::shared_ptr<IUICreator> Ptr;
	virtual AComponentUI::Ptr createUI() = 0;
};
template <class ConcreteUI>
struct UICreator : public IUICreator {
	virtual AComponentUI::Ptr createUI() {
		return ConcreteUI::create();
	}
};
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
	typedef std::map<Loki::TypeInfo, IUICreator::Ptr> UIMap;
	//-------------------------------------------------------------------------
	UIMap uiMap;
protected:
	//-------------------------------------------------------------------------
	ALookAndFeel() {}
public:
	//-------------------------------------------------------------------------
	template <typename Component, typename ComponentUI>
	void registerComponentUI();
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
	uiMap[info] = IUICreator::Ptr(new UICreator<ComponentUI>() );
}
//-----------------------------------------------------------------------------
template <class ConcreteComponent>
AComponentUI::Ptr ALookAndFeel::getUI() const {
	UIMap::const_iterator it = uiMap.find(typeid(ConcreteComponent));
	if (it==uiMap.end()) {
		return AComponentUI::Ptr();
	}
	return it->second->createUI();
}
}}}} // namespace(s)

#endif /* SAMBAG_LOOKANDFEEL_H */
