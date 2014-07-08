/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef GRAPHICELEMENT_HPP_
#define GRAPHICELEMENT_HPP_

#include "sambag/disco/IDrawContext.hpp"
#include "sambag/com/Common.hpp"
#include "sambag/disco/IDrawable.hpp"
#include "sambag/disco/Coordinate.hpp"

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
/**
 * @class GraphicElement
 * @brief Abstract base class for all graphic elements.
 */
class GraphicElement : public virtual IDrawable {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<GraphicElement> Ptr;
private:
    //-------------------------------------------------------------------------
    std::string userData;
protected:
	//-------------------------------------------------------------------------
	GraphicElement();
	//-------------------------------------------------------------------------
	boost::weak_ptr<GraphicElement> __self;
	//-------------------------------------------------------------------------
	void __setSelf( Ptr self ) { __self = self; }
public:
	//-------------------------------------------------------------------------
	virtual Ptr clone() const = 0;
	//-------------------------------------------------------------------------
	virtual ~GraphicElement();
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return __self.lock();
	}
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox(IDrawContext::Ptr context);
    //-------------------------------------------------------------------------
    void setUserData(const std::string &x) {
        userData = x;
    }
    //-------------------------------------------------------------------------
    std::string getUserData() const {
        return userData;
    }
    
};

}}}} // namespace

#endif /* GRAPHICELEMENT_HPP_ */
