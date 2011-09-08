/*
 * FileHandler.h
 *
 *  Created on: 11.10.2011
 *      Author: samba
 */

#ifndef DISCO_FILEHANDLER_H_
#define DISCO_FILEHANDLER_H_

#include "IDataHandler.hpp"
#include "cairo.h"
#include <fstream>

namespace sambag { namespace com {

//=============================================================================
class FileHandler : public IDataHandler {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<FileHandler> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<std::fstream> FStreamPtr;
private:
	//-------------------------------------------------------------------------
	StreamPtr is; // stream to data
protected:
	//-------------------------------------------------------------------------
	/**
	 * @param stream
	 */
	FileHandler(StreamPtr stream);
public:
	//-------------------------------------------------------------------------
	static Ptr create(const std::string &location);
	//-------------------------------------------------------------------------
	static Ptr create(StreamPtr stream) {
		return Ptr(new FileHandler(stream));
	}
	//-------------------------------------------------------------------------
	StreamPtr getDataStream() {
		return is;
	}
	//-------------------------------------------------------------------------
	virtual ~FileHandler();
};

}} // namespace

#endif /* PNGHANDLER_H_ */
