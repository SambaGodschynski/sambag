/*
 * SolidBorder.hpp
 *
 *  Created on: Sun Dec  2 11:07:33 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SOLIDBORDER_H
#define SAMBAG_SOLIDBORDER_H

#include <boost/shared_ptr.hpp>
#include "IBorder.hpp"
#include <sambag/disco/svg/graphicElements/Style.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class SolidBorder.
  */
class SolidBorder : public IBorder {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef IBorder Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SolidBorder> Ptr;
protected:
	//-------------------------------------------------------------------------
	SolidBorder();
private:
	//-------------------------------------------------------------------------
	typedef svg::graphicElements::Style Style;
	Style style;
	Coordinate cornerRadius;
public:
	//-------------------------------------------------------------------------
	void setCornerRadius(const Coordinate &c);
	//-------------------------------------------------------------------------
	const Coordinate & getCornerRadius() const {
		return cornerRadius;	
	}
	//-------------------------------------------------------------------------
	void setStyle(const Style &style);
	//-------------------------------------------------------------------------
	const Style & getStyle() const {
		return style;	
	}
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new SolidBorder());	
	}
	//-------------------------------------------------------------------------
	virtual Insets getBorderInsets(AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual bool isBorderOpaque();
	//-------------------------------------------------------------------------
	virtual void paintBorder(AComponentPtr c,
			IDrawContext::Ptr g, const Rectangle &r);

}; // SolidBorder
}}} // namespace(s)

#endif /* SAMBAG_SOLIDBORDER_H */
