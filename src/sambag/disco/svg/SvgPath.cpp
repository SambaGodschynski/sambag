/*
 * Path.cpp
 *
 *  Created on: 27.09.2011
 *      Author: samba
 */

#include "SvgPath.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
// class SvgPath
//=============================================================================
//-----------------------------------------------------------------------------
SvgPath::SvgPath() {
	path = sambag::disco::graphicElements::Path::create();
}
//-----------------------------------------------------------------------------
SvgPath::~SvgPath() {
}

}}}
