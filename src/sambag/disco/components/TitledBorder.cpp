/*
 * TitledBorder.cpp
 *
 *  Created on: Sun Dec  2 12:56:15 2012
 *      Author: Johannes Unger
 */

#include "TitledBorder.hpp"
#include "ui/ALookAndFeel.hpp"
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/components/AComponent.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class TitledBorder
//=============================================================================
//-----------------------------------------------------------------------------
TitledBorder::TitledBorder() {
}
//-----------------------------------------------------------------------------
Rectangle TitledBorder::getTextBounds(AComponentPtr c,
    IDrawContext::Ptr cn, const Rectangle &r)
{
    Rectangle res = cn->textExtends(c->getName());
    res.x(15);
    res.y(5);
    return res;
}
//-----------------------------------------------------------------------------
void TitledBorder::paintBorder(AComponentPtr c,IDrawContext::Ptr cn,
    const Rectangle &r)
{
    style.intoContext(cn);
    Rectangle textBounds = getTextBounds(c, cn, r);
    Coordinate tx = textBounds.x();
    Coordinate ty = textBounds.y();
    Coordinate tw = textBounds.width();
    Coordinate th = textBounds.height();
    Coordinate cx = c->getX();
    Coordinate cy = c->getY();
    
    IImageSurface::Ptr bff = getDiscoFactory()->createImageSurface(tw, th);
    IDrawContext::Ptr bffCn = getDiscoFactory()->createContext(bff);
    cn->copyAreaTo(bffCn, textBounds, Point2D(0,0));
    Super::paintBorder(c, cn, r);
    bffCn->copyAreaTo(cn, Rectangle(0,0,tw, th), Point2D(tx, ty));
    //bffCn->copyTo(cn);
    cn->moveTo(textBounds.x0());
    cn->textPath(c->getName());
    cn->setFillColor(ColorRGBA(0));
    cn->fill();
}
}}} // namespace(s)
