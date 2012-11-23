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
	DefaultTooltipManager(){}
private:
public:
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
}; // DefaultTooltipManager
}}} // namespace(s)

#endif /* SAMBAG_DEFAULTTOOLTIPMANAGER_H */
