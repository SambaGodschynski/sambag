/*
 * PngSurface.hpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#ifndef PNGSURFACE_HPP_
#define PNGSURFACE_HPP_

#include "cairo.h"
#include "ISurface.hpp"
#include "sambag/com/IDataHandler.hpp"
#include "sambag/com/Exception.hpp"
#include <string>

namespace {
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
class CairoSurface : public ISurface {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoSurface> Ptr;
	//-------------------------------------------------------------------------
	SAMBAG_EXCEPTION_CLASS(SurfaceCreationFailed);
protected:
	//-------------------------------------------------------------------------
	cairo_surface_t * surface;
	//-------------------------------------------------------------------------
	CairoSurface(cairo_surface_t *s) : surface(s) {
		cairo_status_t stat = cairo_surface_status(s);
		if (stat!=CAIRO_STATUS_SUCCESS) {
			std::string what(cairo_status_to_string (stat));
			SAMBAG_THROW(SurfaceCreationFailed, what);
		}
	}
public:
	//-------------------------------------------------------------------------
	cairo_surface_t * getCairoSurface() const {
		return surface;
	}
	//-------------------------------------------------------------------------
	virtual ~CairoSurface() {
		if (!surface)
			return;
		cairo_surface_destroy(surface);
	}
	//-------------------------------------------------------------------------
	virtual Rectangle getSize() const = 0;
};
//=============================================================================
class CairoImageSurface : public CairoSurface, public IImageSurface {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoImageSurface> Ptr;
protected:
	//-------------------------------------------------------------------------
	Rectangle & getImageSurfaceRect(Rectangle &res) const {
		res.setWidth((Number)cairo_image_surface_get_width(surface));
		res.setHeight((Number)cairo_image_surface_get_height(surface));
		return res;
	}
	//-------------------------------------------------------------------------
	CairoImageSurface(cairo_surface_t *s) : CairoSurface(s) {}
public:
	//-------------------------------------------------------------------------
	static Ptr create(const Integer &width, const Integer &height) {
		cairo_surface_t *s =
				cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
		return Ptr (new CairoImageSurface(s));
	}
	//-------------------------------------------------------------------------
	static Ptr create(IDataHandler::Ptr handler) {
		if (!handler)
			return Ptr();
		cairo_surface_t * surface =
				cairo_image_surface_create_from_png_stream
					(&read_handler, handler.get());
		if (!surface)
			return Ptr();
		return Ptr (new CairoImageSurface(surface));
	}
	//-------------------------------------------------------------------------
	virtual void writeToFile(const std::string &filename) const {
		cairo_surface_write_to_png(surface, filename.c_str() );
	}
	//-------------------------------------------------------------------------
	virtual ~CairoImageSurface() {}
	//-------------------------------------------------------------------------
	virtual Rectangle getSize() const {
		Rectangle res(0,0,0,0);
		res.setWidth((Number)cairo_image_surface_get_width(surface));
		res.setHeight((Number)cairo_image_surface_get_height(surface));
		return res;
	}
	//-------------------------------------------------------------------------
};
//=============================================================================
class CairoRecordingSurface : public CairoSurface {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<CairoRecordingSurface> Ptr;
protected:
	//-------------------------------------------------------------------------
	CairoRecordingSurface(cairo_surface_t *s) : CairoSurface(s) {}
public:
	//-------------------------------------------------------------------------
	cairo_surface_t * getCairoSurface() const {
		return surface;
	}
	//-------------------------------------------------------------------------
	static Ptr create() {
		cairo_surface_t *s =
				cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
		return Ptr (new CairoRecordingSurface(s));
	}
	//-------------------------------------------------------------------------
	virtual ~CairoRecordingSurface() {}
	//-------------------------------------------------------------------------
	virtual Rectangle getSize() const {
		Number x, y, w, h;
		cairo_recording_surface_ink_extents (surface, &x, &y, &w, &h);
		return Rectangle(Point2D(x,y), w, h);
	}
	//-------------------------------------------------------------------------
};

}} // namespace

#endif /* PNGSURFACE_HPP_ */
