/*
 * Win32BufferedDraw.cpp
 *
 *  Created on: Sun Jan 27 13:04:06 2013
 *      Author: Johannes Unger
 */

#include "Win32BufferedDraw.hpp"

#ifdef DISCO_USE_WIN32

#include <windows.h>
#include <sambag/disco/CairoSurface.hpp>
#include <cairo-win32.h>
#include <sambag/disco/Win32Surface.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class Win32BufferedDraw
//=============================================================================
//-----------------------------------------------------------------------------
Win32BufferedDraw::Win32BufferedDraw() {
}
//-----------------------------------------------------------------------------
void Win32BufferedDraw::redrawRoot(sambag::disco::ISurface::Ptr sf) {
	if (!root)
		return;
	try {
		SAMBAG_BEGIN_SYNCHRONIZED( root->getTreeLock() )
			if (needUpdate) { // sometimes we need a full redraw
				needUpdate=false;
				root->draw(root->getDrawContext());
				RedrawManager::currentManager(root)->markCompletelyClean(root);
				// return; Cubase/Live8 as nested window(win32) -> need to drawDirtyRegions()
				// otherwise redrawing is locked until system sends WM_PAINT
			}
			RedrawManager::currentManager(root)->drawDirtyRegions();
		SAMBAG_END_SYNCHRONIZED
	} catch (const std::exception &ex) {
		std::cout<<ex.what()<<std::endl;
		exit(0);
	}
}
//-----------------------------------------------------------------------------
void Win32BufferedDraw::processDraw(ISurface::Ptr surface) {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	redrawRoot(surface);
	IDrawContext::Ptr cn =
			getDiscoFactory()->createContext(surface);
	IDrawContext::Ptr bffcn =
			getDiscoFactory()->createContext(bff);
	Rectangle clip = surface->getClipRect();
	bffcn->copyAreaTo( cn, clip, clip.x0());
}
//-----------------------------------------------------------------------------
namespace {
	class Win32ImageBuffer : public CairoImageSurface {
	public:
		typedef boost::shared_ptr<Win32ImageBuffer> Ptr;
	private:
		Win32ImageBuffer(cairo_surface_t *s) : CairoImageSurface(s){}
		// it seems that image surfaces created via 
		// cairo_win32_surface_create_with_dib
		// width = height = 0 queryed with cairo_image_surface_get_width/height
		Rectangle bounds;
	public:
		static IImageSurface::Ptr create(Integer width, Integer height);
		virtual Rectangle getSize() const;
	};
	///////////////////////////////////////////////////////////////////////////
	//-------------------------------------------------------------------------
	Rectangle Win32ImageBuffer::getSize() const {
		return bounds;
	}
	//-------------------------------------------------------------------------
	IImageSurface::Ptr Win32ImageBuffer::create(Integer width, Integer height)
	{
		cairo_surface_t *s = cairo_win32_surface_create_with_dib(
			CAIRO_FORMAT_RGB24, width, height
		);
		Ptr res(new Win32ImageBuffer(s));
		res->bounds = Rectangle(0,0,width,height);
		return res;
	}
} // namespace(s)
void Win32BufferedDraw::init(components::RootPane::Ptr root) {
	using namespace components;
	SAMBAG_ASSERT(root);
	Dimension dim = root->getBounds().getDimension();
	bff = Win32ImageBuffer::create((Integer)dim.width(), (Integer)dim.height());
	root->setSurface(bff);
	this->root = root;
	update();
}

}}} // namespace(s)


#endif //#ifdef DISCO_USE_WIN32