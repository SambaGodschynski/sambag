/*
 * Win32IntResourceManager.hpp
 *
 *  Created on: Thu Jan 17 11:39:19 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WIN32INTRESOURCEMANAGER_H
#define SAMBAG_WIN32INTRESOURCEMANAGER_H

#ifdef DISCO_USE_WIN32

#include <boost/shared_ptr.hpp>
#include "IResourceManager.hpp"
#include <loki/Singleton.h>
#include <boost/unordered_map.hpp>
#include <windows.h>

namespace sambag { namespace disco {
//=============================================================================
/** 
  * @class Win32IntResourceManager.
  * Resource manager for win32 internal app resources
  */
class Win32IntResourceManager : public IResourceManager {
//=============================================================================
friend struct Loki::CreateUsingNew<Win32IntResourceManager>;
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Win32IntResourceManager> Ptr;
protected:
	//-------------------------------------------------------------------------
	Win32IntResourceManager();
private:
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<Url, ImagePtr> ImageMap;
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<Url, std::string> StringMap;
	//-------------------------------------------------------------------------
	ImageMap imageMap;
	//-------------------------------------------------------------------------
	StringMap stringMap;
	//-------------------------------------------------------------------------
	void checkResourceAccess();
	//-------------------------------------------------------------------------
	HINSTANCE hI;
public:
	//-------------------------------------------------------------------------
	void setInstance(HINSTANCE hI);
	//-------------------------------------------------------------------------
	HINSTANCE getInstance() const {
		return hI;
	}
	//-------------------------------------------------------------------------
	static Win32IntResourceManager & instance();
	//-------------------------------------------------------------------------	
	virtual ImagePtr getImage(const Url &url,
		const Dimension &prefferedSize = Dimension(0,0));
	//-------------------------------------------------------------------------
	/**
	 * resources are read only. does nothing.
	 */
	virtual void updateImage(const Url &url);
	//-------------------------------------------------------------------------	
	virtual std::string getString(const Url &url);
	//-------------------------------------------------------------------------	
	/**
	 * @throw IllegalStateException
	 */
	void registerImage(const Url &url, long resID);
	//-------------------------------------------------------------------------	
	/**
	 * @throw IllegalStateException
	 */
	void registerString(const Url &url, long resID);
}; // Win32IntResourceManager
}} // namespace(s)

#endif // #ifdef DISCO_USE_WIN32
#endif /* SAMBAG_WIN32INTRESOURCEMANAGER_H */
