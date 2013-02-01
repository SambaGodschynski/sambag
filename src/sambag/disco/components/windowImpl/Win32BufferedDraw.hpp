/*
 * Win32BufferedDraw.hpp
 *
 *  Created on: Sun Jan 27 13:04:06 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_WIN32BUFFEREDDRAW_H
#define SAMBAG_WIN32BUFFEREDDRAW_H

#ifdef DISCO_USE_WIN32

#include <boost/shared_ptr.hpp>
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/components/RedrawManager.hpp>
#include <sambag/disco/components/Forward.hpp>
#include <sambag/disco/components/RootPane.hpp>

namespace sambag { namespace disco { namespace components { 
//=============================================================================
/** 
  * @class Win32BufferedDraw.
  */
class Win32BufferedDraw {
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
		init(root);
	}
	//-------------------------------------------------------------------------
	/** 
	 *  @note draw policy concept
	 */
	void close() {
		root.reset();
	}
private:
	//-------------------------------------------------------------------------
	RootPane::Ptr root;
	//-------------------------------------------------------------------------
	void redrawRoot(sambag::disco::ISurface::Ptr sf);
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<bool, true> needUpdate;
	//-------------------------------------------------------------------------
	sambag::disco::ISurface::Ptr bff;
public:
	//-------------------------------------------------------------------------
	Win32BufferedDraw();
	//-------------------------------------------------------------------------
	/** 
	 *  @note draw policy concept
	 */
	void update() {
		needUpdate = true;
	}
}; // Win32BufferedDraw
}}} // namespace(s)

#endif //#ifdef DISCO_USE_WIN32
#endif /* SAMBAG_WIN32BUFFEREDDRAW_H */
