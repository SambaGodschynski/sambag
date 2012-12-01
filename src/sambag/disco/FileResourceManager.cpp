/*
 * FileResourceManager.cpp
 *
 *  Created on: Thu Nov  8 11:31:06 2012
 *      Author: Johannes Unger
 */

#include "FileResourceManager.hpp"
#include <boost/filesystem.hpp>
#include <sambag/com/exceptions/IllegalStateException.hpp>
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/svg/SvgBuilder.hpp>
namespace sambag { namespace disco {
namespace {
	typedef Loki::SingletonHolder<FileResourceManager> FactoryHolder;
} // namespace(s)
//=============================================================================
//  Class FileResourceManager
//=============================================================================
//-----------------------------------------------------------------------------
void FileResourceManager::assumeHomeDir() {
	if (homeDir != "")
		return;
	SAMBAG_THROW(sambag::com::exceptions::IllegalStateException,
		"homeDir == NULL");
}
//-----------------------------------------------------------------------------
FileResourceManager::FileResourceManager() {}
//-----------------------------------------------------------------------------
sd::ISurface::Ptr FileResourceManager::loadPng(const std::string &path) {
	return sd::getDiscoFactory()->createImageSurface(
		sambag::com::FileHandler::create(path)
	);	
}
//-----------------------------------------------------------------------------
sd::ISurface::Ptr FileResourceManager::loadSvg(const std::string &path) {
	// prepare recording surface
	sd::IDiscoFactory *fac = sd::getDiscoFactory();
	sd::IRecordingSurface::Ptr surf = fac->createRecordingSurface();
	SAMBAG_ASSERT(surf);
	sd::IDrawContext::Ptr cn = fac->createContext(surf);
	SAMBAG_ASSERT(cn);
	// load svg
	sd::svg::SvgBuilder builder;
	sd::svg::SvgRoot::Ptr rootObject = boost::shared_dynamic_cast<sd::svg::SvgRoot>
			( builder.buildSvgFromFilename(path) );
	sd::svg::graphicElements::SceneGraph::Ptr g = 
		rootObject->getRelatedSceneGraph();
	g->draw(cn);
	// create image surface
	sd::Rectangle size = surf->getSize();
	sd::IImageSurface::Ptr res =
		fac->createImageSurface((int)size.width(), (int)size.height());
	SAMBAG_ASSERT(res);
	cn = fac->createContext(res);
	SAMBAG_ASSERT(cn);
	cn->drawSurface(surf);
	return res;
}
//-----------------------------------------------------------------------------
sd::ISurface::Ptr FileResourceManager::loadImage(const std::string &_path) 
{
	assumeHomeDir();
	boost::filesystem::path path = getHomeDirectory() + "/" + _path;
	if ( !boost::filesystem::exists(path) ) {
		SAMBAG_THROW(sambag::com::exceptions::IllegalStateException,
			"file" + path.string() + " dosent exists.");
		return sd::ISurface::Ptr();
	}
	sd::ISurface::Ptr res;
	if (path.extension() == ".svg") {
		return loadSvg(path.string());
	}
	if (path.extension() == ".png") {
		return loadPng(path.string());
	}
	return res;
}
//-----------------------------------------------------------------------------
sd::ISurface::Ptr FileResourceManager::getImage(const Url &url,
	const Dimension &prefferedSize) 
{
	ImageMap::const_iterator it = imageMap.find(url);
	if (it==imageMap.end())
		return sd::ISurface::Ptr();
	return it->second;
}
//-----------------------------------------------------------------------------
void FileResourceManager::setHomeDirectory(const std::string &path) {
	homeDir = path;
}
//-----------------------------------------------------------------------------
void FileResourceManager::updateImage(const Url &url) {
	ImageMap::iterator it = imageMap.find(url);
	if (it==imageMap.end())
		return;
	sd::ISurface::Ptr img = loadImage(url);
	if (!img)
		return;
	it->second = img; 
}
//-----------------------------------------------------------------------------
void FileResourceManager::init(const std::string &path) {
	instance().setHomeDirectory(path);
}
//-----------------------------------------------------------------------------
void FileResourceManager::registerImage(const Url &url, 
	sd::ISurface::Ptr image)
{
	imageMap[url] = image;
}
//-----------------------------------------------------------------------------
void FileResourceManager::unregisterImage(const Url &url)
{
	imageMap.erase(url);
}
//-----------------------------------------------------------------------------
bool FileResourceManager::registerImage(const Url &url, 
	const std::string &path) 
{
	sd::ISurface::Ptr img = loadImage(path);
	if (!img)
		return false;
	registerImage(url , img);
	return true;
}
//----------------------------------------------------------------------------
FileResourceManager & FileResourceManager::instance() {
	return FactoryHolder::Instance();
}
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
IResourceManager & getResourceManager() {
	return FileResourceManager::instance();
}
}} // namespace(s)
