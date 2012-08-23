/*
 * ComponentWrapper.hpp
 *
 *  Created on: Tue Aug 21 11:46:43 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_COMPONENTWRAPPER_H
#define SAMBAG_COMPONENTWRAPPER_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/svg/graphicElements/GraphicElement.hpp>
#include <sambag/disco/components/AComponent.hpp>
#include <sambag/disco/IDrawContext.hpp>
#include <sambag/disco/IDiscoFactory.hpp>

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
namespace sc = sambag::com;
namespace sd = sambag::disco;
namespace sdc = sd::components;
namespace sdcu = sdc::ui;
//=============================================================================
/** 
  * @class ComponentWrapper.
  * Wraps a disco::components::AComponent around a IDrawable object.
  */
template <class Drawable>
class ComponentWrapper : public sdc::AComponent {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ComponentWrapper> Ptr;
protected:
	//-------------------------------------------------------------------------
	/**
	 * overrides AComponents drawComponent()
	 */
	virtual void drawComponent(IDrawContext::Ptr context);
	//-------------------------------------------------------------------------
	ComponentWrapper();
private:
	//-------------------------------------------------------------------------
	typename Drawable::Ptr drawable;
public:
	//-------------------------------------------------------------------------
	/**
	 * sycronizes Drawable bounds with Component bounds.
	 */
	virtual void updateBounds();
	//-------------------------------------------------------------------------
	typename Drawable::Ptr getObject() const {
		return drawable;
	}
	//-------------------------------------------------------------------------
	static Ptr create(); 
}; // ComponentWrapper
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class Drawable>
void ComponentWrapper<Drawable>::updateBounds() 
{
	sd::IDrawContext::Ptr cn = sd::getDiscoFactory()->createContext();
	cn->setStrokeColor(ColorRGBA(0));
	setBounds(drawable->getBoundingBox(cn));
}
//-----------------------------------------------------------------------------
template <class Drawable>
void ComponentWrapper<Drawable>::drawComponent(IDrawContext::Ptr context) 
{
	sd::Point2D p(0, 0);
	boost::geometry::subtract_point(p, getLocation());
	context->translate(p);
	drawable->draw(context);
}
//-----------------------------------------------------------------------------
template <class Drawable>
typename ComponentWrapper<Drawable>::Ptr ComponentWrapper<Drawable>::create() 
{
	Ptr res( new ComponentWrapper() );
	res->self = res;
	res->postConstructor();
	return res;
}
//-----------------------------------------------------------------------------
template <class Drawable>
ComponentWrapper<Drawable>::ComponentWrapper() {
	drawable = typename Drawable::create();
}
}}}} // namespace(s)

#endif /* SAMBAG_COMPONENTWRAPPER_H */
