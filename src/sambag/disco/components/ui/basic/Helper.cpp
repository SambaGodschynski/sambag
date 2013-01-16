/*
 * Helper.cpp
 *
 *  Created on: Tue Jan 15 10:36:38 2013
 *      Author: Johannes Unger
 */

#include "Helper.hpp"
#include <sambag/disco/genFormatter/GenFlowLayout.hpp>
#include <sambag/disco/genFormatter/RectangleAccess.hpp>
#include <sambag/disco/genFormatter/GenericFormatter.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
namespace {
static const sambag::com::Number GAP= 5.;
void drawIcon(IDrawContext::Ptr cn,
	ISurface::Ptr icon, 
	const Rectangle &bounds) 
{
	if (!icon) {
		return;
	}
	cn->save();
	Rectangle realBounds = icon->getSize();
	if (realBounds.width() <= 0. || realBounds.height() <= 0.)
		return;
	sambag::com::Number scaleX = bounds.width() / realBounds.width();
	sambag::com::Number scaleY = bounds.height() / realBounds.height();
	cn->translate(bounds.x0());
	cn->scale(Point2D(scaleX, scaleY));
	cn->drawSurface(icon);	
	cn->restore();
}
} // namespace(s)
//=============================================================================
//  Class Helper
//=============================================================================
//-----------------------------------------------------------------------------
Rectangle Helper::calcIconBounds(ISurface::Ptr icon, const Coordinate &textHeight) 
{
	if (!icon) {
		return Rectangle(0,0,0,0);	
	}
	Rectangle res(0,0,textHeight,textHeight);
	return res;
}
//-----------------------------------------------------------------------------
void Helper::drawIconAndText(IDrawContext::Ptr cn, ISurface::Ptr icon, 
	std::string &str, const Rectangle &bounds, bool hCenter, bool vCenter)
{
	using namespace sambag::disco::genFormatter;
	Rectangle txtEx = cn->textExtends(str);
	Number cfh = cn->getCurrentFontHeight();
	Rectangle iconBounds = calcIconBounds(icon, cfh);

	typedef GenericFormatter< Rectangle,
		RectangleAccess,
		GenFlowLayout
	> Formatter;
	Formatter form;

	form.setAlignment(Formatter::LEFT); //do not use center without specific width
	form.setX(bounds.x());
	form.setY(bounds.y());
	form.addComponent(&iconBounds);	
	form.addComponent(&txtEx);	
	form.layout();

	Coordinate x, y, w, h;
	form.currentBounds(x,y,w,h);

	if (hCenter) {
		Coordinate offX = (bounds.width() - w) / 2.0;
		iconBounds.x ( iconBounds.x() + offX );
		txtEx.x ( txtEx.x() + offX );
	}

	if (vCenter) {
		Coordinate offY = (bounds.height() - h) / 2.0;
		iconBounds.y ( iconBounds.y() + offY );
		txtEx.y ( txtEx.y() + offY );
	}

	drawIcon(cn, icon, iconBounds);
	cn->moveTo(Point2D(
		txtEx.x(), txtEx.y() + txtEx.height()
	));
	cn->textPath(str);
	cn->fill();
}
}}}}} // namespace(s)
