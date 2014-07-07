/*
 * SvgButtonUI.hpp
 *
 *  Created on: Thu Jul  3 09:59:43 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SvgButtonUI_H
#define SAMBAG_SvgButtonUI_H

#include <boost/shared_ptr.hpp>
#include "SvgComponentUI.hpp"
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <sambag/disco/components/DefaultButtonModell.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui {
//=============================================================================
/** 
  * @class SvgButtonUI.
  */
class SvgButtonUI : public SvgComponentUI<DefaultButtonModell>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgButtonUI> Ptr;
    //-------------------------------------------------------------------------
    typedef DefaultButtonModell::StateChangedEvent StateChanged;
    //-------------------------------------------------------------------------
    typedef SvgComponentUI<DefaultButtonModell> Super;
protected:
	//-------------------------------------------------------------------------
	SvgButtonUI(){}
    //-------------------------------------------------------------------------
    virtual void installListeners(AComponent::Ptr c);
    //-------------------------------------------------------------------------
    virtual void installButtonListeners();
    //-------------------------------------------------------------------------
    virtual void installModelListeners();
private:
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::WPtr main, handle, idle, rollover, pressed;
public:
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::Ptr getMain() const { return getPtr(main); }
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::Ptr getHandle() const { return getPtr(handle); }
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::Ptr getIdle() const { return getPtr(idle); }
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::Ptr getRollover() const { return getPtr(rollover); }
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::Ptr getPressed() const { return getPtr(pressed); }
    //-------------------------------------------------------------------------
    boost::shared_ptr<DefaultButtonModell> getModel() const { return getPtr(model); }
	//-------------------------------------------------------------------------
	/**
	 * Configures the specified component appropriately for the look and feel.
	 * @param c
	 */
	virtual void installUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new SvgButtonUI());
	}
	//-------------------------------------------------------------------------
	void onMouse(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mousePressed(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mouseReleased(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	void mouseEntered(const events::MouseEvent &ev);
    //-------------------------------------------------------------------------
	void mouseExited(const events::MouseEvent &ev);
    //-------------------------------------------------------------------------
    void onStateChanged(const StateChanged &ev);
}; // SvgButtonUI
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
}}}} // namespace(s)

#endif /* SAMBAG_SvgButtonUI_H */

