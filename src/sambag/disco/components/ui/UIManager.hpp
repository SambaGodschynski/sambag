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

namespace sambag { namespace disco { namespace components { namespace ui {

//=============================================================================
/** 
  * @class UIManager.
  */
class UIManager {
//=============================================================================
friend struct Loki::CreateUsingNew<UIManager>;
public:
protected:
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<RootPane::Ptr, ALookAndFeelPtr> LafMap;
	//-------------------------------------------------------------------------
	LafMap lafMap;
private:
	//-------------------------------------------------------------------------
	UIManager() {}
	//-------------------------------------------------------------------------
	UIManager(const UIManager &) {}
public:
	//-------------------------------------------------------------------------
	static UIManager & instance();
	//-------------------------------------------------------------------------
	void installLookAndFeel(RootPane::Ptr root, ALookAndFeel::Ptr laf);
	//-------------------------------------------------------------------------
	ALookAndFeel::Ptr getLookAndFeel(RootPane::Ptr root) const;
}; // UIManager
typedef Loki::SingletonHolder<UIManager, Loki::CreateUsingNew> UIManagerHolder;
}}}} // namespace(s)

#endif /* SAMBAG_UIMANAGER_H */
