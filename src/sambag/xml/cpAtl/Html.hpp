/*
 * Html.hpp
 *
 *  Created on: Dec 12, 2011
 *      Author: samba
 */

/**
 * This file is part of the CpATL package which is a port of
 * the JATL library by Adam Gent.
 *
 * http://code.google.com/p/jatl/
 *
 * Copyright (C) 2010 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HTML_HPP_
#define HTML_HPP_

#include <ostream>
#include "GeneratedHtmlBuilder.hpp"
#include <boost/algorithm/string.hpp>

namespace sambag { namespace xml { namespace cpAtl {
/**
 * Most of the XHTML tags and attributes are available as methods.
 * A rule of thumb is that most tags are zero parameter methods and attribute
 * methods are single parameter.
 *
 * @author adamgent
 *
 * @param <T> See {@link MarkupBuilder} for how this should be parameterized.
 */
//=============================================================================
class Html : public GeneratedHtmlBuilder<Html> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	Html(std::ostream & writer) : GeneratedHtmlBuilder<Html>(writer) {
	}
	//-------------------------------------------------------------------------
	virtual Html & checkbox() {
		return checkbox(false);
	}
	//-------------------------------------------------------------------------
	virtual Html & checkbox(bool checked) {
		input();
		attr("type", "checkbox");
		if (checked)
			attr("checked", "checked");
		return getSelf();
	}
	//-------------------------------------------------------------------------
	virtual Html & getSelf() {
		return *this;
	}
	//-------------------------------------------------------------------------
	virtual Html & option(const std::string &  name,
			const std::string & _value, bool _selected)
	{
		GeneratedHtmlBuilder<Html>::option();
		value(_value);
		if (_selected)
			selected("selected");
		text(name);
		end();
		return getSelf();
	}
	//-------------------------------------------------------------------------
	virtual std::string escapeMarkup(const std::string & sSrc) const {
		// TODO: implement html escape
		return sSrc;
		/*
		ostringstream sRet;
		for( string::const_iterator iter = sSrc.begin(); iter!=sSrc.end(); iter++ )
		{
			unsigned char c = (unsigned char)*iter;
			switch( c ) {
			case '&': sRet << "&amp;"; break;
			case '<': sRet << "&lt;"; break;
			case '>': sRet << "&gt;"; break;
			case '"': sRet << "&quot;"; break;
			case '\'': sRet << "&apos;"; break;
			default:
				if ( c<32 || c>127 ) {
					sRet << "&#" << (unsigned int)c << ";";
				}
				else {
					sRet << c;
				}
			}
		}
		return sRet.str();*/
	}

};
}}}
#endif /* HTML_HPP_ */
