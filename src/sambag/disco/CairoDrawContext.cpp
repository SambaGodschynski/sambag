/*
 * CairoDrawContext.cpp
 *
 *  Created on: 11.09.2011
 *      Author: samba
 */

#include "CairoDrawContext.hpp"
#include "CairoDataHandler.hpp"

namespace sambag { namespace disco {
//=============================================================================
//  Class CairoDrawContext:
//    implements IDrawContext with Cairos draw operations.
//=============================================================================
//-----------------------------------------------------------------------------
CairoDrawContext::CairoDrawContext( cairo_surface_t *surface )
: surface(surface), context(NULL)
{
	context = cairo_create(surface);
	setFont(currentFont);

}
//-----------------------------------------------------------------------------
CairoDrawContext::~CairoDrawContext() {
	cairo_destroy(context);
}
//-----------------------------------------------------------------------------
void CairoDrawContext::rect( const Rectangle &rect, const Number &cornerRadius ) {
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
void CairoDrawContext::drawImage(IDataHandler& _handler) {
	CairoDataHandler *handler = dynamic_cast<CairoDataHandler*>(&_handler);
	if (!handler)
		return;
	cairo_surface_t *png =
			cairo_image_surface_create_from_png_stream (
				CairoDataHandler::getCairoReadFunction(), handler);
	if (!png)
		return;
	cairo_set_source_surface (context, png, 0, 0);
	cairo_paint (context);
	cairo_surface_destroy (png);
}

}} // namespaces
