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
#include <sambag/disco/Forward.hpp>
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
	void init(components::RootPane::Ptr root, ISurface::Ptr surface);
	//-------------------------------------------------------------------------
	void reinit(components::RootPane::Ptr root, ISurface::Ptr surface) {
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
inline void BufferedDrawPolicy::processDraw(components::RootPane::Ptr root,
		ISurface::Ptr surface)
{
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	IDrawContext::Ptr cn =
			getDiscoFactory()->createContext(surface);
	if (needUpdate) {
		root->draw(root->getDrawContext());
		needUpdate=false;
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
			getDiscoFactory()->createImageSurface(dim.width(), dim.height());
	root->setSurface(bff);
	update();
}
}} // namespace(s)

#endif /* DEFAULTDRAWPOLICY_HPP_ */
