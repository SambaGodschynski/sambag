/*
 * Win32ResourceHandler.hpp
 *
 *  Created on: Thu Jan 17 10:02:44 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WIN32RESOURCEHANDLER_H
#define SAMBAG_WIN32RESOURCEHANDLER_H

#ifdef DISCO_USE_WIN32

#include <boost/shared_ptr.hpp>
#include "IDataHandler.hpp"
#include <sstream>
#include <windows.h>

namespace sambag { namespace com {
//=============================================================================
/** 
  * @class Win32ResourceHandler.
  * Handler to an Win32 Reource with type RT_RCDATA.
  */
class Win32ResourceHandler : public IDataHandler {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Win32ResourceHandler> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<std::stringstream> SStreamPtr;
private:
	//-------------------------------------------------------------------------
	HINSTANCE hI;
	//-------------------------------------------------------------------------
	long resID;
	//-------------------------------------------------------------------------
	SStreamPtr is; // stream to data
protected:
	//-------------------------------------------------------------------------
	Win32ResourceHandler(HINSTANCE hI, long resID);
	//-------------------------------------------------------------------------
	SStreamPtr loadResource(HINSTANCE hI, long resID);
public:
	//-------------------------------------------------------------------------
	/**
	 * @param stream the win32 api resource id
	 * @note can only load resources wich are defined in .rc file as "SAMBAG_CUSTOM"
	 */
	static Ptr create(HINSTANCE hI, long resID);
	//-------------------------------------------------------------------------
	StreamPtr getDataStream() {
		return is;
	}
	//-------------------------------------------------------------------------
	SStreamPtr getDataStringStream() {
		return is;
	}
	//-------------------------------------------------------------------------
	virtual ~Win32ResourceHandler();
}; // Win32ResourceHandler
}} // namespace(s)


#endif //#ifdef DISCO_USE_WIN32
#endif /* SAMBAG_WIN32RESOURCEHANDLER_H */
