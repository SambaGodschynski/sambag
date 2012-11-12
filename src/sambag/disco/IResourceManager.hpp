/*
 * IResourceManager.hpp
 *
 *  Created on: Thu Nov  8 11:29:51 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_IGRAPHICSREPOSITORY_H
#define SAMBAG_IGRAPHICSREPOSITORY_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/ISurface.hpp>
#include <string>

namespace sambag { namespace disco {
namespace sd = sambag::disco;
//=============================================================================
/** 
  * @class IResourceManager.
  * Loads an type idependent image by url. For example: 'images.myImage'
  * Every specific implementation has to define where the data for a related
  * url came from.
  */
class IResourceManager {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef std::string Url;
	//-------------------------------------------------------------------------
	typedef IResourceManager* Ptr;
	//-------------------------------------------------------------------------	
	/**
	 * @return an image related to url or NULL.
	 */ 
	virtual sd::ISurface::Ptr getImage(const Url &url) = 0;
	//-------------------------------------------------------------------------
	/**
	 * updates related image e.g. 
	 * for the case that a caching mechanism is implemented.
	 */
	virtual void updateImage(const Url &url) = 0;
}; // IResourceManager
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
extern IResourceManager & getGraphicsRepository();
}} // namespace(s)

#endif /* SAMBAG_IGRAPHICSREPOSITORY_H */
