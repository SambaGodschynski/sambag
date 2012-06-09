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
	//-------------------------------------------------------------------------
	/**
	 * note: not all surfaces implements this
	 * LibCairo:
	 * A Pixmap can never change size, so it is never necessary to call this
	 * function on a surface created for a Pixmap.
	 * @return
	 */
	virtual void setSize(const Rectangle &r) {}
};
//=============================================================================
class IImageSurface : public virtual ISurface {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IImageSurface> Ptr;
	//-------------------------------------------------------------------------
	virtual void writeToFile(const std::string &filename) const = 0;
};
//=============================================================================
class IRecordingSurface : public virtual ISurface {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IRecordingSurface> Ptr;
};
}} // namespace

#endif /* ISURFACE_HPP_ */
