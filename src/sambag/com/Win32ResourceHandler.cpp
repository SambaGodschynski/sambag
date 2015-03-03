/*
 * Win32ResourceHandler.cpp
 *
 *  Created on: Thu Jan 17 10:02:44 2013
 *      Author: Johannes Unger
 */

#ifdef DISCO_USE_WIN32

#include "Win32ResourceHandler.hpp"
#include <sambag/com/exceptions/IllegalStateException.hpp>

namespace sambag { namespace com {
//=============================================================================
// Win32ResourceHandler
//=============================================================================
//-----------------------------------------------------------------------------
Win32ResourceHandler::Win32ResourceHandler(HINSTANCE hI, long resID) : 
	hI(hI),
	resID(resID) 
{
	is = loadResource(hI, resID);
	if (!is) {
		std::stringstream ss;
		DWORD err = GetLastError();
		ss<<"loading win32 reource: " << resID << " failed.("<<err<<")";
		SAMBAG_THROW(
			sambag::com::exceptions::IllegalStateException,
			ss.str()
		);
	}
}
//-----------------------------------------------------------------------------
Win32ResourceHandler::SStreamPtr 
Win32ResourceHandler::loadResource(HINSTANCE hI, long resID)
{
	SStreamPtr ss( 
		new std::stringstream(std::ios::in | std::ios::out | std::ios::binary) 
	);
	HRSRC res = FindResource(hI, MAKEINTRESOURCE(resID), "SAMBAG_CUSTOM" );
	if (!res) {
		return SStreamPtr();
	}
	HGLOBAL rH = LoadResource(hI, res);
	if (!rH) {
		return SStreamPtr();
	}
	char * cData = (char*)LockResource(rH);
	size_t size = SizeofResource(hI, res);
	if (size==0) {
		return SStreamPtr();
	}
	ss->write(cData, size);
	FreeResource(rH);
	return ss;
}
//-----------------------------------------------------------------------------
Win32ResourceHandler::~Win32ResourceHandler() {
}
//-----------------------------------------------------------------------------
Win32ResourceHandler::Ptr Win32ResourceHandler::create(HINSTANCE hI, long resID) 
{
	return Ptr( new Win32ResourceHandler(hI, resID) );
}


}} // namespace(s)

#endif //#ifdef DISCO_USE_WIN32