/*
 * DefaultDrawPolicy.hpp
 *
 *  Created on: Jun 9, 2012
 *      Author: samba
 */

#ifndef DEFAULTDRAWPOLICY_HPP_
#define DEFAULTDRAWPOLICY_HPP_

/*
 * WindowImpl.hpp
 *
 *  Created on: Jun 8, 2012
 *      Author: samba
 */
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/components/RedrawManager.hpp>
#include <sambag/disco/components/Forward.hpp>
#include <sambag/disco/components/RootPane.hpp>

namespace sambag { namespace disco {
//=============================================================================
/**
  * @class BufferedDrawPolicy.
  * draws window impl. using a buffer.
  */
class BufferedDrawPolicy {
//=============================================================================
protected:
	//-------------------------------------------------------------------------
	void processDraw(components::RootPane::Ptr root, ISurface::Ptr surface);
	//-------------------------------------------------------------------------
	void clearBuffer();
	//-------------------------------------------------------------------------
	void init(components::RootPane::Ptr root, ISurface::Ptr surface);
	//-------------------------------------------------------------------------
	void reinit(components::RootPane::Ptr root, ISurface::Ptr surface) {
		if (bff)
			if (bff->getSize().getDimension() ==
					root->getBounds().getDimension())
				return;
		init(root, surface);
	}
private:
	//-------------------------------------------------------------------------
	sambag::disco::IImageSurface::Ptr bff;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<bool, true> needUpdate;
public:
	//-------------------------------------------------------------------------
	void update() {
		needUpdate = true;
	}

}; // X11Window
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
inline void BufferedDrawPolicy::clearBuffer() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	IDrawContext::Ptr cn = getDiscoFactory()->createContext(bff);
	cn->setFillColor(ColorRGBA());
	cn->rect(bff->getSize());
	cn->fill();
}
//-----------------------------------------------------------------------------
inline void BufferedDrawPolicy::processDraw(components::RootPane::Ptr root,
		ISurface::Ptr surface)
{
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	IDrawContext::Ptr cn =
			getDiscoFactory()->createContext(surface);
	if (needUpdate) {
		needUpdate=false;
		root->draw(root->getDrawContext());
		return;
	}
	RedrawManager::currentManager(root)->drawDirtyRegions();
	cn->drawSurface(bff);
}
//-----------------------------------------------------------------------------
inline void BufferedDrawPolicy::init(components::RootPane::Ptr root,
		ISurface::Ptr surface)
{
	using namespace components;
	Dimension dim = root->getBounds().getDimension();
	bff = sambag::disco::
			getDiscoFactory()->createImageSurface((int)dim.width(), (int)dim.height());
	root->setSurface(bff);
	update();
}
}} // namespace(s)

#endif /* DEFAULTDRAWPOLICY_HPP_ */
