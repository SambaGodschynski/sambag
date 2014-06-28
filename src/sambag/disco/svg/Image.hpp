/*
 * Image.hpp
 *
 *  Created on: Fri Jun 27 14:35:04 2014
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_Image_H
#define SAMBAG_Image_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sambag/disco/IDrawable.hpp>
namespace sambag { namespace disco { namespace svg {
class SvgRoot;
typedef boost::shared_ptr<SvgRoot> SvgRootPtr;
//=============================================================================
/** 
  * @class Image.
  */
class Image : public IDrawable {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef IDrawable Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Image> Ptr;
	typedef boost::weak_ptr<Image> WPtr;
protected:
	//-------------------------------------------------------------------------
	Image();
private:
    //-------------------------------------------------------------------------
    svg::SvgRootPtr rootObject;
public:
    //-------------------------------------------------------------------------
    svg::SvgRootPtr getSvgObject() const {
        return rootObject;
    }
    //-------------------------------------------------------------------------
    static Ptr create();
    //-------------------------------------------------------------------------
    void setSvgPath(const std::string &path);
    //-------------------------------------------------------------------------
    void setSvgString(const std::string &str);
    //-------------------------------------------------------------------------
    virtual void setSize(const Dimension&, bool stretchToFit=false);
    //-------------------------------------------------------------------------
    virtual void draw(IDrawContext::Ptr context);
    //-------------------------------------------------------------------------
    virtual Rectangle getBoundingBox(IDrawContext::Ptr context);
    //-------------------------------------------------------------------------
    virtual std::string toString() const;
    
}; // Image
}}} // namespace(s)

#endif /* SAMBAG_Image_H */
