/*
 * ISurface.hpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#ifndef ISURFACE_HPP_
#define ISURFACE_HPP_

#include "sambag/com/Common.hpp"
#include "Geometry.hpp"
#include <boost/shared_ptr.hpp>

namespace sambag { namespace disco {
//=============================================================================
/**
 * Interface for surfaces.
 */
class ISurface {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ISurface> Ptr;
	//-------------------------------------------------------------------------
	virtual Rectangle getSize() const = 0;
};
//=============================================================================
class IImageSurface : public ISurface {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IImageSurface> Ptr;
	//-------------------------------------------------------------------------
	virtual void writeToFile(const std::string &filename) const = 0;
};
}} // namespace

#endif /* ISURFACE_HPP_ */
