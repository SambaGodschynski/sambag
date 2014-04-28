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
#include "sambag/disco/svg/SvgCircle.hpp"
#include "sambag/disco/svg/SvgEllipse.hpp"
#include "sambag/disco/svg/SvgDefs.hpp"
#include "sambag/disco/svg/SvgUse.hpp"
#include "sambag/disco/svg/SvgUse.hpp"
#include "sambag/disco/svg/SvgPolyline.hpp"
#include "sambag/disco/svg/SvgPolygon.hpp"
#include "sambag/disco/svg/SvgImage.hpp"
#include "sambag/disco/svg/SvgStyle.hpp"
#include "sambag/disco/svg/SvgLinearGradient.hpp"
#include "sambag/disco/svg/SvgRadialGradient.hpp"
#include "sambag/disco/svg/SvgColorStop.hpp"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

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
	 xml2Obj.registerObject<SvgCircle>("circle");
	 xml2Obj.registerObject<SvgEllipse>("ellipse");
	 xml2Obj.registerObject<SvgDefs>("defs");
	 xml2Obj.registerObject<SvgUse>("use");
	 xml2Obj.registerObject<SvgPolyline>("polyline");
	 xml2Obj.registerObject<SvgPolygon>("polygon");
	 xml2Obj.registerObject<SvgImage>("image");
	 xml2Obj.registerObject<SvgStyle>("style");
	 xml2Obj.registerObject<SvgLinearGradient>("linearGradient");
	 xml2Obj.registerObject<SvgRadialGradient>("radialGradient");
	 xml2Obj.registerObject<SvgPattern>("pattern");
	 xml2Obj.registerObject<SvgColorStop>("stop");
}
//-----------------------------------------------------------------------------
void SvgBuilder::registerSvgAttributes() {
	SvgRoot::registerAttributes(xml2Obj);
	SvgObject::registerAttributes(xml2Obj);
	SvgLine::registerAttributes(xml2Obj);
	SvgRect::registerAttributes(xml2Obj);
	SvgText::registerAttributes(xml2Obj);
	SvgPath::registerAttributes(xml2Obj);
	SvgCircle::registerAttributes(xml2Obj);
	SvgEllipse::registerAttributes(xml2Obj);
	SvgUse::registerAttributes(xml2Obj);
	SvgPolyline::registerAttributes(xml2Obj);
	SvgPolygon::registerAttributes(xml2Obj);
	SvgImage::registerAttributes(xml2Obj);
	SvgLinearGradient::registerAttributes(xml2Obj);
	SvgRadialGradient::registerAttributes(xml2Obj);
	SvgColorStop::registerAttributes(xml2Obj);
}
//-----------------------------------------------------------------------------
SvgObject::Ptr SvgBuilder::buildSvgFromString(const std::string & str)
{
	graphicElements::SceneGraph::Ptr g = graphicElements::SceneGraph::create();
	SvgRoot::Ptr root = SvgRoot::create(g.get());
	xml2Obj.setClosure(root.get());
	SvgObject::BuilderType::CreatedSignalFunction f =
			boost::bind( &SvgRoot::subObjectCreated, root.get(), _1, _2 );
	xml2Obj.addObjectCreatedSlot(f);
	SvgObject::Ptr neu = xml2Obj.buildWithXmlString(str, root);
	root->initCreatedObjects();
	return neu;
}
//-----------------------------------------------------------------------------
SvgObject::Ptr SvgBuilder::buildSvgFromFilename(const std::string & name)
{
	graphicElements::SceneGraph::Ptr g = graphicElements::SceneGraph::create();
	SvgRoot::Ptr root = SvgRoot::create(g.get(),true);
	boost::filesystem::path loc = name;
	root->setSvgPath(loc.remove_filename().string() + "/");
	xml2Obj.setClosure(root.get());
	SvgObject::BuilderType::CreatedSignalFunction f =
			boost::bind( &SvgRoot::subObjectCreated, root.get(), _1, _2 );
	xml2Obj.addObjectCreatedSlot(f);
	SvgObject::Ptr neu = xml2Obj.buildWithXmlFile(name, root);
	root->initCreatedObjects();
	return neu;
}

}}}
