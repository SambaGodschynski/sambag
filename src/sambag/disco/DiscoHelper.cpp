/*
 * DiscoHelper.cpp
 *
 *  Created on: Sat Apr  6 15:16:56 2013
 *      Author: Johannes Unger
 */

#include "DiscoHelper.hpp"

namespace sambag { namespace disco {
//=============================================================================
// DiscoHelper
//=============================================================================
//-----------------------------------------------------------------------------
Rectangle getPatternRect(IPattern::Ptr p) {
	ILinearPattern *lin = dynamic_cast<ILinearPattern*>(p.get());
	if (lin) {
		const ILinearPattern::LinearPoints &p = lin->getLinearPoints();
		return Rectangle( boost::get<0>(p), boost::get<1>(p) );
	}
	IRadialPattern *rad = dynamic_cast<IRadialPattern*>(p.get());
	if (rad) {
		const IRadialPattern::RadialCircles &c = rad->getRadialCircles();
		const Point2D &p1 = boost::get<0>(c);
		Number rad1 = boost::get<1>(c);
		Rectangle r1(p1.x() - rad1/2., p1.y() - rad1/2., p1.x() + rad1, p1.y() + rad1);
		const Point2D &p2 = boost::get<2>(c);
		Number rad2 = boost::get<3>(c);
		Rectangle r2(p2.x() - rad2/2., p2.y() - rad2/2., p2.x() + rad2, p2.y() + rad2);
		Rectangle res = union_(r1, r2);
		return res;
	}
	ISurfacePattern *sfp = dynamic_cast<ISurfacePattern*>(p.get());
	if (sfp) {
		ISurface::Ptr sf = sfp->getSurface();
		if (sf) {
			return sf->getSize();
		}
	}
	return Rectangle();
}
//-----------------------------------------------------------------------------
void alignPattern(IDrawContext::Ptr cn, IPattern::Ptr p, const Rectangle &dst) {
	Rectangle pRect = getPatternRect(p);
	Number sx = pRect.width() == 0. ? 1. : pRect.width();
	Number sy = pRect.height() == 0. ? 1. : pRect.height();
	sx /= dst.width();
	sy /= dst.height();
	Matrix m = sambag::math::translate2D(pRect.x(), pRect.y());
	m = boost::numeric::ublas::prod(m, sambag::math::scale2D(sx, sy));
	m = boost::numeric::ublas::prod(m, translate2D(-dst.x(), -dst.y()));
	p->setMatrix(m);
}
}} // namespace(s)
