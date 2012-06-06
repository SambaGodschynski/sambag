/*
 * HtmlColors.hpp
 *
 *  Created on: Jun 5, 2012
 *      Author: samba
 */

#ifndef SAMBAG_HTMLCOLORS_HPP_
#define SAMBAG_HTMLCOLORS_HPP_

#include <string>
#include <sambag/disco/ColorRGBA.hpp>
#include <boost/unordered_map.hpp>

namespace sambag { namespace disco { namespace svg {
//=============================================================================
/**
 * @class HtmlColors.
 */
class HtmlColors {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<std::string, ColorRGBA> ColorMap;
	//-------------------------------------------------------------------------
	static ColorMap colorMap;
	//-------------------------------------------------------------------------
	static void initHtmlColors();
public:
	//-------------------------------------------------------------------------
	static const ColorRGBA & getColor( const std::string &name );
};

}}}


#endif /* HTMLCOLORS_HPP_ */
