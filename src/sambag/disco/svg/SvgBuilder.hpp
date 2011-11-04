/*
 * SvgBuilder.hpp
 *
 *  Created on: 20.09.2011
 *      Author: samba
 */

#ifndef SVGBUILDER_HPP_
#define SVGBUILDER_HPP_

#include "Svg.hpp"
#include "sambag/xml/XML2Obj.hpp"
#include <string>


namespace sambag { namespace disco { namespace svg {
//=============================================================================
/**
 * @class SvgBuilder set Xml2Object up to
 * create SVGObjects using a xml respectively a svg file.
 */
class SvgBuilder {
//=============================================================================
private:
	//-------------------------------------------------------------------------
	SvgObject::BuilderType xml2Obj;
protected:
	//-------------------------------------------------------------------------
	/**
	 * registers the needed svg class structure in xml2Object
	 */
	void registerSvgClasses();
	//-------------------------------------------------------------------------
	/**
	 * registers the needed svg attribute tags in xml2Object
	 */
	void registerSvgAttributes();
public:
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param  svg string
	 * @return the root SvgGraphicElement
	 */
	SvgObject::Ptr buildSvgFromString( const std::string & );
	//-------------------------------------------------------------------------
	/**
	* TODO: handle use case with one stream instead of string or file.
	* @param  svg filename
	* @return the root SvgGraphicElement
	*/
	SvgObject::Ptr buildSvgFromFilename( const std::string & );
	//-------------------------------------------------------------------------
	SvgBuilder();
	//-------------------------------------------------------------------------
	virtual ~SvgBuilder();
	//-------------------------------------------------------------------------
};

}}}

#endif /* SVGBUILDER_HPP_ */
