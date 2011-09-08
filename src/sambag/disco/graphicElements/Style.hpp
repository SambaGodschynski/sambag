/*
 * Style.h
 *
 *  Created on: 19.09.2011
 *      Author: samba
 */

#ifndef STYLE_H_
#define STYLE_H_

#include "sambag/disco/IDrawContext.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// A container for styling information.
struct Style {
//=============================================================================
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>properties
	Number stokeWidth;
	IDrawContext::LineCapStyle lineCapStyle;
	IDrawContext::FillRule fillRule;
	ColorRGBA strokeColor;
	ColorRGBA fillColor;
	IDrawContext::Dash dash;
	bool isFilled;
	bool isStroked;
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>methods
	//-------------------------------------------------------------------------
	Style() :
		stokeWidth(1.0),
		lineCapStyle(IDrawContext::LINE_CAP_BUTT),
		fillRule(IDrawContext::FILL_RULE_WINDING),
		strokeColor( ColorRGBA(0,0,0,1.0) ),
		dash(NULL,0,0),
		isFilled(false),
		isStroked(true)
	{
	}
	//-------------------------------------------------------------------------
	void setStrokeStyle( IDrawContext::Ptr cn ) {
		cn->setStrokeWidth(stokeWidth);
		cn->setLineCap(lineCapStyle);
		if (dash.get<0>()!=NULL)
			cn->setDash(dash);
		cn->setSourceColor(strokeColor);
	}
	//-------------------------------------------------------------------------
	void setFillStyle( IDrawContext::Ptr cn ) {
		cn->setFillRule(fillRule);
		cn->setSourceColor(fillColor);
	}

};
}}} // namespace

#endif /* STYLE_H_ */
