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
#include <sambag/com/AbstractType.hpp>

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
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<std::string, AbstractType::Ptr> PropertyMap;
protected:
private:
	//-------------------------------------------------------------------------
	PropertyMap propertyMap;
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
	//-------------------------------------------------------------------------
	/**
	 * Returns the value of the property with the specified key.  Only
	 * properties added with <code>putClientProperty</code> will return
	 * a non-<code>null</code> value.
	 *
	 * @param key the being queried
	 * @return the value of this property or <code>null</code>
	 * @see #putClientProperty
	 */
	AbstractType::Ptr getProperty(const std::string &key) const;
	//-------------------------------------------------------------------------
	/**
	 * Adds an arbitrary key/value "client property" to this laf.
	 * <p>
	 * If value is <code>null</code> this method will remove the property.
	 * Changes to client properties are reported with
	 * <code>PropertyChange</code> events.
	 * @param key the new client property key
	 * @param value the new client property value; if <code>null</code>
	 *          this method will remove the property
	 * @see #getClientProperty
	 * @see #addPropertyChangeListener
	 */
	void putProperty(const std::string &key, AbstractType::Ptr value);
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
