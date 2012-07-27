/*
 * Rect.cpp
 *
 *  Created on: 22.09.2011
 *      Author: samba
 */

#include "Rect.hpp"
#include "sambag/com/Common.hpp"
#include "Path.hpp"
#include "PointContainer.hpp"
#include <boost/assign/list_of.hpp>
#include <fstream>
namespace sambag { namespace disco { namespace svg { namespace graphicElements {

//=============================================================================
// class Rect
//=============================================================================
//-----------------------------------------------------------------------------
Rect::Rect() {

}
//-----------------------------------------------------------------------------
Rect::~Rect() {
}
//-----------------------------------------------------------------------------
void Rect::_rect(IDrawContext::Ptr cn) {
	if (r.x() == 0.0 || r.y() == 0.0 ) {
		cn->rect(rect.solve(cn));
		return;
	}
	/*
	  Mathematically, a 'rect' element can be mapped to an equivalent 'path' element as follows:
	 */
	Rectangle sr = rect.solve(cn);
	Number x = sr.x0().x();
	Number y = sr.x0().y();
	Number w = sr.getWidth();
	Number h = sr.getHeight();
	Number rx = r.x();
	Number ry = r.y();
	using namespace pathInstruction;
	using namespace boost::assign;
	PathInstructions pI;
	// perform an absolute moveto operation to location (x+rx,y),
	PointContainer pC = list_of( x + rx )( y );
	pI.push_back( std::make_pair(pC, MOVETO_ABS));
	pC.clear();
	// perform an absolute horizontal lineto operation to location (x+width-rx,y),
	pC = list_of( x + w - rx);
	pI.push_back( std::make_pair(pC, LINETO_HORIZONTAL_ABS));
	pC.clear();
	//perform an absolute elliptical arc operation to coordinate (x+width,y+ry),
	//where the effective values for the rx and ry attributes on the 'rect'
	//element converted to user space are used as the rx and ry attributes on the elliptical
	//arc command, respectively, the x-axis-rotation is set to zero, the large-arc-flag is set to zero,
	//and the sweep-flag is set to one
	pC = list_of(rx)(ry)(0)(0)(1)(x+w)(y+ry);
	pI.push_back( std::make_pair(pC, ARC_ABS));
	pC.clear();
	//perform a absolute vertical lineto to location (x+width,y+height-ry),
	pC = list_of(y+h-ry);
	pI.push_back( std::make_pair(pC, LINETO_VERTICAL_ABS));
	pC.clear();
	//perform an absolute elliptical arc operation to coordinate (x+width-rx,y+height)
	pC = list_of(rx)(ry)(0)(0)(1)(x+w-rx)(y+h);
	pI.push_back( std::make_pair(pC, ARC_ABS));
	pC.clear();
	//perform an absolute horizontal lineto to location (x+rx,y+height)
	pC = list_of(x + rx);
	pI.push_back( std::make_pair(pC, LINETO_HORIZONTAL_ABS));
	pC.clear();
	//perform an absolute elliptical arc operation to coordinate (x,y+height-ry)
	pC = list_of(rx)(ry)(0)(0)(1)(x)(y+h-ry);
	pI.push_back( std::make_pair(pC, ARC_ABS));
	pC.clear();
	//perform an absolute absolute vertical lineto to location (x,y+ry)
	pC = list_of(y+ry);
	pI.push_back( std::make_pair(pC, LINETO_VERTICAL_ABS));
	pC.clear();
	//perform an absolute elliptical arc operation to coordinate (x+rx,y)
	pC = list_of(rx)(ry)(0)(0)(1)(x+rx)(y);
	pI.push_back( std::make_pair(pC, ARC_ABS));
	// draw path
	// TODO: avoid frequently recreating of path!
	graphicElements::Path::Ptr path = graphicElements::Path::create();
	path->setPathInstructions(pI);
	path->appendPathToContext(cn);
}
//-----------------------------------------------------------------------------
void Rect::draw( IDrawContext::Ptr cn ) {
	if ( cn->isFilled() ) {
		_rect(cn);
		cn->fill();
	}
	if ( cn->isStroked() ) {
		_rect(cn);
		cn->stroke();
	}
}

}}}}
