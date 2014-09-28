/*
 * SceneGraph.hpp
 *
 *  Created on: 19.10.2011
 *      Author: samba
 */

#ifndef ISCENEGRAPH_HPP_
#define ISCENEGRAPH_HPP_

#include "sambag/disco/IDrawContext.hpp"
#include "sambag/disco/IDrawable.hpp"
#include <sambag/disco/svg/Style.hpp>
#include "sambag/com/Common.hpp"

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
/**
 * @class ISceneGraph.
 * @brief SceneGraph base
 * see http://www.4divisions.com/forx/wiki/doku.php?id=wiki:scenegraph
 */
class ISceneGraph {
//=============================================================================
public:
    //-------------------------------------------------------------------------
    virtual ~ISceneGraph() {}
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<ISceneGraph> Ptr;
    //-------------------------------------------------------------------------
    typedef boost::weak_ptr<ISceneGraph> WPtr;
    //-------------------------------------------------------------------------
    // TODO: cleanup SceneGraphElement / IDrawable confusuion
    typedef IDrawable::Ptr SceneGraphElement;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<Style> StylePtr;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<Matrix> MatrixPtr;
    //-------------------------------------------------------------------------
    typedef std::string Id;
    //-------------------------------------------------------------------------
    typedef std::string Class;
    //-------------------------------------------------------------------------
    typedef std::string Tag;
    //-------------------------------------------------------------------------
    typedef std::vector<SceneGraphElement> Elements;
    //-------------------------------------------------------------------------
    typedef std::vector<Class> Classes;
    //-------------------------------------------------------------------------
    /**
     * @brief invalidates bounds and scene
     */
    virtual void invalidate() = 0;
    //-------------------------------------------------------------------------
    /**
     * @brief invalidates bounds only
     */
    virtual void invalidateBounds() = 0;
    //-------------------------------------------------------------------------
    /**
     * @brief validates bounds and scene
     * @param size is used to solve relative svg coordinates
     */
    virtual void validate(const Dimension &size) = 0;
    //-------------------------------------------------------------------------
    /**
     * @brief validates bounds only
     * @param size is used to solve relative svg coordinates
     */
    virtual void validateBounds(const Dimension &size) = 0;
    //-------------------------------------------------------------------------
    virtual SceneGraphElement getRootElement() = 0;
    //-------------------------------------------------------------------------
    virtual void draw(IDrawContext::Ptr) = 0;
    //-------------------------------------------------------------------------
    virtual bool setTransfomationTo(SceneGraphElement el, const Matrix &m) = 0;
    //-------------------------------------------------------------------------
    /**
     * creates a style node and relates it to el
     * @param el a SceneGraphElement of this graph object
     * @param s a @see Style object
     */
    virtual bool setStyleTo(SceneGraphElement el, const Style &s) = 0;
    //-------------------------------------------------------------------------
    /**
     * creates a transformation node and relates it to el
     * @param el a SceneGraphElement of this graph object
     * @param m a transformation matrix
     */
    virtual bool setTransfomationRefTo(SceneGraphElement el, MatrixPtr m) = 0;
    //-------------------------------------------------------------------------
    /**
     * creates a style node and relates it to el
     * @param el a SceneGraphElement of this graph object
     * @param s a @see Style object
     */
    virtual bool setStyleRefTo(SceneGraphElement el, StylePtr s) = 0;
    //-------------------------------------------------------------------------
    virtual MatrixPtr getTransformationRef(SceneGraphElement el) const = 0;
    //-------------------------------------------------------------------------
    /**
     * @return the transformation matrix of an element
     * @note this does not considering the scene tree
     * Use @see calculateTransformation() for this purpose.
     */
    virtual Matrix getTransformationOf(SceneGraphElement el) const = 0;
    //-------------------------------------------------------------------------
    /**
     * @return the style related to an element
     * @note this style does not considering the scene tree, which means
     * if a parent element is filled red and the target element isn't filled,
     * the resulting style has no fill attribute. 
     * Use @see calculateStyle() instead.
     */
    virtual Style getStyleOf(SceneGraphElement el) const = 0;
    //-------------------------------------------------------------------------
    /**
     * @brief calculates the style of an element considering the scene
     * tree.
     */
    virtual Style calculateStyle(SceneGraphElement el) = 0;
    //-------------------------------------------------------------------------
    /**
     * @brief calculates the transformatin of an element considering the scene
     * tree.
     */
    virtual Matrix calculateTransformation(SceneGraphElement el) = 0;
    //-------------------------------------------------------------------------
    virtual StylePtr getStyleRef(SceneGraphElement el) const = 0;
    //-------------------------------------------------------------------------
    virtual void getChildrenX(SceneGraphElement el,
        Elements &c, bool deep = true ) const = 0;
    //-------------------------------------------------------------------------
    virtual void getChildrenByTagX(SceneGraphElement el, const Tag &tagName,
        Elements &c, bool deep = true) const = 0;
    //-------------------------------------------------------------------------
    virtual void getChildrenByClassX(SceneGraphElement el, const Class &className,
            Elements &c, bool deep = true) const = 0;
    //-------------------------------------------------------------------------
    virtual void getChildrenByIdX(SceneGraphElement el, const Id &id,
        Elements &c, bool deep = true) const = 0;
    //-------------------------------------------------------------------------
    virtual void getClassNamesX(SceneGraphElement el, Classes &c) const = 0;
    //-------------------------------------------------------------------------
    virtual void getElementsByClassX(const Class & className, Elements &c) = 0;
    //-------------------------------------------------------------------------
    virtual void getElementsByTagX(const Tag & tagName, Elements &c) = 0;
    //-------------------------------------------------------------------------
    virtual void select(const std::string & sel, Elements &c) = 0;
    //-------------------------------------------------------------------------
    virtual Tag getTagName(SceneGraphElement el) const = 0;
    //-------------------------------------------------------------------------
    virtual Id getIdName(SceneGraphElement el) const = 0;
    //-------------------------------------------------------------------------
    virtual SceneGraphElement getElementById(const Id &id) const = 0;
    //-------------------------------------------------------------------------
    /**
     * @return bounds or NULL_RECTANGLE if obj not found
     */
    virtual Rectangle getBoundingBox(SceneGraphElement obj) const = 0;
    //-------------------------------------------------------------------------
    /**
     * @brief Set an element visible or not. If an element has children then
     *        they will be affected too.
     */
    virtual void setVisible(SceneGraphElement el, bool val) = 0;
    //-------------------------------------------------------------------------
    /**
     * @return true if an element is visible.
     */
    virtual bool isVisible(SceneGraphElement el) const = 0;
};
}}}} // namespace


#endif /* ISCENEGRAPH_HPP_ */
