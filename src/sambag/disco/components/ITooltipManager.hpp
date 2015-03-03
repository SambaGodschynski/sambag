/*
 * ITooltipManager.hpp
 *
 *  Created on: Fri Nov 23 14:14:54 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ITOOLTIPMANAGER_H
#define SAMBAG_ITOOLTIPMANAGER_H

#include "Forward.hpp"
#include <boost/function.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class ITooltipManager.
  */
class ITooltipManager {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	/**
	 * @return the dismissal delay value.
	 */
	virtual int getDismissDelay() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * @return the initial delay value.
	 */
	virtual int getInitialDelay() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * 	@return the reshow delay property.
	 */
	virtual int getReshowDelay() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * @return if enabled
	 */
	virtual bool isEnabled() const = 0;
	//-------------------------------------------------------------------------
	/**
	 * Registers a component for tooltip management.
	 * @param component
	 */
	virtual void registerComponent(AComponentPtr component) = 0;
	//-------------------------------------------------------------------------
	/**
	 *  Specifies the dismissal delay value.
	 * @param milliseconds
	 */
	virtual void setDismissDelay(int milliseconds) = 0;
	/**
	 *   Enables or disables the tooltip.
	 * @param flag
	 */
	//-------------------------------------------------------------------------
	virtual void setEnabled(bool flag) = 0;
	//-------------------------------------------------------------------------
	/**
	 * Specifies the initial delay value.
	 * @param milliseconds
	 */
	virtual void setInitialDelay(int milliseconds) = 0;
	//-------------------------------------------------------------------------
	/**
	 * Used to specify the amount of time before the user has to wait
	 * initialDelay milliseconds before a tooltip will be shown.
	 * @param milliseconds
	 */
	virtual void setReshowDelay(int milliseconds) = 0;
	//-------------------------------------------------------------------------
	/**
	 *  Removes a component from tooltip control.
	 * @param component
	 */
	virtual void unregisterComponent(AComponentPtr component) = 0;
}; // ITooltipManager
///////////////////////////////////////////////////////////////////////////////
typedef boost::function<ITooltipManager&()> TooltipManagerCreator;
void registerTooltipManager(const TooltipManagerCreator &creator);
const TooltipManagerCreator & getTooltipManagerCreator();
ITooltipManager & getTooltipManager();
}}} // namespace(s)

#endif /* SAMBAG_ITOOLTIPMANAGER_H */
