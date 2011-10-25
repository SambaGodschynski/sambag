/*
 * GraphicElement.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef GRAPHICELEMENT_HPP_
#define GRAPHICELEMENT_HPP_

#include "sambag/disco/IDrawContext.hpp"
#include "Style.hpp"
#include "sambag/com/Common.hpp"
#include "sambag/disco/IDrawable.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// Abstract base class for all graphic elements.
class GraphicElement : public IDrawable {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<GraphicElement> Ptr;
private:
	//-------------------------------------------------------------------------
	/**
	 * the object transformations matrix
	 */
	sambag::com::Matrix tMatrix;
protected:
	//-------------------------------------------------------------------------
	Style style;
	//-------------------------------------------------------------------------
	GraphicElement();
	//-------------------------------------------------------------------------
	boost::weak_ptr<GraphicElement> __self;
	//-------------------------------------------------------------------------
	void __setSelf( Ptr self ) { __self = self; }
public:
	//-------------------------------------------------------------------------
	std::string toString() const = 0;
	//-------------------------------------------------------------------------
	virtual Ptr clone() const = 0;
	//-------------------------------------------------------------------------
	virtual const sambag::com::Matrix & getTransformMatrixDeprecated() const {
		return tMatrix;
	}
	//-------------------------------------------------------------------------
	virtual void setTransformMatrix(const sambag::com::Matrix &m) {
		if (m.size1() != 3 || m.size2() != 3)
			return;
		tMatrix = m;
	}
	//-------------------------------------------------------------------------
	virtual const Style & getStyle() const { return style; }
	//-------------------------------------------------------------------------
	/**
	 * @see Style.copyFrom()
	 * @param _style
	 */
	virtual void copyStyleFrom( const Style &b ) { style.copyFrom(b); }
	//-------------------------------------------------------------------------
	virtual void setStyle( const Style &_style ) { style =_style; }
	//-------------------------------------------------------------------------
	virtual ~GraphicElement();
	//-------------------------------------------------------------------------
	virtual void draw( IDrawContext::Ptr context ) = 0;
	//-------------------------------------------------------------------------
	virtual Rectangle getBoundingBox() const = 0;
};

}}} // namespace

#endif /* GRAPHICELEMENT_HPP_ */
