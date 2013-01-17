/*
 * FileResourceManager.hpp
 *
 *  Created on: Thu Nov  8 11:31:06 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_FRXGRAPHICFILEREPOSITORY_H
#define SAMBAG_FRXGRAPHICFILEREPOSITORY_H

#include <boost/shared_ptr.hpp>
#include <loki/Singleton.h>
#include "IResourceManager.hpp"
#include <boost/unordered_map.hpp>

namespace sambag { namespace disco {
//=============================================================================
/** 
  * @class FileResourceManager.
  */
class FileResourceManager : public IResourceManager {
//=============================================================================
friend struct Loki::CreateUsingNew<FileResourceManager>;
public:
	//-------------------------------------------------------------------------
	typedef FileResourceManager* Ptr;
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<Url, ImagePtr> ImageMap;
protected:
	//-------------------------------------------------------------------------
	ImageMap imageMap;
	//-------------------------------------------------------------------------
	std::string homeDir;
	//-------------------------------------------------------------------------
	void registerImage(const Url &url, ImagePtr image);
	//-------------------------------------------------------------------------
	FileResourceManager();
private:
	//-------------------------------------------------------------------------
	ImagePtr loadPng(const std::string &path);
	//-------------------------------------------------------------------------
	ImagePtr loadSvg(const std::string &path);
	//-------------------------------------------------------------------------
	void setHomeDirectory(const std::string &path);
	//-------------------------------------------------------------------------
	/**
	 * @throw IllegalStateException 
	 */
	void assumeHomeDir();
public:
	//-------------------------------------------------------------------------	
	virtual std::string getString(const Url &url);
	//-------------------------------------------------------------------------
	static FileResourceManager & instance();
	//-------------------------------------------------------------------------
	bool registerImage(const Url &url, const std::string &path);
	//-------------------------------------------------------------------------
	void unregisterImage(const Url &url);
	//-------------------------------------------------------------------------
	static void init(const std::string &homeDirPath);
	//-------------------------------------------------------------------------
	const std::string & getHomeDirectory() const {
		return homeDir;
	}
	//-------------------------------------------------------------------------
	ImagePtr loadImage(const std::string &path);
	//-------------------------------------------------------------------------
	virtual ImagePtr getImage(const Url &url,
		const Dimension &prefferedSize = Dimension(0,0));
	//-------------------------------------------------------------------------
	/**
	 * reloads file.
	 */
	virtual void updateImage(const Url &url);
}; // FileResourceManager
}} // namespace(s)

#endif /* SAMBAG_FRXGRAPHICFILEREPOSITORY_H */
