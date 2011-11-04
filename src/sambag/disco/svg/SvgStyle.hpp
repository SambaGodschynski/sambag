/*
 * SvgStyle.hpp
 *
 *  Created on: Nov 14, 2011
 *      Author: samba
 */

#ifndef SVGSTYLE_HPP_
#define SVGSTYLE_HPP_

#include "Svg.hpp"
#include "sambag/com/Common.hpp"

namespace sambag { namespace disco { namespace svg {

//=============================================================================
class SvgStyle : public SvgObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgStyle> Ptr;
private:
	//-------------------------------------------------------------------------
	std::string styleText;
protected:
	//-------------------------------------------------------------------------
	SvgStyle(){}
public:
	//-------------------------------------------------------------------------
	/**
	 * performs styling on scenegraph
	 */
	virtual void init();
	//-------------------------------------------------------------------------
	virtual GraphicElement::Ptr getGraphicElement() const {
		return GraphicElement::Ptr();
	}
	//-------------------------------------------------------------------------
	virtual void setRelatedSceneGraph(SceneGraph::Ptr ptr) {
		sceneGraph = ptr;
	}
	//-------------------------------------------------------------------------
	virtual void setXmlText(const std::string &str) {
		styleText = str;
	}
	//-------------------------------------------------------------------------
	virtual ~SvgStyle(){}
	//-------------------------------------------------------------------------
	static Ptr create( SvgRoot *root = NULL ) {
		Ptr neu(new SvgStyle());
		neu->__setSelf(neu);
		neu->createBase(root);
		return neu;
	}
};

}}} // namespaces

#endif /* SVGSTYLE_HPP_ */
