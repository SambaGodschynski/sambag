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
#include "sambag/com/Common.hpp"
#include <map>

namespace sambag { namespace disco { namespace svg {

//=============================================================================
/**
 * binding to the svg <root> element.
 */
class SvgRoot : public SvgCompound {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgRoot> Ptr;
	//-------------------------------------------------------------------------
	typedef std::list<SvgObject::Ptr> Svgs;
	// created svg objects
	Svgs svgs;
private:
	//-------------------------------------------------------------------------
	/**
	 * Initiates svg objects. eg
	 */
	virtual void init();
protected:
	//-------------------------------------------------------------------------
	SvgRoot(){}
public:
	//-------------------------------------------------------------------------
	virtual ~SvgRoot(){}
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param g
	 * @param firstElement true if first element in tree
	 * @return
	 */
	static Ptr create(
			graphicElements::SceneGraph *g = NULL,
			bool firstElement = false)
	{
		Ptr neu(new SvgRoot());
		neu->__setSelf(neu);
		if (!g)
			return neu;
		neu->setRelatedSceneGraph(g->getPtr());
		// set default style if first element
		if (firstElement) {
			g->setStyleTo(neu->getGraphicElement(),
					graphicElements::Style::DEFAULT_STYLE);
		}
		return neu;
	}
	//-------------------------------------------------------------------------
	/**
	 * Funtion for @see XML2Object::CreatedSignal slot.
	 * Will be called whenever a sub object of this root element was created.
	 * @param newObject
	 */
	void subObjectCreated( SvgObject::Ptr newObject );
};


}}} // namespace
#endif /* SVGROOT_HPP_HPP_ */
