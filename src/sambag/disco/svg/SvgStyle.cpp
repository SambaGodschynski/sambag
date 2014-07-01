/*
 * SvgStyle.cpp
 *
 *  Created on: Nov 14, 2011
 *      Author: samba
 */

#include "SvgStyle.hpp"
#include "StyleParser.hpp"
#include "graphicElements/SceneGraphHelper.hpp"
#include <list>
#include <string>
#include <boost/algorithm/string_regex.hpp>
#include <istream>
#include <iostream>
#include <sstream>
#include "AttributeParser.hpp"

namespace sambag { namespace disco { namespace svg {
//-----------------------------------------------------------------------------
typedef std::string Selectors;
//-----------------------------------------------------------------------------
typedef std::string Properites;
//-----------------------------------------------------------------------------
typedef std::pair<Selectors, Properites> Rule;
//-----------------------------------------------------------------------------
typedef std::list<Rule> Rules;
//-----------------------------------------------------------------------------
void parseStyleRules(const std::string &rules, Rules& outRules) {
	if (rules.length()==0) return;
	std::string inStr = rules;
	AttributeParser::prepareString(inStr, false);
	// replace out-comment
	std::ostringstream ss;
	std::ostream_iterator<char, char> oi(ss);
	boost::regex_replace(oi, inStr.begin(), inStr.end(), boost::regex("/\\*.*?\\*/"), "");
	// extract all matches
	std::string tmp(ss.str());
	std::string::const_iterator begin = tmp.begin();
	std::string::const_iterator end = tmp.end();
	boost::match_results<std::string::const_iterator> what;
	boost::regex re("(.+?)\\{(.*?)\\}");
	for ( ;
		regex_search(begin, end, what, re);
		begin = what[0].second
	) {
		std::string selectors = what[1];
		std::string properties = what[2];
		outRules.push_back(std::make_pair(selectors, properties));
	}
}
//-----------------------------------------------------------------------------
template<typename Container>
void parseSelectors(const std::string &selectors,
	graphicElements::SceneGraph::Ptr g,
	Container &outContainer)
{
	if (selectors.length()==0) return;
	std::string inStr = selectors;
	AttributeParser::prepareString(inStr, false);
	// extract all matches
	boost::regex re("\\s*,\\s*");
	boost::sregex_token_iterator i(inStr.begin(), inStr.end(), re, -1);
	boost::sregex_token_iterator j;
	for(; i!=j; ++i) {
		getGraphElementsBySelector(*i, g, outContainer);
	}
}
//-----------------------------------------------------------------------------
void processRulesList(const Rules& rules, graphicElements::SceneGraph::Ptr g) {
	boost_for_each(const Rule &r, rules) {
		// create style object
		std::stringstream is(r.second);
		graphicElements::Style style;
		is >> style;
		// find related objects
		std::list<IDrawable::Ptr> objects;
		parseSelectors(r.first, g, objects);
		boost_for_each(IDrawable::Ptr obj, objects) {
			graphicElements::SceneGraph::StylePtr olStyle;
			olStyle = g->getStyleRef(obj);
			if (olStyle) { // update style
				olStyle->add(style);
				continue;
			}
			g->setStyleTo(obj, style);
		}
	}
}

//=============================================================================
// class SvgStyle
//=============================================================================
//-----------------------------------------------------------------------------
void SvgStyle::init() {
	applyCssText(styleText, getRelatedSceneGraph());
}
//-----------------------------------------------------------------------------
void SvgStyle::applyCssText(const std::string &txt,
        graphicElements::SceneGraph::Ptr g)
{
    Rules rules;
	parseStyleRules(txt, rules);
	processRulesList(rules, g);
}
}}}

