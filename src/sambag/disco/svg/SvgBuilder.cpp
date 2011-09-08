/*
 * SvgBuilder.cpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#include "SvgBuilder.hpp"
#include "sambag/disco/svg/SvgLine.hpp"
#include "sambag/disco/svg/SvgRect.hpp"
#include "sambag/disco/svg/SvgGroup.hpp"
#include "sambag/disco/svg/SvgRoot.hpp"
#include "sambag/disco/svg/SvgText.hpp"
#include "sambag/disco/svg/SvgPath.hpp"
#include <boost/bind.hpp>

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
	 xml2Obj.registerObject<SvgRect>("rect");
	 xml2Obj.registerObject<SvgGroup>("g");
	 xml2Obj.registerObject<SvgText>("text");
	 xml2Obj.registerObject<SvgPath>("path");
}
//-----------------------------------------------------------------------------
void SvgBuilder::registerSvgAttributes() {
	SvgObject::registerAttributes(xml2Obj);
	SvgLine::registerAttributes(xml2Obj);
	SvgRect::registerAttributes(xml2Obj);
	SvgText::registerAttributes(xml2Obj);
	SvgPath::registerAttributes(xml2Obj);
}
//-----------------------------------------------------------------------------
SvgObject::Ptr SvgBuilder::buildSvgFromString( const std::string & str)
{
	SvgRoot::Ptr root = SvgRoot::create();
	SvgObject::BuilderType::CreatedSignalFunction f =
			boost::bind( &SvgRoot::subObjectCreated, root.get(), _1 );
	xml2Obj.addObjectCreatedSlot(f);
	return xml2Obj.buildWithXmlString(str, root);
}
//-----------------------------------------------------------------------------
SvgObject::Ptr SvgBuilder::buildSvgFromFilename( const std::string & name)
{
	SvgRoot::Ptr root = SvgRoot::create();
	SvgObject::BuilderType::CreatedSignalFunction f =
			boost::bind( &SvgRoot::subObjectCreated, root.get(), _1 );
	xml2Obj.addObjectCreatedSlot(f);
	return xml2Obj.buildWithXmlFile(name, root);
}

}}}
