/*
 * DoubleBufferedDrawPolicy.cpp
 *
 *  Created on: Wed Jan 16 20:39:17 2013
 *      Author: Johannes Unger
 */

#include "DoubleBufferedDrawPolicy.hpp"
#include <sambag/com/exceptions/IllegalStateException.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
//  Class DoubleBufferedDrawPolicy::BufferAccess
//=============================================================================
DoubleBufferedDrawPolicy::BufferAccess::BufferAccess(BufferAccessData &ba) {
	
	boost::tie(mutex, bff) = ba;
}
//-----------------------------------------------------------------------------
DoubleBufferedDrawPolicy::BufferAccess::~BufferAccess() {
	mutex->unlock();
}
//=============================================================================
//  Class DoubleBufferedDrawPolicy
//=============================================================================
//-----------------------------------------------------------------------------
DoubleBufferedDrawPolicy::BufferAccessData 
DoubleBufferedDrawPolicy::getBufferAccessData() 
{
	SAMBAG_BEGIN_SYNCHRONIZED(bufferAccessLock)
		if ( bufferMutex[0].try_lock() ) {
			return BufferAccessData(&bufferMutex[0], bff[0]);
		}
		if ( bufferMutex[1].try_lock() ) {
			return BufferAccessData(&bufferMutex[1], bff[1]);
		}
	SAMBAG_END_SYNCHRONIZED
	SAMBAG_THROW(
		sambag::com::exceptions::IllegalStateException,
		"double buffer creation failed"
	);
}
//-----------------------------------------------------------------------------
DoubleBufferedDrawPolicy::DoubleBufferedDrawPolicy() {
}
//-----------------------------------------------------------------------------
void DoubleBufferedDrawPolicy::onUpdateThread() {
	while(running) {
		redrawRoot();
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
	}
}
//-----------------------------------------------------------------------------
void DoubleBufferedDrawPolicy::close() {
	if (!running) {
		return;
	}
	running = false;
	updateThread.join();
	root.reset();
}
//-----------------------------------------------------------------------------
void DoubleBufferedDrawPolicy::redrawRoot() {
	BufferAccess bffA (getBufferAccessData());
	if (!root)
		return;
	try {
		SAMBAG_BEGIN_SYNCHRONIZED( root->getTreeLock() )
			root->setSurface(bffA.getBuffer());
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
void DoubleBufferedDrawPolicy::processDraw(ISurface::Ptr surface)
{
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	BufferAccess bffA(getBufferAccessData());
	if (bffA.mutex == &bufferMutex[0]) {
		std::cout<<1<<" ";
	}
	if (bffA.mutex == &bufferMutex[0]) {
		std::cout<<2<<" ";
	}
	IDrawContext::Ptr cn =
			getDiscoFactory()->createContext(surface);
	cn->drawSurface(bffA.getBuffer());
}
//-----------------------------------------------------------------------------
void DoubleBufferedDrawPolicy::init(components::RootPane::Ptr root)
{
	SAMBAG_BEGIN_SYNCHRONIZED(bufferAccessLock)
		/*boost::unique_lock<sambag::com::Mutex> lock01 (bufferMutex[0], boost::try_to_lock);
		if (!lock01.owns_lock()) { 
			lock01.timed_lock(boost::get_system_time() + boost::posix_time::seconds(5)); 
		}
		if ( !lock01.owns_lock() ) {
			SAMBAG_THROW(SAMBAG_DEADLOCK_EXCEPTION, "deadlock exception");
		}

		boost::unique_lock<sambag::com::Mutex> lock02 (bufferMutex[1], boost::try_to_lock);
		if (!lock02.owns_lock()) { 
			lock02.timed_lock(boost::get_system_time() + boost::posix_time::seconds(5)); 
		}
		if ( !lock02.owns_lock() ) {
			SAMBAG_THROW(SAMBAG_DEADLOCK_EXCEPTION, "deadlock exception");
		}*/


		using namespace components;
		SAMBAG_ASSERT(root);
		Dimension dim = root->getBounds().getDimension();
		IDiscoFactory *fac = sambag::disco::getDiscoFactory();
		bff[0] = fac->createImageSurface((int)dim.width(), (int)dim.height());
		bff[1] = fac->createImageSurface((int)dim.width(), (int)dim.height());
		this->root = root;
		update();
	SAMBAG_END_SYNCHRONIZED
	running = true;
	updateThread = boost::thread(
		boost::bind(&DoubleBufferedDrawPolicy::onUpdateThread, this)
	);
}
}}} // namespace(s)
