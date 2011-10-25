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
cairo_status_t read_handler (
		void *handler,
		unsigned char *data,
		unsigned int length)
{
	IDataHandler *ptr = static_cast<IDataHandler*>(handler);
	if (!ptr)
		return CAIRO_STATUS_NULL_POINTER;
	IDataHandler::StreamPtr is = ptr->getDataStream();
	if (!is)
		return CAIRO_STATUS_NULL_POINTER;
	is->read((char*)data, length);
	if (is->fail())
		return CAIRO_STATUS_READ_ERROR;
	return CAIRO_STATUS_SUCCESS;
}
} // namespace

namespace sambag { namespace disco {
//=============================================================================
//  Class CairoDrawContext:
//    implements IDrawContext with Cairos draw operations.
//=============================================================================
//-----------------------------------------------------------------------------
CairoDrawContext::CairoDrawContext(cairo_surface_t *surface)
: surface(surface), _isFilled(true),_isStroked(true), context(NULL)
{
	context = cairo_create(surface);
	setFont(currentFont);

}
//-----------------------------------------------------------------------------
CairoDrawContext::~CairoDrawContext() {
	cairo_destroy(context);
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
void CairoDrawContext::drawSurface(ISurface::Ptr _surface, const Number &alpha) {
	CairoSurface::Ptr surface = boost::shared_dynamic_cast<CairoSurface>(_surface);
	if (!_surface)
		return;
	cairo_surface_t *png = surface->getCairoSurface();
	if (!png)
		return;
	cairo_set_source_surface (context, png, 0, 0);
	if (alpha>=1.0) {
		cairo_paint (context);
	} else {
		cairo_paint_with_alpha(context, alpha);
	}
}
//-----------------------------------------------------------------------------
ISurface::Ptr CairoDrawContext::createPngSurface(IDataHandler::Ptr handler) {
	if (!handler)
		return ISurface::Ptr();
	cairo_surface_t * surface =
			cairo_image_surface_create_from_png_stream
				(&read_handler, handler.get());
	if (!surface)
		return ISurface::Ptr();
	CairoSurface::Ptr neu = CairoSurface::create(surface);
	return neu;
}

}} // namespaces
