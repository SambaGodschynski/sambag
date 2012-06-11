/*
 * RootPane.hpp
 *
 *  Created on: Wed May 30 14:16:30 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ROOTPANE_H
#define SAMBAG_ROOTPANE_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/ISurface.hpp>
#include "AContainer.hpp"
#include <sambag/disco/components/ui/ALookAndFeel.hpp>
#include "Forward.hpp"
#include "Panel.hpp"

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class RootPane.
  */
class RootPane : public Panel {
//=============================================================================
friend class ui::UIManager;
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<RootPane> Ptr;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_SURFACE;
protected:
	//-------------------------------------------------------------------------
	ui::AComponentUIPtr getComponentUI(ui::ALookAndFeelPtr laf) const {
		return ui::AComponentUIPtr();
	}
	//-------------------------------------------------------------------------
	RootPane(ISurface::Ptr surface);
	//-------------------------------------------------------------------------
	virtual void redrawParentIfNeeded(const Rectangle &r);
private:
	//-------------------------------------------------------------------------
	ISurface::Ptr surface;
public:
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return boost::shared_dynamic_cast<RootPane>(AComponent::getPtr());
	}
	//-------------------------------------------------------------------------
	ui::ALookAndFeelPtr getCurrentLookAndFeel() const;
	//-------------------------------------------------------------------------
	virtual bool isValidateRoot() const;
	//-------------------------------------------------------------------------
	virtual bool isDisplayable() const;
	//-------------------------------------------------------------------------
	virtual bool isShowing() const;
	//-------------------------------------------------------------------------
	void __processMouseEvent_(const events::MouseEvent &ev);
	//-------------------------------------------------------------------------
	virtual void draw(IDrawContext::Ptr cn);
	//-------------------------------------------------------------------------
	virtual AContainer::Ptr getParent() const {
		return AContainer::Ptr();
	}
	//-------------------------------------------------------------------------
	void setSurface(ISurface::Ptr _surface);
	//-------------------------------------------------------------------------
	IDrawContext::Ptr getDrawContext() const;
	//-------------------------------------------------------------------------
	static Ptr create(ISurface::Ptr surface = ISurface::Ptr());
	//-------------------------------------------------------------------------
	ISurface::Ptr getSurface() const {
		return surface;
	}

}; // RootPane
}}} // namespace(s)

#endif /* SAMBAG_ROOTPANE_H */
