/*
 * Def.hpp
 *
 *  Created on: 06.10.2011
 *      Author: samba
 */

#ifndef DISCO_SVG_DEF_HPP_
#define DISCO_SVG_DEF_HPP_

#include "Svg.hpp"
#include "sambag/com/Common.hpp"
#include "SvgCompound.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
class SvgDefs : public SvgCompound {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgDefs> Ptr;
private:
protected:
	SvgDefs(){}
public:
	//-------------------------------------------------------------------------
	virtual ~SvgDefs(){}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgDefs());
		neu->__setSelf(neu);
		neu->createBase(root);
		return neu;
	}
};

}}} // namespaces

#endif /* DEF_HPP_ */
