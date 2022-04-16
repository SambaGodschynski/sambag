/*
 * Image.cpp
 *
 *  Created on: 12.10.2011
 *      Author: samba
 */

#include "Image.hpp"
#include "sambag/com/FileHandler.hpp"
#include "sambag/disco/IDiscoFactory.hpp"
#include <boost/filesystem.hpp>
#include <sambag/disco/svg/Image.hpp>

namespace sambag { namespace disco { namespace svg { namespace graphicElements {

namespace {
    ISurface::Ptr _loadSvg(const std::string &path, const Dimension &r) {
        try {
            svg::Image::Ptr image = svg::Image::create();
            image->setSvgPath(path);
            image->setSize(r, true);
            ISurface::Ptr sf = getDiscoFactory()->createRecordingSurface();
            IDrawContext::Ptr cn = getDiscoFactory()->createContext(sf);
            cn->rect(r);
            cn->clip();
            image->draw(cn);
            return sf;
        } catch(...) {
        }
        return ISurface::Ptr();
    }
}

//=============================================================================
// class Image
//=============================================================================
//-----------------------------------------------------------------------------
void Image::loadImage(const Dimension &r) {
    if (boost::filesystem::path(uri).extension().string()==".svg") {
        image = _loadSvg(uri, r);
        return;
    }
	com::FileHandler::Ptr fh = com::FileHandler::create(uri);
	image = getDiscoFactory()->createImageSurface(fh);
}
//-----------------------------------------------------------------------------
void Image::drawPlaceholder(IDrawContext::Ptr cn) {
	cn->setFillColor(ColorRGBA());
	cn->rect(outline.solve(cn));
	cn->fill();
}
//-----------------------------------------------------------------------------
void Image::setSurface(ISurface::Ptr sf) {
	image = sf;
}
//-----------------------------------------------------------------------------
void Image::draw( IDrawContext::Ptr cn ) {
    Rectangle outline = this->outline.solve(cn);
	if (!image) {
		loadImage(outline.size());
		if (!image) { // loading failed
			drawPlaceholder(cn);
			return;
		}
	}
	cn->translate(outline.x0());
	Rectangle orgSz = image->getSize();
	cn->scale( Point2D(
		outline.getWidth()/orgSz.getWidth(),
		outline.getHeight()/orgSz.getHeight()
	));
	cn->drawSurface(image, opacity);
}
}}}}
