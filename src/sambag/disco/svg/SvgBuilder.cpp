/*
 * SvgBuilder.cpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "SvgBuilder.hpp"
#include "sambag/disco/svg/SvgLine.hpp"

namespace sambag { namespace disco { namespace svg {
//=============================================================================
// class SvgBuilder
//=============================================================================
//-----------------------------------------------------------------------------
SvgBuilder::SvgBuilder() {
	registerSvgClasses();
	registerSvgAttributes();
}
//-----------------------------------------------------------------------------
SvgBuilder::~SvgBuilder() {

}
//-----------------------------------------------------------------------------
void SvgBuilder::registerSvgClasses() {
	 xml2Obj.registerObject<SvgRoot>("svg");
	 xml2Obj.registerObject<SvgLine>("line");
}
//-----------------------------------------------------------------------------
void SvgBuilder::registerSvgAttributes() {
	SvgObject::registerAttributes(xml2Obj);
	SvgLine::registerAttributes(xml2Obj);
}
//-----------------------------------------------------------------------------
SvgObject::Ptr SvgBuilder::buildSvgFromString( const std::string & str)
{
	return xml2Obj.buildWithXmlString(str);
}
//-----------------------------------------------------------------------------
SvgObject::Ptr SvgBuilder::buildSvgFromFilename( const std::string & name)
{
	return xml2Obj.buildWithXmlFile(name);
}

}}}
