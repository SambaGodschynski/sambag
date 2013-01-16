/*
 * Helper.hpp
 *
 *  Created on: Tue Jan 15 10:36:38 2013
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_HELPER_H
#define SAMBAG_HELPER_H

#include <sambag/disco/IDrawContext.hpp>
#include <sambag/disco/Geometry.hpp>
#include <string>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {

//=============================================================================
/** 
  * @class Helper.
  */
struct Helper {
//=============================================================================
	//-------------------------------------------------------------------------
	/**
	 * @return the appropriate bounds of an icon related to a fontsize.
	 * @param the icon
	 * @param the fontsize in px
	 */
	static Rectangle calcIconBounds(ISurface::Ptr icon, const Coordinate &fontsize);
	//-------------------------------------------------------------------------
	/**
	 * draws icon and text.
	 * @param the draw context
	 * @param the icon
	 * @param the text
	 * @param bounds
	 * @param horizontal centered
	 * @param vertical centered
	 */
	static void drawIconAndText(IDrawContext::Ptr cn, 
		ISurface::Ptr icon,
		std::string &text,
		const Rectangle &bounds,
		bool hCenter = true,
		bool vCenter = true);
}; // Helper
}}}}} // namespace(s)

#endif /* SAMBAG_HELPER_H */
