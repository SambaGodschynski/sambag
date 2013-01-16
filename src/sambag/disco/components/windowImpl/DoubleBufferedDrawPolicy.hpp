/*
 * DoubleBufferedDrawPolicy.hpp
 *
 *  Created on: Wed Jan 16 20:39:17 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DOUBLEBUFFEREDDRAWPOLICY_H
#define SAMBAG_DOUBLEBUFFEREDDRAWPOLICY_H

#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/components/RedrawManager.hpp>
#include <sambag/disco/components/Forward.hpp>
#include <sambag/disco/components/RootPane.hpp>
#include <sambag/com/Thread.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
/** 
  * @class DoubleBufferedDrawPolicy.
  * draws window impl. double buffered.
  */
class DoubleBufferedDrawPolicy {
//=============================================================================
protected:
	//-------------------------------------------------------------------------
	Dimension currDim;
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
		if (currDim == root->getBounds().getDimension()) {
			return;
		}
		init(root);
	}
	//-------------------------------------------------------------------------
	/** 
	 *  @note draw policy concept
	 */
	void close();
private:
	//-------------------------------------------------------------------------
	bool running;
	//-------------------------------------------------------------------------
	boost::thread updateThread;
	//-------------------------------------------------------------------------
	void onUpdateThread();
	//-------------------------------------------------------------------------
	typedef std::pair<sambag::com::Mutex *, sambag::disco::ISurface::Ptr>
		BufferAccessData;
	//-------------------------------------------------------------------------
	struct BufferAccess {
		sambag::com::Mutex *mutex;
		sambag::disco::ISurface::Ptr bff;
		BufferAccess(BufferAccessData&);
		~BufferAccess();
		sambag::disco::ISurface::Ptr getBuffer() { return bff; }
	};
	//-------------------------------------------------------------------------
	sambag::com::RecursiveMutex bufferAccessLock;
	//-------------------------------------------------------------------------
	sambag::disco::ISurface::Ptr bff[2];
	//-------------------------------------------------------------------------
	sambag::com::Mutex bufferMutex[2];
	//-------------------------------------------------------------------------
	bool accessPlan[2];
	//-------------------------------------------------------------------------
	BufferAccessData getBufferAccessData();
	//-------------------------------------------------------------------------
	RootPane::Ptr root;
	//-------------------------------------------------------------------------
	void redrawRoot();
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<bool, true> needUpdate;
public:
	//-------------------------------------------------------------------------
	DoubleBufferedDrawPolicy();
	//-------------------------------------------------------------------------
	~DoubleBufferedDrawPolicy() {
		close();
	}
	//-------------------------------------------------------------------------
	/** 
	 *  @note draw policy concept
	 */
	void update() {
		needUpdate = true;
	}
}; // DoubleBufferedDrawPolicy
}}} // namespace(s)

#endif /* SAMBAG_DOUBLEBUFFEREDDRAWPOLICY_H */
