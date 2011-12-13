/*
 * CairoDrawContext.cpp
 *
 *  Created on: 11.09.2011
 *      Author: samba
 */

#include "CairoDrawContext.hpp"
#include "CairoSurface.hpp"

namespace {
//-----------------------------------------------------------------------------
using namespace sambag::disco;
//-----------------------------------------------------------------------------
CairoPatternRef createPattern(SolidPattern::Ptr pt) {
	if (!pt)
		return CairoPatternRef();
	const ColorRGBA &col = pt->getSolidColor();
	CairoPatternRef neu = createPatternRef(
		cairo_pattern_create_rgba(col.getR(), col.getG(), col.getB(), col.getA())
	);
	return neu;
}
//-----------------------------------------------------------------------------
void copyGradient(const Gradient &src, cairo_pattern_t *dst) {
	boost_for_each( const Gradient::ColorStop &st, src.getStops() ) {
		ColorRGBA col;
		Number offset;
		boost::tie(col, offset) = st;
		cairo_pattern_add_color_stop_rgba(dst,
			offset,
			col.getR(),
			col.getG(),
			col.getB(),
			col.getA()
		);
	}
}
//-----------------------------------------------------------------------------
CairoPatternRef createPattern(LinearPattern::Ptr pt) {
	if (!pt)
		return CairoPatternRef();
	Point2D p0, p1;
	boost::tie(p0, p1) = pt->getLinearPoints();
	CairoPatternRef neu = createPatternRef(
		cairo_pattern_create_linear(p0.x(), p0.y(), p1.x(), p1.y())
	);
	copyGradient(*(pt.get()), neu.get());
	return neu;
}
//-----------------------------------------------------------------------------
CairoPatternRef createPattern(RadialPattern::Ptr pt) {
	if (!pt)
		return CairoPatternRef();
	Point2D c0, c1;
	Number r0, r1;
	boost::tie(c0, r0, c1, r1) = pt->getRadialCircles();
	CairoPatternRef neu = createPatternRef(
		cairo_pattern_create_radial(c0.x(), c0.y(), r0, c1.x(), c1.y(), r1)
	);
	copyGradient(*(pt.get()), neu.get());
	return neu;
}
//-----------------------------------------------------------------------------
CairoPatternRef createPattern(SurfacePattern::Ptr pt) {
	if (!pt)
		return CairoPatternRef();
	CairoSurface::Ptr surface =
		boost::shared_dynamic_cast<CairoSurface>(pt->getSurface());
	if (!surface)
		return CairoPatternRef();

	CairoPatternRef neu = createPatternRef(
		cairo_pattern_create_for_surface(surface->getCairoSurface())
	);
	return neu;
}
//-----------------------------------------------------------------------------
CairoPatternRef createPattern(APattern::Ptr pattern) {
	if (!pattern)
		return CairoPatternRef();
	CairoPatternRef pat;
	switch (pattern->getType()) {
		case APattern::SOLID :
			pat =
				createPattern( boost::shared_dynamic_cast<SolidPattern>(pattern) );
			break;
		case APattern::LINEAR :
			pat =
				createPattern( boost::shared_dynamic_cast<LinearPattern>(pattern) );
			break;
		case APattern::RADIAL :
			pat =
				createPattern( boost::shared_dynamic_cast<RadialPattern>(pattern) );
			break;
		case APattern::SURFACE :
			pat =
				createPattern( boost::shared_dynamic_cast<SurfacePattern>(pattern) );
			break;
	}
	cairo_matrix_t cm;
	CairoDrawContext::discoMatrixToCairoMatrix(pattern->getMatrix(), cm);
	cairo_pattern_set_matrix(pat.get(), &cm);
	return pat;
}

} // namespace

namespace sambag { namespace disco {
//-----------------------------------------------------------------------------
extern void destroyCairoPattern(cairo_pattern_t *p) {
	cairo_pattern_destroy(p);
}
//=============================================================================
//  Class CairoDrawContext:
//    implements IDrawContext with Cairos draw operations.
//=============================================================================
//-----------------------------------------------------------------------------
CairoDrawContext::CairoDrawContext(cairo_surface_t *surface)
: patternInUse(INVALID), surface(surface), context(NULL)
{
	context = cairo_create(surface);
	setFont(currentFont);

}
//-----------------------------------------------------------------------------
CairoDrawContext::CairoDrawContext(CairoSurface::Ptr _surface)
: patternInUse(INVALID), surfaceRef(_surface), context(NULL)
{
	surface = surfaceRef->getCairoSurface();
	context = cairo_create(surface);
	setFont(currentFont);

}
//-----------------------------------------------------------------------------
CairoDrawContext::~CairoDrawContext() {
	cairo_destroy(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::_save() {
	stack.push_back(
			StateInfo(strokePattern, fillPattern, currentFont, currentDash)
	);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::_restore() {
	if (stack.empty())
		return;
	boost::tie(strokePattern, fillPattern, currentFont, currentDash) = stack.back();
	stack.pop_back();
	patternInUse = INVALID;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::rect(const Rectangle &rect, const Number &cornerRadius) {
	Number radius = cornerRadius;
	Number degrees = M_PI / 180.0;

	Number width = rect.getWidth();
	Number height = rect.getHeight();
	Number x = rect.getX0().x();
	Number y = rect.getX0().y();

	cairo_new_sub_path (context);
	cairo_arc (context, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	cairo_arc (context, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	cairo_arc (context, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
	cairo_arc (context, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path (context);

}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFillPattern(APattern::Ptr pattern) {
	fillPattern = createPattern(pattern);
	patternInUse = INVALID;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setStrokePattern(APattern::Ptr pattern) {
	strokePattern = createPattern(pattern);
	patternInUse = INVALID;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::drawSurface(ISurface::Ptr _surface) {
	CairoSurface::Ptr surface = boost::shared_dynamic_cast<CairoSurface>(_surface);
	if (!_surface)
		return;
	cairo_surface_t *png = surface->getCairoSurface();
	if (!png)
		return;
	cairo_set_source_surface (context, png, 0, 0);
	//if (alpha>=1.0) {
		cairo_paint (context);
	//} else {
	//	cairo_paint_with_alpha(context, alpha);
	//}
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFillPattern() {
	if (!fillPattern)
		return;
	cairo_set_source(context, fillPattern.get());
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setStrokePattern() {
	if (!strokePattern)
		return;
	cairo_set_source(context, strokePattern.get());
}

}} // namespaces
