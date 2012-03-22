/*
 * IDataHandler.hpp
 *
 *  Created on: 11.10.2011
 *      Author: samba
 */

#ifndef DISCO_IDATAHANDLER_HPP_
#define DISCO_IDATAHANDLER_HPP_

#include <istream>
#include <boost/shared_ptr.hpp>

namespace sambag { namespace com {
//=============================================================================
/**
 * Interface for data handler which is a wrapper for a std::istream
 */
class IDataHandler {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IDataHandler> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<std::istream> StreamPtr;
	//-------------------------------------------------------------------------
	virtual StreamPtr getDataStream() = 0;
};

}} // namespace

#endif /* IFILEHANDLER_HPP_ */
