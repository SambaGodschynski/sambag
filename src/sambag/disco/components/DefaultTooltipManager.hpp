/*
 * DefaultTooltipManager.hpp
 *
 *  Created on: Fri Nov 23 14:43:39 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DEFAULTTOOLTIPMANAGER_H
#define SAMBAG_DEFAULTTOOLTIPMANAGER_H

#include <loki/Singleton.h>
#include "ITooltipManager.hpp"
#include "Timer.hpp"
#include "events/MouseEvent.hpp"
#include <sambag/com/events/Events.hpp>
#include <boost/unordered_map.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class DefaultTooltipManager.
  */
class DefaultTooltipManager : public ITooltipManager {
//=============================================================================
friend struct Loki::CreateUsingNew<DefaultTooltipManager>;
public:
protected:
	//-------------------------------------------------------------------------
	DefaultTooltipManager();
	//-------------------------------------------------------------------------
	void onTimer(void *src, const TimerEvent &ev);
	//-------------------------------------------------------------------------
	virtual void showTooltip(AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual void hideTooltip(AComponentPtr c);
private:
	//-------------------------------------------------------------------------
	Point2D location;
	//-------------------------------------------------------------------------
	AComponentWPtr currentObject;
	//-------------------------------------------------------------------------
	Timer::Ptr timer;
	//-------------------------------------------------------------------------
	com::events::EventSender<TimerEvent>::Connection timerConnection;
	//-------------------------------------------------------------------------
	int dismissDelay, initialDelay, reshowDelay;
	//-------------------------------------------------------------------------
	bool enabled;
	//-------------------------------------------------------------------------
	Timer::Ptr getTimer();
	//-------------------------------------------------------------------------
	Point2D determineLocation(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	typedef com::events::EventSender<events::MouseEvent>::Connection Connection;
	typedef boost::unordered_map<AComponent*, Connection> ConnectionMap;
	ConnectionMap connectionMap;
public:
	//-------------------------------------------------------------------------
	virtual ~DefaultTooltipManager();
	//-------------------------------------------------------------------------
	virtual int getDismissDelay() const;
	//-------------------------------------------------------------------------
	virtual int getInitialDelay() const;
	//-------------------------------------------------------------------------
	virtual int getReshowDelay() const;
	//-------------------------------------------------------------------------
	virtual bool isEnabled() const;
	//-------------------------------------------------------------------------
	virtual void registerComponent(AComponentPtr component);
	//-------------------------------------------------------------------------
	virtual void setDismissDelay(int milliseconds);
	//-------------------------------------------------------------------------
	virtual void setEnabled(bool flag);
	//-------------------------------------------------------------------------
	virtual void setInitialDelay(int milliseconds);
	//-------------------------------------------------------------------------
	virtual void setReshowDelay(int milliseconds);
	//-------------------------------------------------------------------------
	virtual void unregisterComponent(AComponentPtr component);
	//-------------------------------------------------------------------------
	static DefaultTooltipManager & instance();
	//-------------------------------------------------------------------------
	///////////////////////////////////////////////////////////////////////////
	// mouse events
	//-------------------------------------------------------------------------
	virtual void onMouse(void *src, const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	virtual void mouseEntered(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	virtual void mouseExited(const events::MouseEvent &ev);
}; // DefaultTooltipManager
}}} // namespace(s)

#endif /* SAMBAG_DEFAULTTOOLTIPMANAGER_H */
