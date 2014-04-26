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
#include <boost/unordered_map.hpp>

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
    /**
     * @brief A Dummy will be created for every "Disco" class element
     * in the svg tree. The SvgComponent and its containing Dummy components
     * are the Component representation of the Svg document. 
     */
    //=========================================================================
    //  Class Dummy
    //=========================================================================
    class Dummy : public AContainer {
    public:
        //---------------------------------------------------------------------
        typedef boost::shared_ptr<Dummy> Ptr;
        typedef boost::weak_ptr<Dummy> WPtr;
    protected:
        //---------------------------------------------------------------------
        Dummy() {
            setName("SvgComponent::Dummy");
        }
    private:
        //---------------------------------------------------------------------
        friend class SvgComponent;
        boost::weak_ptr<IDrawable> drawable;
    public:
        //---------------------------------------------------------------------
        /**
         * @brief set the stroke color on related object in scene graph
         */
        virtual void setForeground(IPattern::Ptr pat);
        //---------------------------------------------------------------------
        /**
         * @brief set the fill color on related object in scene graph
         */
        virtual void setBackground(IPattern::Ptr pat);
        //---------------------------------------------------------------------
        /**
         * @return the stroke color on related object in scene graph
         */
        virtual IPattern::Ptr getForegroundPattern() const;
        //---------------------------------------------------------------------
        /**
         * @return the fill color on related object in scene graph
         */
        virtual IPattern::Ptr getBackgroundPattern() const;
        //---------------------------------------------------------------------
        SAMBAG_STD_STATIC_COMPONENT_CREATOR(Dummy)
        //---------------------------------------------------------------------
        void drawComponent (IDrawContext::Ptr context);
    };
    typedef boost::shared_ptr<Dummy> DummyPtr;
    typedef boost::weak_ptr<Dummy> DummyWPtr;
    //-------------------------------------------------------------------------
    typedef AContainer Super;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<SvgComponent> Ptr;
    typedef boost::weak_ptr<SvgComponent> WPtr;
protected:
    //-------------------------------------------------------------------------
    void createComponentTree();
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
    com::ArithmeticWrapper<bool, false> stretchToFit;
    //-------------------------------------------------------------------------
    typedef boost::unordered_map<IDrawable::Ptr, DummyWPtr> ElementMap;
    ElementMap elMap;
    //-------------------------------------------------------------------------
    svg::SvgRootPtr rootObject;
    //-------------------------------------------------------------------------
    void updateDummyBounds(AContainer::Ptr x);
    //-------------------------------------------------------------------------
    void updateDummyBounds();
    //-------------------------------------------------------------------------
    DummyPtr createDummy(IDrawable::Ptr x);
public:
    //-------------------------------------------------------------------------
    /**
     * @return the related dummy component for a svg drawable
     */
    DummyPtr getDummy(IDrawable::Ptr x);
    //-------------------------------------------------------------------------
    /**
     * @return the related svg drawable for a dummy object
     */
    IDrawable::Ptr getDrawable(DummyPtr x);
    //-------------------------------------------------------------------------
    /**
     * @return dummy for svg id
     * @note the id including pre '#' char -> "#objectID"
     */
    DummyPtr getDummyById(const std::string &id);
    //-------------------------------------------------------------------------
    /**
     * @return dummies for svg class
     * @note the id including pre '.' char -> ".classID"
     */
    void getDummiesByClass(const std::string &_class, std::vector<DummyPtr> &out);
    //-------------------------------------------------------------------------
    void setStretchToFit(bool stretch);
    //-------------------------------------------------------------------------
    bool isStretchToFit() const {return stretchToFit;}
    //-------------------------------------------------------------------------
    /**
     * @return the svg root object
     */
    svg::SvgRootPtr getSvgObject() const {
        return rootObject;
    }
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
