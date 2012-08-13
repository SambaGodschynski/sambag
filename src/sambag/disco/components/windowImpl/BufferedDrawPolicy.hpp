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
#include <sambag/disco/components/Timer.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
/**
  * @class BufferedDrawPolicy.
  * draws window impl. using a buffer.
  */
class BufferedDrawPolicy {
//=============================================================================
protected:
	//-------------------------------------------------------------------------
	void processDraw(ISurface::Ptr surface);
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
	Timer::Ptr updateTimer;
	//-------------------------------------------------------------------------
	RootPane::Ptr root;
	//-------------------------------------------------------------------------
	void redrawRoot();
	//-------------------------------------------------------------------------
	sambag::disco::IImageSurface::Ptr bff;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<bool, true> needUpdate;
public:
	//-------------------------------------------------------------------------
	BufferedDrawPolicy();
	//-------------------------------------------------------------------------
	void update() {
		needUpdate = true;
	}

}; // X11Window
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
inline BufferedDrawPolicy::BufferedDrawPolicy() {
	/*updateTimer = Timer::create(10);
	updateTimer->EventSender<Timer::Event>::addEventListener(
		boost::bind(&BufferedDrawPolicy::onUpdateTimer, this)
	);
	updateTimer->setNumRepetitions(-1);
	updateTimer->start();*/
}
//-----------------------------------------------------------------------------
inline void BufferedDrawPolicy::redrawRoot() {
	if (!bff || !root)
		return;
	SAMBAG_BEGIN_SYNCHRONIZED( root->getTreeLock() )
		if (needUpdate) { // sometimes we need a full redraw
			needUpdate=false;
			RedrawManager::currentManager(root)->markCompletelyDirty(root);
		}
		RedrawManager::currentManager(root)->drawDirtyRegions();
	SAMBAG_END_SYNCHRONIZED
}
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
inline void BufferedDrawPolicy::processDraw(ISurface::Ptr surface)
{
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	redrawRoot();
	IDrawContext::Ptr cn =
			getDiscoFactory()->createContext(surface);
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
	this->root = root;
	update();
}
}}} // namespace(s)

#endif /* DEFAULTDRAWPOLICY_HPP_ */
