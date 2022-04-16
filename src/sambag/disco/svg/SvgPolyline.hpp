/*
 * Polyline.hpp
 *
 *  Created on: 09.10.2011
 *      Author: samba
 */

#ifndef DISCO_POLYLINE_HPP_
#define DISCO_POLYLINE_HPP_

#include "Svg.hpp"
#include "graphicElements/Path.hpp"
#include "graphicElements/PointContainer.hpp"
#include "sambag/com/Common.hpp"
#include "sambag/com/Helper.hpp"
#include <boost/tuple/tuple.hpp>

namespace sambag { namespace disco { namespace svg {

//=============================================================================
class SvgPolyline : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgPolyline> Ptr;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute tags
	//-------------------------------------------------------------------------
	struct Points_tag { typedef graphicElements::pathInstruction::PointContainer Type; };
private:
protected:
	//-------------------------------------------------------------------------
	sambag::disco::svg::graphicElements::Path::Ptr path;
	//-------------------------------------------------------------------------
	SvgPolyline() {
		path = sambag::disco::svg::graphicElements::Path::create();
	}
public:
	//-------------------------------------------------------------------------
	GraphicElement::Ptr getGraphicElement() const {
		return path;
	}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgPolyline());
		neu->__setSelf(neu);
		neu->createBase(root);
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgPolyline(){}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Attribute setter
	//-------------------------------------------------------------------------
	virtual void set( const Points_tag::Type &_v, Points_tag ) {
		using namespace sambag::disco::svg::graphicElements::pathInstruction;
		Points_tag::Type v = _v;
		Points_tag::Type p0;
		if (v.empty()) return;
		// make pathInstructions with points
		PathInstructions pI;
		// first point is a move_to
		Number x=0,y=0;
		boost::tie(x,y) =
				com::fromContainer<boost::tuple<Number, Number>, Points_tag::Type >(v);
		p0.push_back(x); p0.push_back(y);
		pI.push_back(std::make_pair(p0, MOVETO_ABS));
		// the rest
		pI.push_back(std::make_pair(v, LINETO_ABS));
		path->setPathInstructions(pI);
	}
	//-------------------------------------------------------------------------
	static void registerAttributes( SvgObject::BuilderType &binder ) {
		binder.registerAttribute<Points_tag::Type, Points_tag, SvgPolyline>("points");
	}
};

}}} // namespace

#endif /* POLYLINE_HPP_ */
