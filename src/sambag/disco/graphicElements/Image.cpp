/*
 * Image.cpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#include "Image.hpp"
#include "sambag/com/FileHandler.hpp"

namespace sambag { namespace disco { namespace graphicElements {
//=============================================================================
// class Image
//=============================================================================
//-----------------------------------------------------------------------------
void Image::loadImage(IDrawContext::Ptr cn) {
	com::FileHandler::Ptr fh = com::FileHandler::create(uri);
	image = cn->createPngSurface(fh);
}
//-----------------------------------------------------------------------------
void Image::drawPlaceholder(IDrawContext::Ptr cn) {
	cn->setSourceColor(ColorRGBA());
	cn->rect(outline);
	cn->fill();
}
//-----------------------------------------------------------------------------
void Image::draw( IDrawContext::Ptr cn ) {
	if (!image) {
		loadImage(cn);
		if (!image) { // loading failed
			drawPlaceholder(cn);
			return;
		}
	}

	Matrix tmp;
	cn->getMatrix(tmp);
	cn->translate(outline.getX0());
	Rectangle orgSz = image->getSize();
	cn->scale( Point2D(
		outline.getWidth()/orgSz.getWidth(),
		outline.getHeight()/orgSz.getHeight()
	));
	cn->drawSurface(image, getStyle().opacity());

	cn->identityMatrix();
	cn->transform(tmp);
}
}}}
