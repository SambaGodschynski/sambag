/*
 * SvgComponent.hpp
 *
 *  Created on: Sat Mar 29 15:15:06 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SVGCOMPONENT_H
#define SAMBAG_SVGCOMPONENT_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "AContainer.hpp"

namespace sambag { namespace disco {
namespace svg {
    class SvgRoot;
    typedef boost::shared_ptr<SvgRoot> SvgRootPtr;
}
namespace components {
//=============================================================================
/** 
  * @class SvgComponent.
  * @brief creates a disco component by using a svg.
  * SvgDummyComponents will be created by svg groups, they will be subobjects
  * of the SvgComponent. 
  */
class SvgComponent : public AContainer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SvgComponent> Ptr;
	typedef boost::weak_ptr<SvgComponent> WPtr;
protected:
    //-------------------------------------------------------------------------
    virtual void drawComponent (IDrawContext::Ptr context);
    //-------------------------------------------------------------------------
    virtual void postConstructor();
	//-------------------------------------------------------------------------
	SvgComponent();
    //-------------------------------------------------------------------------
    void setupSvgObject(svg::SvgRootPtr obj);
private:
    //-------------------------------------------------------------------------
    svg::SvgRootPtr rootObject;
public:
    //-------------------------------------------------------------------------
    virtual void doLayout();
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(SvgComponent)
    //-------------------------------------------------------------------------
    /**
     * @brief set the svg content as svg stringfile
     * @throw IllegalStateException
     */
    void setSvgString(const std::string &str);
    //-------------------------------------------------------------------------
    /**
     * @brief set the svg content as svg file
     * @throw IllegalStateException
     */
    void setSvgFilename(const std::string &name);
}; // SvgComponent
}}} // namespace(s)

#endif /* SAMBAG_SVGCOMPONENT_H */
