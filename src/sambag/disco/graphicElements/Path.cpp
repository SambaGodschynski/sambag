/*
 * Path.cpp
 *
 *  Created on: 27.09.2011
 *      Author: samba
 */

#include "Path.hpp"
#include "sambag/com/Common.hpp"
#include <boost/tuple/tuple.hpp>
#include "sambag/com/Helper.hpp"

namespace { // path impl.
using namespace sambag;
using namespace sambag::disco;
using namespace sambag::disco::graphicElements;
using pathInstruction::PointContainer;
using namespace boost;
//-----------------------------------------------------------------------------
typedef boost::tuple<Number> Tuple1;
typedef boost::tuple<Number, Number> Tuple2;
typedef boost::tuple<Number, Number, Number> Tuple3;
typedef boost::tuple<Number, Number, Number, Number> Tuple4;
typedef boost::tuple<Number, Number, Number, Number, Number> Tuple5;
typedef boost::tuple<Number, Number, Number, Number, Number, Number> Tuple6;
typedef boost::tuple<Number, Number, Number, Number, Number, Number, Number> Tuple7;
//-----------------------------------------------------------------------------
template <int OPC>
void _draw( IDrawContext::Ptr cn, const pathInstruction::PointContainer &c ) {
	// wrong opcode
}
//-----------------------------------------------------------------------------
template <>
void _draw<pathInstruction::MOVETO_ABS>
	( IDrawContext::Ptr cn, const PointContainer &c )
{
	Number x = 0, y = 0;
	tie(x, y) = com::extractContainer<Tuple2, PointContainer>(c);
	cn->moveTo( Point2D(x,y) );
}
//-----------------------------------------------------------------------------
template <>
void _draw<pathInstruction::MOVETO_REL>
	( IDrawContext::Ptr cn, const PointContainer &c )
{
	Number x = 0, y = 0;
	tie(x, y) = com::extractContainer<Tuple2, PointContainer>(c);
	cn->relMoveTo( Point2D(x,y) );
}
//-----------------------------------------------------------------------------
template <>
void _draw<pathInstruction::LINETO_ABS>
	( IDrawContext::Ptr cn, const PointContainer &c )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	while (it!=end) {
		Number x = 0, y = 0;
		tie(x, y) = com::extractContainer<Tuple2, PointContainer>(it, end);
		cn->lineTo( Point2D(x,y) );
	}
}
//-----------------------------------------------------------------------------
template <>
void _draw<pathInstruction::LINETO_REL>
	( IDrawContext::Ptr cn, const PointContainer &c )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	while (it!=end) {
		Number x = 0, y = 0;
		tie(x, y) = com::extractContainer<Tuple2, PointContainer>(it, end);
		cn->lineTo( Point2D(x,y) );
	}
}
//-----------------------------------------------------------------------------
template <>
void _draw<pathInstruction::CLOSEPATH>
	( IDrawContext::Ptr cn, const PointContainer &c )
{
	cn->closePath();
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


namespace sambag { namespace disco { namespace graphicElements {

//=============================================================================
// class Path
//=============================================================================
//-----------------------------------------------------------------------------
Path::Path()  {
}
//-----------------------------------------------------------------------------
Path::~Path() {
}
//-----------------------------------------------------------------------------
void Path::drawPath( IDrawContext::Ptr cn ) {
	if (storedPath) {
		cn->appendPath(storedPath);
		return;
	}
	drawInstructions(cn);
	storedPath = cn->copyPath();
}
//-----------------------------------------------------------------------------
void Path::draw( IDrawContext::Ptr cn ) {
	if ( style.isStroked() ) {
		style.setStrokeStyle(cn);
		drawPath(cn);
		cn->stroke();
	}
	if ( !style.isStroked() ) {
		style.setFillStyle(cn);
		drawPath(cn);
		cn->fill();
	}
}
//-----------------------------------------------------------------------------
void Path::drawInstructions( IDrawContext::Ptr cn ) {
	if (pathInstructions.empty())
		return;
	for_each( const pathInstruction::PathInstruction &pI, pathInstructions ) {
		switch (pI.second) {
		using namespace pathInstruction;
		case CLOSEPATH                    : //................................
			_draw<CLOSEPATH>( cn, pI.first );
			continue;
		case MOVETO_ABS                   : //................................
			_draw<MOVETO_ABS>( cn, pI.first );
			continue;
		case MOVETO_REL                   : //................................
			_draw<MOVETO_REL>( cn, pI.first );
			continue;
		case LINETO_ABS                   : //................................
			_draw<LINETO_ABS>( cn, pI.first );
			continue;
		case LINETO_REL                   : //................................
			_draw<LINETO_REL>( cn, pI.first );
			continue;
		case CURVETO_CUBIC_ABS            : //................................
			continue;
		case CURVETO_CUBIC_REL            : //................................
			continue;
		case CURVETO_QUADRATIC_ABS        : //................................
			continue;
		case CURVETO_QUADRATIC_REL        : //................................
			continue;
		case ARC_ABS                      : //................................
			continue;
		case ARC_REL                      : //................................
			continue;
		case LINETO_HORIZONTAL_ABS        : //................................
			continue;
		case LINETO_HORIZONTAL_REL        : //................................
			continue;
		case LINETO_VERTICAL_ABS          : //................................
			continue;
		case LINETO_VERTICAL_REL          : //................................
			continue;
		case CURVETO_CUBIC_SMOOTH_ABS     : //................................
			continue;
		case CURVETO_CUBIC_SMOOTH_REL     : //................................
			continue;
		case CURVETO_QUADRATIC_SMOOTH_ABS : //................................
			continue;
		case CURVETO_QUADRATIC_SMOOTH_REL : //................................
			continue;
		case NONE						  : //................................
		default							  :
			continue;
		}
	}
}

}}}
