/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef SVG_GRAPHICELEMENT_HPP_
#define SVG_GRAPHICELEMENT_HPP_


#include "sambag/com/Common.hpp"
#include "sambag/disco/graphicElements/GraphicElement.hpp"
#include "sambag/xml/XML2Obj.hpp"
#include <list>

namespace sambag { namespace disco { namespace svg {

//=============================================================================
// SVG version of Graphic Element.
class SvgObject : public sambag::disco::graphicElements::GraphicElement {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef sambag::xml::XML2Object<SvgObject> BuilderType;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgObject> Ptr;
	//-------------------------------------------------------------------------
	typedef std::list<SvgObject::Ptr> SvgObjects;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct StrokeWidth_tag { typedef Number Type; };
	//-------------------------------------------------------------------------
	// TODO: more style tags

private:
protected:
	SvgObject(){}
public:
	//-------------------------------------------------------------------------
	virtual void add(Ptr obj) {}
	//-------------------------------------------------------------------------
	virtual ~SvgObject(){}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context ) = 0;
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const  = 0;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const StrokeWidth_tag::Type &width, StrokeWidth_tag ) {
		style.stokeWidth = width;
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute
			<StrokeWidth_tag::Type, StrokeWidth_tag, SvgObject>
				("stroke-width");

		// TODO: register more style tags
	}
};

//=============================================================================
class SvgRoot : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef sambag::xml::XML2Object<SvgObject> BuilderType;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgRoot> Ptr;
private:
	//-------------------------------------------------------------------------
	SvgObjects svgObjects;
protected:
	SvgRoot(){}
public:
	//-------------------------------------------------------------------------
	size_t getNumChildren() const { return svgObjects.size(); }
	//-------------------------------------------------------------------------
	virtual void add(SvgObject::Ptr obj) {
		svgObjects.push_back(obj);
	}
	//-------------------------------------------------------------------------
	virtual ~SvgRoot(){}
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const  { return Rectangle(); }
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new SvgRoot());
		neu->__setSelf(neu);
		return neu;
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>tagged attribute methods
	//-------------------------------------------------------------------------
};


}}} // namespace

#endif /* SVGGRAPHICELEMENT_HPP_ */
