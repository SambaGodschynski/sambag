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
#include <sambag/disco/svg/Image.hpp>
#include <sambag/disco/svg/graphicElements/Style.hpp>
#include <sambag/com/ArbitraryType.hpp>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include <sambag/disco/svg/graphicElements/ISceneGraph.hpp>
namespace sambag { namespace disco { namespace components {
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
        //---------------------------------------------------------------------
        static const std::string PROPERTY_MODEL;
        static const std::string PROPERTY_HIT_STRATEGY;
    protected:
        //---------------------------------------------------------------------
        Dummy() {
            setName("SvgComponent::Dummy");
        }
    private:
        //---------------------------------------------------------------------
        mutable IPattern::Ptr tmpBg, tmpFg;
        //---------------------------------------------------------------------
        friend class SvgComponent;
        boost::weak_ptr<IDrawable> drawable;
        //---------------------------------------------------------------------
        /**
        * the model object which is possibly used by an extension.
        */
        typedef com::ArbitraryType ModelObject;
        ModelObject::Ptr model;
    public:
        //---------------------------------------------------------------------
        ModelObject::Ptr getRawModel() const {
            return model;
        }
        //---------------------------------------------------------------------
        template<class Model>
        void setModel(boost::shared_ptr<Model> model) {
            com::ArbitraryType::Ptr old = this->model;
            this->model = com::createObject(model);
            firePropertyChanged(PROPERTY_MODEL, old, this->model);
        }
        //---------------------------------------------------------------------
        /**
        * @return the model object of the extension, can be null if
        * no extension is used or the concrete type doesn't match.
        */
        template <class ConcreteModel>
        boost::shared_ptr<ConcreteModel> getModel() const {
            boost::shared_ptr<ConcreteModel> res;
            com::get(model, res);
            return res;
        }
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
        /**
         * @return calculated style
         */
        svg::graphicElements::Style getStyle() const;
        //---------------------------------------------------------------------
        void setStyle(const svg::graphicElements::Style &x);
        //---------------------------------------------------------------------
        /**
         * @return style ref of object.
         * @note can be null even if object has style in result. 
         * please consider the differences between a style ref. and a calculated
         * style
         */
        typedef boost::shared_ptr<svg::graphicElements::Style> StylePtr;
        StylePtr getStyleRef();
        //---------------------------------------------------------------------
        SAMBAG_STD_STATIC_COMPONENT_CREATOR(Dummy)
        //---------------------------------------------------------------------
        void drawComponent (IDrawContext::Ptr context);
        //---------------------------------------------------------------------
        virtual void setVisible(bool b);
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
    /**
     * @brief if you want to create a component with interaction
     * such as a knob or a button you need to 'extend' the component.
     * This is done by an "Extend" function, which is usually triggered
     * by a registered svg class keyword.
     */
    typedef boost::function<void(Dummy::Ptr)> ExtendFunction;
    typedef std::string SvgClass;
    typedef boost::unordered_map<SvgClass, ExtendFunction> ExtensionRegister;
    //-------------------------------------------------------------------------
    virtual void drawComponent (IDrawContext::Ptr context);
    //-------------------------------------------------------------------------
    virtual void postConstructor();
    //-------------------------------------------------------------------------
    SvgComponent();
    //-------------------------------------------------------------------------
    void setupSvgObject(svg::SvgRootPtr obj);
    //-------------------------------------------------------------------------
    /**
     * @return the svg root object
     */
    svg::SvgRootPtr getSvgObject() const;
private:
    //-------------------------------------------------------------------------
    struct SyncedSceneGraph;
    mutable boost::shared_ptr<SyncedSceneGraph> sceneGraph;
    //-------------------------------------------------------------------------
    void initExtendRegister();
    //-------------------------------------------------------------------------
    ExtensionRegister exReg;
    //-------------------------------------------------------------------------
    com::ArithmeticWrapper<bool, false> stretchToFit;
    //-------------------------------------------------------------------------
    typedef boost::unordered_map<IDrawable::Ptr, DummyWPtr> ElementMap;
    ElementMap elMap;
    //-------------------------------------------------------------------------
    svg::Image::Ptr svgImage;
    //-------------------------------------------------------------------------
    void updateDummies();
    //-------------------------------------------------------------------------
    DummyPtr createDummy(IDrawable::Ptr x);
    //-------------------------------------------------------------------------
    void updateDrawOrder();
    //-------------------------------------------------------------------------
    DummyPtr getDummyOrCreateNew(IDrawable::Ptr x);
    //-------------------------------------------------------------------------
    void installExtension(DummyPtr dummy, const std::list<SvgClass> &classes);
public:
    //-------------------------------------------------------------------------
    Dimension getSvgSize() const;
    //-------------------------------------------------------------------------
    svg::graphicElements::ISceneGraph::Ptr getSceneGraph() const;
    //-------------------------------------------------------------------------
    const ExtensionRegister & getExtensionRegister() const;
    //-------------------------------------------------------------------------
    ExtensionRegister & getExtensionRegister();
    //-------------------------------------------------------------------------
    virtual AComponentPtr findComponentAt (const Point2D &p, bool includeSelf=true);
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
