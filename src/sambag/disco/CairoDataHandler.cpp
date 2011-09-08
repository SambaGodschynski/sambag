/*
 * PngHandler.cpp
 *
 *  Created on: 11.10.2011
 *      Author: samba
 */

#include "CairoDataHandler.hpp"

namespace sambag { namespace disco {
//=============================================================================
// PngHandler
//=============================================================================
//-----------------------------------------------------------------------------
CairoDataHandler::CairoDataHandler(std::istream &pngStream) : is(NULL) {
	setDataInStream(pngStream);
}
//-----------------------------------------------------------------------------
CairoDataHandler::~CairoDataHandler() {
}
//-----------------------------------------------------------------------------
cairo_status_t CairoDataHandler::read_func (
		void *handler,
		unsigned char *data,
		unsigned int length)
{
	CairoDataHandler *ptr = static_cast<CairoDataHandler*>(handler);
	if (!ptr)
		return CAIRO_STATUS_NULL_POINTER;
	std::istream &is = *ptr->is;
	is.read((char*)data, length);
	if (is.fail())
		return CAIRO_STATUS_READ_ERROR;
	return CAIRO_STATUS_SUCCESS;
}

}} // namespace
