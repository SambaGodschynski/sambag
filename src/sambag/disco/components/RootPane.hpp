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
#include <boost/unordered_map.hpp>

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
  * @class RootPane.
  */
class RootPane : public AContainer {
//=============================================================================
friend class ui::UIManager;
friend class sambag::disco::components::Window;
public:
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<RootPane> Ptr;
	//-------------------------------------------------------------------------
	static const std::string PROPERTY_SURFACE;
protected:
	//-------------------------------------------------------------------------
	virtual void postConstructor();
	//-------------------------------------------------------------------------
	ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const {
		return ui::AComponentUIPtr();
	}
	//-------------------------------------------------------------------------
	RootPane(ISurface::Ptr surface);
private:
	//-------------------------------------------------------------------------
	class RootLayout;
	//-------------------------------------------------------------------------
	Panel::Ptr createContentPane();
	//-------------------------------------------------------------------------
	Panel::Ptr contentPane;
	//-------------------------------------------------------------------------
	typedef boost::unordered_multimap<std::string, AComponentPtr> TagMap;
	//-------------------------------------------------------------------------
	TagMap tagMap;
	//-------------------------------------------------------------------------
	ISurface::Ptr surface;
public:
	//-------------------------------------------------------------------------
	virtual void setContentPane(Panel::Ptr c);
	//-------------------------------------------------------------------------
	virtual Panel::Ptr getContentPane() const;
	//-------------------------------------------------------------------------
	/**
	 * adds tag, component relation.
	 * @note case sensitive
	 * @note dosen't check whether component is contained by container
	 * @param comp
	 * @param tag
	 * @see getComponentsByTag
	 */
	void addTag(AComponent::Ptr comp, const std::string &tag);
	//-------------------------------------------------------------------------
	/**
	 * returns components which are related to tag
	 * @note case sensitive
	 * @param tag
	 * @param out
	 */
	template <typename Container>
	void getComponentsByTag(const std::string &tag, Container &out) const;
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return boost::dynamic_pointer_cast<RootPane>(AComponent::getPtr());
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
	void processMouseEvent(const events::MouseEvent &ev);
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
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <typename Container>
void RootPane::getComponentsByTag(const std::string &tag, Container &out)
const
{
	TagMap::const_iterator it, end;
	boost::tie(it, end) = tagMap.equal_range(tag);
	for (; it!=end; ++it) {
		out.push_back(it->second);
	}
}
}}} // namespace(s)

#endif /* SAMBAG_ROOTPANE_H */
