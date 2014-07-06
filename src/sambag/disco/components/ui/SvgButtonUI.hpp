/*
 * SvgButtonUI.hpp
 *
 *  Created on: Thu Jul  3 09:59:43 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SvgButtonUI_H
#define SAMBAG_SvgButtonUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <sambag/disco/components/DefaultButtonModell.hpp>
#include <sambag/disco/components/SvgComponent.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui {
//=============================================================================
/** 
  * @class SvgButtonUI.
  */
class SvgButtonUI : public AComponentUI,
                    public boost::enable_shared_from_this<SvgButtonUI>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgButtonUI> Ptr;
    //-------------------------------------------------------------------------
    typedef DefaultButtonModell::StateChangedEvent StateChanged;
protected:
	//-------------------------------------------------------------------------
	SvgButtonUI(){}
    //-------------------------------------------------------------------------
    virtual void installListeners(AComponent::Ptr c);
    //-------------------------------------------------------------------------
    virtual void installHandleListeners();
    //-------------------------------------------------------------------------
    virtual void installModelListeners();
    //-------------------------------------------------------------------------
    virtual void installPropertyListeners();
private:
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::WPtr main, handle;
    boost::weak_ptr<DefaultButtonModell> model;
    //-------------------------------------------------------------------------
    template <class T>
    boost::shared_ptr<T> getPtr(boost::weak_ptr<T> _x) const {
        boost::shared_ptr<T> res = _x.lock();
        if (!res) {
            throw std::runtime_error("SvgButtonUI null_pointer");
        }
        return res;
    }
public:
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::Ptr getMain() const { return getPtr(main); }
    //-------------------------------------------------------------------------
    SvgComponent::Dummy::Ptr getHandle() const { return getPtr(handle); }
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
    void onPropertyChanged(const com::events::PropertyChanged &ev);
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

