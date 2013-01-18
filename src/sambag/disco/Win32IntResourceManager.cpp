/*
 * Win32IntResourceManager.cpp
 *
 *  Created on: Thu Jan 17 11:39:19 2013
 *      Author: Johannes Unger
 */

#ifdef DISCO_USE_WIN32

#include "Win32IntResourceManager.hpp"
#include <sambag/com/Win32ResourceHandler.hpp>
#include "IDiscoFactory.hpp"
#include <sambag/com/exceptions/IllegalStateException.hpp>
namespace sambag { namespace disco {
//=============================================================================
//  Class Win32IntResourceManager
//=============================================================================
//-----------------------------------------------------------------------------
void Win32IntResourceManager::checkResourceAccess() {
	if (!hI) {
		SAMBAG_THROW(
			sambag::com::exceptions::IllegalStateException,
			"no HINSTANCE set."
		);
	}
}
//-----------------------------------------------------------------------------
Win32IntResourceManager & Win32IntResourceManager::instance() {
	typedef Loki::SingletonHolder<Win32IntResourceManager> FactoryHolder;
	return FactoryHolder::Instance();
}
//-----------------------------------------------------------------------------
Win32IntResourceManager::ImagePtr Win32IntResourceManager::
getImage(const Url &url, const Dimension &prefferedSize) 
{
	ImageMap::const_iterator it = imageMap.find(url);
	if (it == imageMap.end()) {
		return ImagePtr();
	}
	return it->second;
}
//-----------------------------------------------------------------------------
void Win32IntResourceManager::updateImage(const Url &url) {
}
//-----------------------------------------------------------------------------
std::string Win32IntResourceManager::getString(const Url &url) {
	StringMap::const_iterator it = stringMap.find(url);
	if (it == stringMap.end()) {
		return "";
	}
	return it->second;
}
//-----------------------------------------------------------------------------	
void Win32IntResourceManager::registerImage(const Url &url, long resID) {
	checkResourceAccess();
	imageMap[url] = getDiscoFactory()->createImageSurface(
		Win32ResourceHandler::create(hI, resID)
	);
}
//-----------------------------------------------------------------------------
void Win32IntResourceManager::registerString(const Url &url, long resID) {
	checkResourceAccess();
	Win32ResourceHandler::Ptr hand = Win32ResourceHandler::create(hI, resID);
	std::string tmp = hand->getDataStringStream()->str();
	stringMap[url] = hand->getDataStringStream()->str();
}
//-----------------------------------------------------------------------------
Win32IntResourceManager::Win32IntResourceManager() : hI(NULL) {
}
//-----------------------------------------------------------------------------
void Win32IntResourceManager::setInstance(HINSTANCE hI) {
	this->hI = hI;
}

}} // namespace(s)


#endif // #ifdef DISCO_USE_WIN32