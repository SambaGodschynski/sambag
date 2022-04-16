/*
 * Text.cpp
 *
 *  Created on: 24.09.2011
 *      Author: samba
 */

#include "Text.hpp"
#include "sambag/com/Common.hpp"
#include <boost/algorithm/string.hpp>

namespace sambag { namespace disco { namespace svg { namespace graphicElements {

//=============================================================================
// class Text
//=============================================================================
//-----------------------------------------------------------------------------
Text::Text() {
}
//-----------------------------------------------------------------------------
Text::~Text() {
}
//-----------------------------------------------------------------------------
void Text::shape( IDrawContext::Ptr cn ) {
	cn->moveTo(pos.solve(cn));
    cn->textPath(text);
}
//-----------------------------------------------------------------------------
void Text::setText(const std::string &t)  {
    text = t;
    boost::algorithm::replace_all(text, "nbsp;", " ");
}
}}}}
