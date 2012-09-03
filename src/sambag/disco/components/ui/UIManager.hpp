/*
 * UIManager.hpp
 *
 *  Created on: Sun Jun  3 14:17:21 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_UIMANAGER_H
#define SAMBAG_UIMANAGER_H

#include <boost/shared_ptr.hpp>
#include <loki/Singleton.h>
#include "ALookAndFeel.hpp"
#include <boost/unordered_map.hpp>
#include <sambag/disco/components/RootPane.hpp>
#include <sambag/com/ArbitraryType.hpp>

namespace sambag { namespace disco { namespace components { namespace ui {

//=============================================================================
/** 
  * @class UIManager.
  */
class UIManager {
//=============================================================================
friend struct Loki::CreateUsingNew<UIManager>;
public:
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<std::string, ArbitraryType::Ptr> PropertyMap;
protected:
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<RootPane::Ptr, ALookAndFeelPtr> LafMap;
	//-------------------------------------------------------------------------
	LafMap lafMap;
	//-------------------------------------------------------------------------
	PropertyMap propertyMap;
private:
	//-------------------------------------------------------------------------
	UIManager() {}
	//-------------------------------------------------------------------------
	UIManager(const UIManager &) {}
public:
	//-------------------------------------------------------------------------
	template <typename T>
	void putProperty(const std::string &name, const T &c);
	//-------------------------------------------------------------------------
	template <typename T>
	void getProperty(const std::string &name, T &out) const;
	//-------------------------------------------------------------------------
	ArbitraryType::Ptr getProperty(const std::string &key) const;
	//-------------------------------------------------------------------------
	void putPropertyImpl(const std::string &key, ArbitraryType::Ptr value);
	//-------------------------------------------------------------------------
	static UIManager & instance();
	//-------------------------------------------------------------------------
	void installLookAndFeel(RootPane::Ptr root, ALookAndFeel::Ptr laf);
	//-------------------------------------------------------------------------
	ALookAndFeel::Ptr getLookAndFeel(RootPane::Ptr root) const;
}; // UIManager
///////////////////////////////////////////////////////////////////////////////
typedef Loki::SingletonHolder<UIManager, Loki::CreateUsingNew> UIManagerHolder;
//-----------------------------------------------------------------------------
template <typename T>
void UIManager::putProperty(const std::string &name, const T &c) {
	typedef ConcreteType<T> Type;
	typename Type::Ptr val = Type::create(c);
	putPropertyImpl(name, val);
}
//-----------------------------------------------------------------------------
template <typename T>
void UIManager::getProperty(const std::string &name, T &out) const {
	typedef ConcreteType<T> Type;
	typename Type::Ptr val = boost::shared_dynamic_cast<Type>(
			getProperty(name)
	);
	if (val)
		out = *val;
}
inline UIManager & getUIManager() {
	return UIManager::instance();
}
///////////////////////////////////////////////////////////////////////////////
namespace {
	int resetStamp = 0;
} //namespace
//-----------------------------------------------------------------------------
/**
 * Returns a UIManager property value cached.
 * The UIManager property map will be invoked once at first call.
 * The PropertyTag has to comply following concept:
 *
 * struct BasicPropertyTag {
 *    static const char * propertyName();
 * };
 */
template <class PropertyTag, class T>
const T & getUIPropertyCached(const T& defaultValue) {
	static int localStamp = -1;
	static T val = defaultValue;
	if (localStamp!=resetStamp) {
		getUIManager().getProperty (
			std::string(typename PropertyTag::propertyName()), val
		);
		localStamp = resetStamp;
	}
	return val;
}

/**
 * Resets all cached values.
 */
inline void resetUIPorpertyCache() {
	resetStamp++;
}


#define SAMBAG_PROPERTY_TAG(classname, name) struct classname { \
	static const char * propertyName() { return (name); }   \
}

}}}} // namespace(s)

#endif /* SAMBAG_UIMANAGER_H */
