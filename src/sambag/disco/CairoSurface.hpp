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
	CairoSurface(cairo_surface_t *s);
public:
	//-------------------------------------------------------------------------
	cairo_surface_t * getCairoSurface() const {
		return surface;
	}
	//-------------------------------------------------------------------------
	/**
	 * default impl. 
	 * @return getSize();
	 */
	virtual Rectangle getClipRect() const {
		return getSize();
	}
	//-------------------------------------------------------------------------
	virtual ~CairoSurface();
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
	Rectangle & getImageSurfaceRect(Rectangle &res) const;
	//-------------------------------------------------------------------------
	CairoImageSurface(cairo_surface_t *s) : CairoSurface(s) {}
public:
	//-------------------------------------------------------------------------
	static Ptr create(const Integer &width, const Integer &height);
	//-------------------------------------------------------------------------
	static Ptr create(IDataHandler::Ptr handler);
	//-------------------------------------------------------------------------
	virtual void writeToFile(const std::string &filename) const;
	//-------------------------------------------------------------------------
	virtual ~CairoImageSurface() {}
	//-------------------------------------------------------------------------
	virtual Rectangle getSize() const;
	//-------------------------------------------------------------------------
	/**
	 * default impl. 
	 * @return getSize();
	 */
	virtual Rectangle getClipRect() const {
		return getSize();
	}
};
//=============================================================================
class CairoRecordingSurface : public CairoSurface, public IRecordingSurface {
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
	static Ptr create();
	//-------------------------------------------------------------------------
	static Ptr create(const Integer &width, const Integer &height);
	//-------------------------------------------------------------------------
	virtual ~CairoRecordingSurface() {}
	//-------------------------------------------------------------------------
	virtual Rectangle getSize() const;
	//-------------------------------------------------------------------------
	/**
	 * default impl. 
	 * @return getSize();
	 */
	virtual Rectangle getClipRect() const {
		return getSize();
	}
};

}} // namespace

#endif /* PNGSURFACE_HPP_ */
