/*
 * SvgGroup.hpp
 *
 *  Created on: 23.09.2011
 *      Author: samba
 */

#ifndef SVGGROUP_HPP_
#define SVGGROUP_HPP_

#include "Svg.hpp"
#include "sambag/com/Common.hpp"
#include "SvgCompound.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
class SvgGroup : public SvgCompound {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgGroup> Ptr;
private:
protected:
	SvgGroup(){}
public:
	//-------------------------------------------------------------------------
	virtual ~SvgGroup(){}
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new SvgGroup());
		neu->__setSelf(neu);
		return neu;
	}
};

}}} // namespaces

#endif /* SVGGROUP_HPP_ */
