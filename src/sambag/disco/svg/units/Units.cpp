/*
 * Units
 *
 *  Created on: Thu Jul 26 12:18:40 2012
 *      Author: Johannes Unger
 */

#include "Units.hpp"
#include <sambag/com/Common.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>

namespace sambag { namespace disco { namespace svg { namespace units {
//=============================================================================
//  Class Unit
//=============================================================================
//-----------------------------------------------------------------------------
sambag::disco::Coordinate
Unit::solve(IDrawContext::Ptr cn, Coordinate absolute) const
{
	if (!cn && absolute==NULL_NUMBER) {
		return NULL_NUMBER;
	}
	switch (type) {
	case NONE:
		return 0;
	case UnitPX:
		return value;
	case UnitIN: //	"1in" equals "90px"
		return value * 90.;
	case UnitCM: //	"1cm" equals "35.43307px"
		return value * 35.43307;
	case UnitMM: //	"1mm" would be "3.543307px"
		return value * 3.543307;
	case UnitPT: //	"1pt" equals "1.25px"
		return value * 1.25;
	case UnitPC: //	"1pc" equals "15px"
		return value * 15.;
	case UnitEM:
		SAMBA_LOG_NOT_YET_IMPL();
		break;
	case UnitEX:
		SAMBA_LOG_NOT_YET_IMPL();
		break;
	case UnitPERCENT: {
		if (absolute == NULL_NUMBER) {
			// For any other length value expressed as a percentage of the
			// viewport, the percentage is calculated as the specified
			// percentage of
			// sqrt((actual-width)**2 + (actual-height)**2))/sqrt(2).
			// see http://www.w3.org/TR/SVG/coords.html#Units
			sambag::disco::Rectangle clip = cn->clipExtends();
			Coordinate w = pow(clip.width(), 2);
			Coordinate h = pow(clip.height(), 2);
			absolute = sqrt(w + h) / 1.4142135623730951;
		}
		return absolute * value / 100.;
	}
	}
	return 0;
}
//=============================================================================
// Point
//=============================================================================
//-----------------------------------------------------------------------------
sambag::disco::Point2D Point::solve(IDrawContext::Ptr cn) const {
//	//TODO: transf. calc wrong!
//	namespace trans = boost::geometry::strategy::transform;
	typedef sambag::disco::Point2D P;
	typedef sambag::disco::Rectangle R;
	R clip = cn->clipExtends();
//	// consider transformation
//	Matrix m = IDENTITY_MATRIX;
//	cn->getMatrix(m);
//	trans::ublas_transformer<P, P, 2, 2> matrixTransf(m);
//	boost::geometry::transform(clip.x0(), clip.x0(), matrixTransf);
//	boost::geometry::transform(clip.x1(), clip.x1(), matrixTransf);
	return P(
		_x.solve(cn, clip.width()),
		_y.solve(cn, clip.height())
	);
}
//=============================================================================
// Dimension
//=============================================================================
//-----------------------------------------------------------------------------
sambag::disco::Dimension Dimension::solve(IDrawContext::Ptr cn) const {
	sambag::disco::Point2D p = dimension.solve(cn);
	return sambag::disco::Dimension(p.x(), p.y());
}
//=============================================================================
// Rectangle
//=============================================================================
//-----------------------------------------------------------------------------
sambag::disco::Rectangle Rectangle::solve(IDrawContext::Ptr cn) const {
	sambag::disco::Dimension size = _size.solve(cn);
	return sambag::disco::Rectangle(_x0.solve(cn), size.width(), size.height());
}

}}}} // namespace(s)
