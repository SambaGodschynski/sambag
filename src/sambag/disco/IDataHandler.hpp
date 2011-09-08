/*
 * IFileHandler.hpp
 *
 *  Created on: 11.10.2011
 *      Author: samba
 */

#ifndef DISCO_IDATAHANDLER_HPP_
#define DISCO_IDATAHANDLER_HPP_

#include <istream>

namespace sambag { namespace disco {
//=============================================================================
/**
 * Interface for data handler.
 */
class IDataHandler {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	/**
	 * set data in stream.
	 * @param is
	 */
	virtual void setDataInStream(std::istream &is) = 0;
};

}} // namespace

#endif /* IFILEHANDLER_HPP_ */
