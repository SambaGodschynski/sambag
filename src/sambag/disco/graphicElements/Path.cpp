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
#include <boost/geometry/policies/compare.hpp>


namespace sambag { namespace disco { namespace graphicElements { namespace pathHelper {
extern boost::tuple<Number, Number, Number, Number, Number, Number> fromEndpointToCenter(
		Number x1, Number y1, Number x2, Number y2,
		Number rx, Number ry, Number phi,
		Number fA, Number fS);
}}}}


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
/**
 * Some commands needs information from a prev. command.
 * So this struct will be returned by every draw command and settted to
 * the following draw command as parameter.
 */
struct DrawReturn {
	Point2D prevPoint;
	pathInstruction::InstructionOp prevCommand;

	DrawReturn(
		pathInstruction::InstructionOp prevCommand = pathInstruction::NONE,
		const Point2D &p=com::NULL_POINT2D
	) : prevPoint(p),
		prevCommand(prevCommand) {}
};
//-----------------------------------------------------------------------------
/**
 *
 * @param cn
 * @param c
 * @param r DrawReturn: return val by prev. command
 * @return
 */
template <int OPC>
DrawReturn _draw ( IDrawContext::Ptr cn,
		           const pathInstruction::PointContainer &c,
		           const DrawReturn &previousReturned=DrawReturn())
{
	// wrong opcode
	return DrawReturn();
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::MOVETO_ABS>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	Number x = 0, y = 0;
	tie(x, y) = com::extractContainer<Tuple2, PointContainer>(c);
	cn->moveTo( Point2D(x,y) );
	return DrawReturn(pathInstruction::MOVETO_ABS);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::MOVETO_REL>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	Number x = 0, y = 0;
	tie(x, y) = com::extractContainer<Tuple2, PointContainer>(c);
	cn->relMoveTo( Point2D(x,y) );
	return DrawReturn(pathInstruction::MOVETO_REL);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::LINETO_ABS>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	while (it!=end) {
		Number x = 0, y = 0;
		tie(x, y) = com::extractContainer<Tuple2, PointContainer>(it, end);
		cn->lineTo( Point2D(x,y) );
	}
	return DrawReturn(pathInstruction::LINETO_ABS);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::LINETO_REL>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	while (it!=end) {
		Number x = 0, y = 0;
		tie(x, y) = com::extractContainer<Tuple2, PointContainer>(it, end);
		cn->relLineTo( Point2D(x,y) );
	}
	return DrawReturn(pathInstruction::LINETO_REL);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::CLOSEPATH>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	cn->closePath();
	return DrawReturn(pathInstruction::CLOSEPATH);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::LINETO_HORIZONTAL_ABS>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	while (it!=end) {
		Number x = 0;
		Point2D p = cn->getCurrentPoint();
		tie(x) = com::extractContainer<Tuple1, PointContainer>(it, end);
		p.x(x);
		cn->lineTo( p );
	}
	return DrawReturn(pathInstruction::LINETO_HORIZONTAL_ABS);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::LINETO_HORIZONTAL_REL>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	while (it!=end) {
		Number x = 0;
		tie(x) = com::extractContainer<Tuple1, PointContainer>(it, end);
		cn->relLineTo( Point2D(x,0) );
	}
	return DrawReturn(pathInstruction::LINETO_HORIZONTAL_REL);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::LINETO_VERTICAL_ABS>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	while (it!=end) {
		Number y = 0;
		Point2D p = cn->getCurrentPoint();
		tie(y) = com::extractContainer<Tuple1, PointContainer>(it, end);
		p.y(y);
		cn->lineTo( p );
	}
	return DrawReturn(pathInstruction::LINETO_VERTICAL_ABS);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::LINETO_VERTICAL_REL>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	while (it!=end) {
		Number y = 0;
		tie(y) = com::extractContainer<Tuple1, PointContainer>(it, end);
		cn->relLineTo( Point2D(0,y) );
	}
	return DrawReturn(pathInstruction::LINETO_VERTICAL_REL);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::CURVETO_CUBIC_ABS>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	Point2D cp2 = com::NULL_POINT2D;
	while (it!=end) {
		Number x1 = 0, y1 = 0, x2 = 0, y2 = 0, x = 0, y = 0;
		tie(x1,y1,x2,y2,x,y) =
				com::extractContainer<Tuple6, PointContainer>(it, end);
		cp2 = Point2D(x2, y2);
		cn->curveTo(Point2D(x1,y1), cp2, Point2D(x,y));
	}
	return DrawReturn(pathInstruction::CURVETO_CUBIC_ABS, cp2);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::CURVETO_CUBIC_REL>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	Point2D cp2 = com::NULL_POINT2D;
	while (it!=end) {
		Number x1 = 0, y1 = 0, x2 = 0, y2 = 0, x = 0, y = 0;
		tie(x1,y1,x2,y2,x,y) =
				com::extractContainer<Tuple6, PointContainer>(it, end);
		cp2 = Point2D(x2, y2);
		cn->relCurveTo(Point2D(x1,y1), cp2, Point2D(x,y));
	}
	return DrawReturn(pathInstruction::CURVETO_CUBIC_REL, cp2);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::CURVETO_CUBIC_SMOOTH_ABS>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	Point2D cp2 = com::NULL_POINT2D;
	while (it!=end) {
		Number x2 = 0, y2 = 0, x = 0, y = 0;
		Point2D curr = cn->getCurrentPoint();
		bool validPrevOp = pathInstruction::isCubic(r.prevCommand);
		Point2D x1 = boost::geometry::equals(r.prevPoint, com::NULL_POINT2D)
		             && validPrevOp ? curr : r.prevPoint;
		/*
		 *  http://www.w3.org/TR/2003/REC-SVG11-20030114/paths.html:
		 *  The first control point is assumed to be the reflection of the second control
		 *  point on the previous command relative to the current point. (If there is no previous
		 *  command or if the previous command was not an C, c, S or s, assume the first control point is
		 *  coincident with the current point.)
		 */
		x1 = Point2D( curr.x() - ( x1.x() - curr.x() ), curr.y() - ( x1.y() - curr.y() ) );
		tie(x2,y2,x,y) = com::extractContainer<Tuple4, PointContainer>(it, end);
		cp2 = Point2D(x2, y2);
		cn->curveTo(x1, cp2, Point2D(x,y));
	}
	return DrawReturn(pathInstruction::CURVETO_CUBIC_SMOOTH_ABS, cp2);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::CURVETO_CUBIC_SMOOTH_REL>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	Point2D cp2 = com::NULL_POINT2D;
	while (it!=end) {
		Number x2 = 0, y2 = 0, x = 0, y = 0;
		Point2D curr = cn->getCurrentPoint();
		bool validPrevOp = pathInstruction::isCubic(r.prevCommand);
		Point2D x1 = boost::geometry::equals(r.prevPoint, com::NULL_POINT2D)
				     && validPrevOp ? curr : r.prevPoint;
		/*
		 *  http://www.w3.org/TR/2003/REC-SVG11-20030114/paths.html:
		 *  The first control point is assumed to be the reflection of the second control
		 *  point on the previous command relative to the current point. (If there is no previous
		 *  command or if the previous command was not an C, c, S or s, assume the first control point is
		 *  coincident with the current point.)
		 */
		x1 = Point2D( curr.x() - ( x1.x() - curr.x() ), curr.y() - ( x1.y() - curr.y() ) );
		tie(x2,y2,x,y) = com::extractContainer<Tuple4, PointContainer>(it, end);
		cp2 = Point2D(x2, y2);
		cn->relCurveTo(x1, cp2, Point2D(x,y));
	}
	return DrawReturn(pathInstruction::CURVETO_CUBIC_SMOOTH_ABS, cp2);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::CURVETO_QUADRATIC_ABS>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	Point2D cp2 = com::NULL_POINT2D;
	while (it!=end) {
		Number x1 = 0, y1 = 0, x = 0, y = 0;
		Point2D x0 = cn->getCurrentPoint();
		tie(x1,y1,x,y) = com::extractContainer<Tuple4, PointContainer>(it, end);
		cp2 = Point2D(x1, y1);
		cn->quadraticCurveTo( Point2D(x1,y1), Point2D(x,y));
	}
	return DrawReturn(pathInstruction::CURVETO_QUADRATIC_ABS, cp2);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::CURVETO_QUADRATIC_REL>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	Point2D cp2 = com::NULL_POINT2D;
	while (it!=end) {
		Number x1 = 0, y1 = 0, x = 0, y = 0;
		Point2D x0 = cn->getCurrentPoint();
		tie(x1,y1,x,y) = com::extractContainer<Tuple4, PointContainer>(it, end);
		cp2 = Point2D(x1, y1);
		cn->relQuadraticCurveTo( Point2D(x1,y1), Point2D(x,y));
	}
	return DrawReturn(pathInstruction::CURVETO_QUADRATIC_REL, cp2);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::CURVETO_QUADRATIC_SMOOTH_ABS>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	Point2D x1 = com::NULL_POINT2D;
	while (it!=end) {
		Number x = 0, y = 0;
		Point2D curr = cn->getCurrentPoint();
		bool validPrevOp = pathInstruction::isQuadratic(r.prevCommand);
		x1 = boost::geometry::equals(r.prevPoint, com::NULL_POINT2D)
		             && validPrevOp ? curr : r.prevPoint;
		/*
		 *  http://www.w3.org/TR/2003/REC-SVG11-20030114/paths.html:
		 *  The control point is assumed to be the reflection of the control point
		 *  on the previous command relative to the current point.
		 *  (If there is no previous command or if the previous command was not a Q, q, T or t,
		 *   assume the control point is coincident with the current point.)
		 */
		x1 = Point2D( curr.x() - ( x1.x() - curr.x() ), curr.y() - ( x1.y() - curr.y() ) );
		tie(x,y) = com::extractContainer<Tuple2, PointContainer>(it, end);
		cn->quadraticCurveTo( x1, Point2D(x,y));
	}
	return DrawReturn(pathInstruction::CURVETO_CUBIC_SMOOTH_ABS, x1);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::CURVETO_QUADRATIC_SMOOTH_REL>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	PointContainer::const_iterator it = c.begin(), end=c.end();
	Point2D x1 = com::NULL_POINT2D;
	while (it!=end) {
		Number x = 0, y = 0;
		Point2D curr = cn->getCurrentPoint();
		bool validPrevOp = pathInstruction::isQuadratic(r.prevCommand);
		x1 = boost::geometry::equals(r.prevPoint, com::NULL_POINT2D)
		             && validPrevOp ? curr : r.prevPoint;
		/*
		 *  http://www.w3.org/TR/2003/REC-SVG11-20030114/paths.html:
		 *  The control point is assumed to be the reflection of the control point
		 *  on the previous command relative to the current point.
		 *  (If there is no previous command or if the previous command was not a Q, q, T or t,
		 *   assume the control point is coincident with the current point.)
		 */
		x1 = Point2D( curr.x() - ( x1.x() - curr.x() ), curr.y() - ( x1.y() - curr.y() ) );
		tie(x,y) = com::extractContainer<Tuple2, PointContainer>(it, end);
		cn->relQuadraticCurveTo( x1, Point2D(x,y));
	}
	return DrawReturn(pathInstruction::CURVETO_QUADRATIC_SMOOTH_REL, x1);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::ARC_ABS>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	using namespace sambag::disco::graphicElements::pathHelper;
	PointContainer::const_iterator it = c.begin(), end=c.end();
	while (it!=end) {
		Number rx=0, ry=0, x_axis_rot=0, arc_flag=0, sweep_flag=0, x=0, y=0;
		tie(rx,ry,x_axis_rot,arc_flag,sweep_flag,x,y) =
				com::extractContainer<Tuple7, PointContainer>(it, end);
		Point2D p0 = cn->getCurrentPoint();
		Number cx=0, cy=0, theta=0, delta=0;
		tie(cx, cy, rx, ry, theta, delta)	=
				fromEndpointToCenter(p0.x(), p0.y(), x, y, rx, ry,
						             x_axis_rot, arc_flag, sweep_flag);
		Number sA = theta * M_PI/180.0;
		Number eA = delta * M_PI/180.0;

		cn->save();

		cn->translate( Point2D(cx, cy) );
		cn->rotate(x_axis_rot * M_PI/180.0);
		cn->scale(Point2D(1.0, ry/rx));

		if (sweep_flag>=1.0)
			cn->arc(Point2D(0,0),rx, sA, eA + sA);
		else
			cn->arcNegative(Point2D(0,0),rx, sA, eA + sA);

		cn->restore();
	}
	return DrawReturn(pathInstruction::ARC_ABS);
}
//-----------------------------------------------------------------------------
template <>
DrawReturn _draw<pathInstruction::ARC_REL>
	( IDrawContext::Ptr cn, const PointContainer &c, const DrawReturn &r )
{
	using namespace sambag::disco::graphicElements::pathHelper;
	PointContainer::const_iterator it = c.begin(), end=c.end();
	while (it!=end) {
		Number rx=0, ry=0, x_axis_rot=0, arc_flag=0, sweep_flag=0, x=0, y=0;
		tie(rx,ry,x_axis_rot,arc_flag,sweep_flag,x,y) =
				com::extractContainer<Tuple7, PointContainer>(it, end);
		Point2D p0 = cn->getCurrentPoint();
		x+=p0.x(); y+=p0.y();
		Number cx=0, cy=0, theta=0, delta=0;
		tie(cx, cy, rx, ry, theta, delta)	=
				fromEndpointToCenter(p0.x(), p0.y(), x, y, rx, ry,
						             x_axis_rot, arc_flag, sweep_flag);
		Number sA = theta * M_PI/180.0;
		Number eA = delta * M_PI/180.0;

		cn->save();
		cn->translate( Point2D(cx, cy) );
		cn->rotate(x_axis_rot * M_PI/180.0);
		cn->scale(Point2D(1.0, ry/rx));

		if (sweep_flag>=1.0)
			cn->arc(Point2D(0,0),rx, sA, eA + sA);
		else
			cn->arcNegative(Point2D(0,0),rx, sA, eA + sA);

		cn->restore();
	}
	return DrawReturn(pathInstruction::ARC_REL);
}
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
	if ( style.isFilled() ) {
		style.setFillStyle(cn);
		drawPath(cn);
		cn->fill();
	}
	if ( style.isStroked() ) {
		style.setStrokeStyle(cn);
		drawPath(cn);
		cn->stroke();
	}
}
//-----------------------------------------------------------------------------
void Path::drawInstructions( IDrawContext::Ptr cn ) {
	if (pathInstructions.empty())
		return;
	DrawReturn ret;
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
			ret = _draw<CURVETO_CUBIC_ABS>( cn, pI.first );
			continue;
		case CURVETO_CUBIC_REL            : //................................
			ret = _draw<CURVETO_CUBIC_REL>( cn, pI.first );
			continue;
		case CURVETO_QUADRATIC_ABS        : //................................
			ret = _draw<CURVETO_QUADRATIC_ABS>( cn, pI.first );
			continue;
		case CURVETO_QUADRATIC_REL        : //................................
			ret = _draw<CURVETO_QUADRATIC_REL>( cn, pI.first );
			continue;
		case ARC_ABS                      : //................................
			ret = _draw<ARC_ABS>( cn, pI.first );
			continue;
		case ARC_REL                      : //................................
			ret = _draw<ARC_REL>( cn, pI.first );
			continue;
		case LINETO_HORIZONTAL_ABS        : //................................
			_draw<LINETO_HORIZONTAL_ABS>( cn, pI.first );
			continue;
		case LINETO_HORIZONTAL_REL        : //................................
			_draw<LINETO_HORIZONTAL_REL>( cn, pI.first );
			continue;
		case LINETO_VERTICAL_ABS          : //................................
			_draw<LINETO_VERTICAL_ABS>( cn, pI.first );
			continue;
		case LINETO_VERTICAL_REL          : //................................
			_draw<LINETO_VERTICAL_REL>( cn, pI.first );
			continue;
		case CURVETO_CUBIC_SMOOTH_ABS     : //................................
			ret = _draw<CURVETO_CUBIC_SMOOTH_ABS>( cn, pI.first, ret );
			continue;
		case CURVETO_CUBIC_SMOOTH_REL     : //................................
			ret = _draw<CURVETO_CUBIC_SMOOTH_REL>( cn, pI.first, ret );
			continue;
		case CURVETO_QUADRATIC_SMOOTH_ABS : //................................
			ret = _draw<CURVETO_QUADRATIC_SMOOTH_ABS>( cn, pI.first, ret );
			continue;
		case CURVETO_QUADRATIC_SMOOTH_REL : //................................
			ret = _draw<CURVETO_QUADRATIC_SMOOTH_REL>( cn, pI.first, ret );
			continue;
		case NONE						  : //................................
		default							  :
			continue;
		}
	}
}

}}}
