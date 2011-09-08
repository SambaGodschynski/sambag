/*
 * PngHandler.h
 *
 *  Created on: 11.10.2011
 *      Author: samba
 */

#ifndef DISCO_PNGHANDLER_H_
#define DISCO_PNGHANDLER_H_

#include "IDataHandler.hpp"
#include "cairo.h"

namespace sambag { namespace disco {

//=============================================================================
/**
 * A wrapper for: istream cairo data access and
 * cairo access via cairo_read_func_t.
 */
class CairoDataHandler : public IDataHandler {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	std::istream *is; // stream to png data
	//-------------------------------------------------------------------------
	/**
	 * cairo callback
	 * @param closure ptr to PngHandler
	 * @param data
	 * @param length
	 * @return
	 */
	static cairo_status_t read_func (void *handler, unsigned char *data, unsigned int length);
public:
	//-------------------------------------------------------------------------
	static cairo_read_func_t getCairoReadFunction()  {
		return &read_func;
	}
	//-------------------------------------------------------------------------
	/**
	 * @param pngStream
	 */
	CairoDataHandler(std::istream &pngStream);
	//-------------------------------------------------------------------------
	/**
	 * set data in stream.
	 * @param is
	 */
	virtual void setDataInStream(std::istream &_is) {
		is = &_is;
	}
	//-------------------------------------------------------------------------
	virtual ~CairoDataHandler();
};

}} // namespace

#endif /* PNGHANDLER_H_ */
