/*
 * CairoDrawContext.cpp
 *
 *  Created on: 11.09.2011
 *      Author: samba
 */

#include "CairoDrawContext.hpp"
#include "CairoSurface.hpp"
#include <sambag/com/Common.hpp>
#include "IDiscoFactory.hpp"


bool isFatalCairoStatus(cairo_status_t status) {
	if (status==CAIRO_STATUS_INVALID_STRING)
		return false;
	return true;
}

namespace sambag { namespace disco {
//=============================================================================
//  Class CairoDrawContext:
//    implements IDrawContext with Cairos draw operations.
//=============================================================================
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
	Number x = rect.x0().x();
	Number y = rect.x0().y();

	cairo_new_sub_path (context);
	cairo_arc (context, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	cairo_arc (context, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	cairo_arc (context, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
	cairo_arc (context, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path (context);

}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFillPattern(IPattern::Ptr pattern) {
	fillPattern = boost::shared_dynamic_cast<CairoPatternBase>(pattern);
	patternInUse = INVALID;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setStrokePattern(IPattern::Ptr pattern) {
	strokePattern = boost::shared_dynamic_cast<CairoPatternBase>(pattern);
	patternInUse = INVALID;
}
//-----------------------------------------------------------------------------
void CairoDrawContext::drawSurface(ISurface::Ptr _surface, Number opacity) {
	CairoSurface::Ptr surface = boost::shared_dynamic_cast<CairoSurface>(_surface);
	if (!_surface)
		return;
	cairo_surface_t *png = surface->getCairoSurface();
	if (!png)
		return;
	cairo_set_source_surface (context, png, 0, 0);
	if (opacity == 1.0) {
		cairo_paint (context);
	} else {
		cairo_paint_with_alpha(context, opacity);
	}
	// bug/curiosity:
	// color settings has to be reseted.
	setFillColor(getFillColor());
	setStrokeColor(getStrokeColor());
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setFillPattern() {
	if (!fillPattern)
		return;
	cairo_set_source(context, fillPattern->getCairoPatternRef().get());
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setStrokePattern() {
	if (!strokePattern)
		return;
	cairo_set_source(context, strokePattern->getCairoPatternRef().get());
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::setClip(const Rectangle &r) {
	rect(r);
	clip();
}
//-----------------------------------------------------------------------------
void CairoDrawContext::copyTo(IDrawContext::Ptr cn) const
{
	CairoDrawContext::Ptr dst =
		boost::shared_dynamic_cast<CairoDrawContext>(cn);
	SAMBAG_ASSERT(dst);
	cairo_set_source_surface (dst->getCairoContext(),
			surfaceRef->getCairoSurface(),
			0, 0);
	cairo_paint (dst->getCairoContext());
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::copyAreaTo(IDrawContext::Ptr cn,
		const Rectangle &src, const Point2D &dest) const
{
	CairoDrawContext::Ptr dstCn =
		boost::shared_dynamic_cast<CairoDrawContext>(cn);
	SAMBAG_ASSERT(dstCn);
	cairo_t *cr = dstCn->getCairoContext();
	cairo_set_source_surface (cr,
			surfaceRef->getCairoSurface(),
			dest.x() - src.x0().x(), dest.y() - src.x0().y());
	cairo_rectangle (cr, dest.x(), dest.y(), src.width(), src.height());
	cairo_fill (cr);
	SAMBAG_CHECK_CONTEXT_STATE(context);
}
}} // namespaces
