/*
 * TitledBorder.hpp
 *
 *  Created on: Sun Dec  2 12:56:15 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_TITLEDBORDER_H
#define SAMBAG_TITLEDBORDER_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/svg/Style.hpp>
#include "IBorder.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class TitledBorder.
  */
class TitledBorder : public IBorder {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef IBorder Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<TitledBorder> Ptr;
protected:
	//-------------------------------------------------------------------------
	IBorder::Ptr border;
	//-------------------------------------------------------------------------
	TitledBorder();
	//-------------------------------------------------------------------------
	Rectangle getTextBounds(AComponentPtr c,
        IDrawContext::Ptr g, const Rectangle &r);
	//-------------------------------------------------------------------------
	typedef svg::Style Style;
	Style style;
	Coordinate cornerRadius;
	//-------------------------------------------------------------------------
	std::string getText(AComponentPtr c, const Rectangle &size) const;
	//-------------------------------------------------------------------------
	Point2D getTextPos(AComponentPtr c, const Rectangle &size) const;
private:
public:
	//-------------------------------------------------------------------------
	void setStyle(const Style &style);
	//-------------------------------------------------------------------------
	const Style & getStyle() const {
		return style;	
	}
	//-------------------------------------------------------------------------
	void setBorder(IBorder::Ptr border);
	//-------------------------------------------------------------------------
	IBorder::Ptr getBorder();
	//-------------------------------------------------------------------------
	virtual Insets getBorderInsets(AComponentPtr c);
	//-------------------------------------------------------------------------
	virtual bool isBorderOpaque();
	//-------------------------------------------------------------------------
    static Ptr create() {
        return Ptr(new TitledBorder);
    }
	//-------------------------------------------------------------------------
	virtual void paintBorder(AComponentPtr c,
        IDrawContext::Ptr g, const Rectangle &r);

}; // TitledBorder
}}} // namespace(s)

#endif /* SAMBAG_TITLEDBORDER_H */
