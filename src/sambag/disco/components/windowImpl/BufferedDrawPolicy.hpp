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
	/** 
	 *  @note draw policy concept
	 */
	void processDraw(ISurface::Ptr surface);
	//-------------------------------------------------------------------------
	/** 
	 *  @note draw policy concept
	 */
	void init(components::RootPane::Ptr root);
	//-------------------------------------------------------------------------
	/** 
	 *  @note draw policy concept
	 */
	void reinit(components::RootPane::Ptr root) {
		if (bff)
			if (bff->getSize().getDimension() ==
					root->getBounds().getDimension())
				return;
		init(root);
	}
	//-------------------------------------------------------------------------
	/** 
	 *  @note draw policy concept
	 */
	void close() {
		root.reset();
		bff.reset();
	}
private:
	//-------------------------------------------------------------------------
	Timer::Ptr updateTimer;
	//-------------------------------------------------------------------------
	RootPane::Ptr root;
	//-------------------------------------------------------------------------
	void redrawRoot(sambag::disco::ISurface::Ptr sf);
	//-------------------------------------------------------------------------
	sambag::disco::ISurface::Ptr bff;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<bool, true> needUpdate;
public:
	//-------------------------------------------------------------------------
	BufferedDrawPolicy();
	//-------------------------------------------------------------------------
	/** 
	 *  @note draw policy concept
	 */
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
inline void BufferedDrawPolicy::redrawRoot(sambag::disco::ISurface::Ptr sf) {
	if (!bff || !root)
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
		throw;
	}
}
//-----------------------------------------------------------------------------
inline void BufferedDrawPolicy::processDraw(ISurface::Ptr surface)
{
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	redrawRoot(surface);
	IDrawContext::Ptr cn =
			getDiscoFactory()->createContext(surface);
	IDrawContext::Ptr bffcn =
			getDiscoFactory()->createContext(bff);
	Rectangle clip = surface->getClipRect();
	bffcn->copyAreaTo( cn, clip, clip.x0());
	//cn->drawSurface(bff);
}
//-----------------------------------------------------------------------------
inline void BufferedDrawPolicy::init(components::RootPane::Ptr root)
{
	using namespace components;
	SAMBAG_ASSERT(root);
	Dimension dim = root->getBounds().getDimension();
	bff = sambag::disco::
			getDiscoFactory()->
				createImageSurface((int)dim.width(), (int)dim.height());
	root->setSurface(bff);
	this->root = root;
	update();
}
}}} // namespace(s)

#endif /* DEFAULTDRAWPOLICY_HPP_ */
