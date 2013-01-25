#include "CairoSurface.hpp"
#include "CreateAppropriateCairoSurface.hpp"

namespace sambag { namespace disco {
//=============================================================================
// class CairoSurface 
//=============================================================================
//-----------------------------------------------------------------------------
CairoSurface::CairoSurface(cairo_surface_t *s) : surface(s) {
	cairo_status_t stat = cairo_surface_status(s);
	if (stat!=CAIRO_STATUS_SUCCESS) {
		std::string what(cairo_status_to_string (stat));
		SAMBAG_THROW(SurfaceCreationFailed, what);
	}
}
//-----------------------------------------------------------------------------
CairoSurface::~CairoSurface() {
	if (!surface)
		return;
	cairo_surface_flush(surface); 
	cairo_surface_finish(surface);
	cairo_surface_destroy(surface);
}

//=============================================================================
// class CairoImageSurface 
//=============================================================================
//-----------------------------------------------------------------------------
Rectangle & CairoImageSurface::getImageSurfaceRect(Rectangle &res) const {
	res.setWidth((Number)cairo_image_surface_get_width(surface));
	res.setHeight((Number)cairo_image_surface_get_height(surface));
	return res;
}
//-----------------------------------------------------------------------------
CairoImageSurface::Ptr CairoImageSurface::create(const Integer &width, 
														const Integer &height) 
{
	cairo_surface_t *s = createAppropriateCairoImageSurface(width, height);
	return Ptr (new CairoImageSurface(s));
}
//-----------------------------------------------------------------------------
CairoImageSurface::Ptr CairoImageSurface::create(IDataHandler::Ptr handler) 
{
	if (!handler)
		return Ptr();
	cairo_surface_t * surface =
			cairo_image_surface_create_from_png_stream
				(&read_handler, handler.get());
	if (!surface)
		return Ptr();
	return Ptr (new CairoImageSurface(surface));
}
//-----------------------------------------------------------------------------
void CairoImageSurface::writeToFile(const std::string &filename) const {
	cairo_surface_write_to_png(surface, filename.c_str() );
}
//-----------------------------------------------------------------------------
Rectangle CairoImageSurface::getSize() const {
	Rectangle res(0,0,0,0);
	res.setWidth((Number)cairo_image_surface_get_width(surface));
	res.setHeight((Number)cairo_image_surface_get_height(surface));
	return res;
}
//=============================================================================
// class CairoRecordingSurface
//=============================================================================
//-----------------------------------------------------------------------------
CairoRecordingSurface::Ptr CairoRecordingSurface::create() {
	cairo_surface_t *s =
			cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
	return Ptr (new CairoRecordingSurface(s));
}
//-----------------------------------------------------------------------------
CairoRecordingSurface::Ptr CairoRecordingSurface::create(const Integer &width, 
	const Integer &height)
{
	cairo_rectangle_t r = {0};
	r.width = width;
	r.height = height;
	cairo_surface_t *s =
			cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, &r);
	return Ptr (new CairoRecordingSurface(s));
}
//-----------------------------------------------------------------------------
Rectangle CairoRecordingSurface::getSize() const {
	Number x, y, w, h;
	cairo_recording_surface_ink_extents (surface, &x, &y, &w, &h);
	return Rectangle(Point2D(x,y), w, h);
}
}} // namespace