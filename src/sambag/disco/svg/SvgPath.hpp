/*
 * Path.hpp
 *
 *  Created on: 27.09.2011
 *      Author: samba
 */

#ifndef DISCO_SVG_PATH_HPP_
#define DISCO_SVG_PATH_HPP_

#include "Svg.hpp"
#include "sambag/disco/graphicElements/Path.hpp"


namespace sambag { namespace disco { namespace svg {
using sambag::disco::graphicElements::pathInstruction::PathInstructions;
//=============================================================================
class SvgPath : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgPath> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct D_tag { typedef PathInstructions Type; };
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::graphicElements::Path::Ptr path;
	//-------------------------------------------------------------------------
	SvgPath();
public:
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getDrawingObject() const {
		return path;
	}
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new SvgPath());
		neu->__setSelf(neu);
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgPath();
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context );
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const {
		if (!path) return Rectangle();
		return path->getBoundingBox();
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const D_tag::Type &v, D_tag ) {
		path->setPathInstructions(v);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<D_tag::Type, D_tag, SvgPath>("d");
	}
};

}}} // namespace


#endif /* PATH_HPP_ */
