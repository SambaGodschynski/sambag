/*
 * SvgRoot.hpp.hpp
 *
 *  Created on: 24.09.2011
 *      Author: samba
 */

#ifndef SVGROOT_HPP_HPP_
#define SVGROOT_HPP_HPP_

#include "Svg.hpp"
#include "SvgCompound.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
class SvgRoot : public SvgCompound {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgRoot> Ptr;
private:
protected:
	SvgRoot(){}
public:
	//-------------------------------------------------------------------------
	virtual ~SvgRoot(){}
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new SvgRoot());
		neu->__setSelf(neu);
		return neu;
	}
};


}}} // namespace
#endif /* SVGROOT_HPP_HPP_ */
