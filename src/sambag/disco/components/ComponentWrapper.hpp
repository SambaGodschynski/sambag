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
#include <sambag/disco/svg/Style.hpp>
#include <sambag/disco/components/AComponent.hpp>
#include <sambag/disco/IDrawContext.hpp>
#include <sambag/disco/IDiscoFactory.hpp>

namespace sambag { namespace disco { namespace components {
namespace sds = svg;
namespace sdsg = sds::graphicElements;
//=============================================================================
/** 
  * @class ComponentWrapper.
  * Wraps a disco::components::AComponent around a IDrawable object.
  */
template <class Drawable>
class ComponentWrapper : public AComponent {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ComponentWrapper> Ptr;
	//-------------------------------------------------------------------------
	typedef ComponentWrapper<Drawable> Class;
	//-------------------------------------------------------------------------
	typedef AComponent Super;
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
	//-------------------------------------------------------------------------
	sds::Style style;
public:
	//-------------------------------------------------------------------------
	/**
	 * Overridden for performance reasons.
	 * See the <a href="#override">Implementation Note</a>
	 * for more information.
	 */
	virtual void validate() {}
	//-------------------------------------------------------------------------
	/**
	 * Overridden for performance reasons.
	 * See the <a href="#override">Implementation Note</a>
	 * for more information.
	 *
	 * @since 1.5
	 */
	virtual void invalidate() {}
	//-------------------------------------------------------------------------
	virtual void setStyle( const sds::Style &style ) { 
		this->style = style;
	}
	//-------------------------------------------------------------------------
	virtual const sds::Style & getStyle() const { 
		return style;
	}
	//-------------------------------------------------------------------------
	virtual sds::Style & getStyle() { 
		return style;
	}
	//-------------------------------------------------------------------------
	/**
	 * sycronizes Drawable bounds with Component bounds.
	 */
	virtual void updateBounds();
	//-------------------------------------------------------------------------
	/**
	 * @deprecated use getDrawable instead
	 */
	typename Drawable::Ptr getObject() const {
		return drawable;
	}
	//-------------------------------------------------------------------------
	typename Drawable::Ptr getDrawable() const {
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
	IDrawContext::Ptr cn = getDiscoFactory()->createContext();
	cn->setStrokeColor(ColorRGBA(0)); // without stroke, no bounding
	setBounds(drawable->getBoundingBox(cn));
}
//-----------------------------------------------------------------------------
template <class Drawable>
void ComponentWrapper<Drawable>::drawComponent(IDrawContext::Ptr context) 
{
	Point2D p(0, 0);
	boost::geometry::subtract_point(p, getLocation());
	context->translate(p);
	style.intoContext(context);
	drawable->draw(context);
}
//-----------------------------------------------------------------------------
template <class Drawable>
typename ComponentWrapper<Drawable>::Ptr ComponentWrapper<Drawable>::create() 
{
	Ptr res( new Class() );
	res->self = res;
	res->postConstructor();
	res->setName("Unnamed ComponentWrapper");
	return res;
}
//-----------------------------------------------------------------------------
template <class Drawable>
ComponentWrapper<Drawable>::ComponentWrapper() {
	drawable = Drawable::create();
}
}}} // namespace(s)

#endif /* SAMBAG_COMPONENTWRAPPER_H */
