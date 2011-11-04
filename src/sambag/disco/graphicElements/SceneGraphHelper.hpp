/*
 * SceneGraphHelper.hpp
 *
 *  Created on: Nov 15, 2011
 *      Author: samba
 */

#ifndef SCENEGRAPHHELPER_HPP_
#define SCENEGRAPHHELPER_HPP_
#include "SceneGraph.hpp"
#include <string>
#include <boost/algorithm/string_regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <sambag/com/Exception.hpp>
#include "RefElement.hpp"

namespace {
using namespace sambag::disco::graphicElements;
//-----------------------------------------------------------------------------
template <typename Container>
void getGraphElements(const std::string &a,
	SceneGraph::Ptr g,
	Container &outContainer)
{
	if (a=="*") { // all
		SceneGraph::VertexIterator it, end;
		boost::tie(it, end) = boost::vertices(g->getGraphImpl());
		for(; it!=end; ++it) {
			if (g->getVertexType(*it) == SceneGraph::IDRAWABLE)
				outContainer.push_back(g->getSceneGraphElement(*it));
		}
		return;
	}
	if (a.at(0) == '#') { // id
		outContainer.push_back(g->getElementById(a));
		return;
	}
	if (a.at(0) == '.') { // class
		g->getElementsByClass(a, outContainer);
		return;
	}
	g->getElementsByTag(a, outContainer);
}
//-----------------------------------------------------------------------------
template <typename Container>
void getGraphElements(const std::string &a,
	const std::string &_op,
	const std::string &b,
	SceneGraph::Ptr g,
	Container &outContainer)
{
	if (b.length() == 0)
		return;
	typedef typename Container::value_type T;
	std::string op = boost::algorithm::trim_copy(_op);
	bool deepSearch = true;
	if (op==">") {
		deepSearch = false;
	}
	typename std::list<T> l;
	getGraphElements(a, g, l);
	boost_for_each(T obj, l) {
		char sType = b.at(0); // selector type
		if (sType=='*') { // all
			g->getChildren(obj, outContainer);
			continue;
		}
		if (sType == '#') { // id
			g->getChildrenById(obj, b, outContainer, deepSearch);
			continue;
		}
		if (sType == '.') { // class
			g->getChildrenByClass(obj, b, outContainer, deepSearch);
			continue;
		}
		g->getChildrenByTag(obj, b, outContainer, deepSearch);
	}
}
} // namespace

namespace sambag { namespace disco {
//=============================================================================
// SceneGraph Helper
//=============================================================================
//-----------------------------------------------------------------------------
/**
 * finds graph elemends by selector string such as in css,
 * supports only:
 * "A B", "A>B", "*"
 * @param selector
 * @param g
 * @param outContainer
 */
template<typename Container>
void getGraphElementsBySelector(const std::string &selector,
	SceneGraph::Ptr g,
	Container &outContainer)
{
	// extract all matches
	std::string::const_iterator begin = selector.begin();
	std::string::const_iterator end = selector.end();
	boost::match_results<std::string::const_iterator> what;
	boost::regex re("(\\S+)([ >+~]*)(\\S*)");
	for ( ;
		regex_search(begin, end, what, re);
		begin = what[0].second
	) {
		std::string a = what[1];
		std::string op = what[2];
		std::string b = what[3];
		if (b.length()>0)
			getGraphElements(a, op, b, g, outContainer);
		else
			getGraphElements(a, g, outContainer);
	}

}
//-----------------------------------------------------------------------------
/**
 * copies 'el' from 'src' to 'dst' including traits such as class, id, order ...
 * @param src
 * @param dst
 */
extern void copyElement(SceneGraph::Ptr src,
	SceneGraph::Ptr dst,
	SceneGraph::SceneGraphElement el);
//-----------------------------------------------------------------------------
/**
 * copies 'srcEl`s' traits from 'src' SceneGraph to 'dstEl' SceneGraph 'dst'
 * @param src
 * @param dst
 * @param srcEl
 * @param dstEl
 */
extern void copyElementTraits(SceneGraph::Ptr src,
	SceneGraph::Ptr dst,
	SceneGraph::SceneGraphElement srcEl,
	SceneGraph::SceneGraphElement dstEl
);
//-----------------------------------------------------------------------------
/**
 * copies subgraph 'src' starting with vertex 'start' into 'dst'.
 * Assumes that start!=dst
 * @param scr
 * @param dst
 * @param start
 * @param newElements
 */
extern void copySubGraph(SceneGraph::Ptr src,
	SceneGraph::Ptr dst,
	const SceneGraph::Vertex &start);
//-----------------------------------------------------------------------------
/**
 * cpies 'src' to 'dst' using RefElement as new SceneGraphElement
 * which refers src's elements. Assumes that start!=dst
 * @param src
 * @param dst
 * @param outMap
 */
template <typename Old2NewMap>
extern void addToGraphAsReference( SceneGraph::Ptr src,
	SceneGraph::Ptr dst,
	Old2NewMap &outMap)
{
	SAMBA_ASSERT(src!=dst);
	SAMBA_ASSERT(src && dst);
	typedef SceneGraph::SceneGraphElement Element;
	typedef SceneGraph::Vertex Vertex;
	// copy vertices
	SceneGraph::VertexIterator vIt, vEnd;
	boost::tie(vIt, vEnd) = boost::vertices(src->getGraphImpl());
	for(; vIt!=vEnd; ++vIt) {
		Element old = src->getSceneGraphElement(*vIt);
		if (!old)
			continue;
		RefElement::Ptr neu = RefElement::create();
		neu->setReference(old);
		dst->addElement(neu);
		outMap[old] = neu;
		copyElementTraits(src, dst, old, neu);
	}
	// copy edges
	SceneGraph::EdgeIterator eIt, eEnd;
	boost::tie(eIt, eEnd) = boost::edges(src->getGraphImpl());
	for(; eIt!=eEnd; ++eIt) {
		Vertex sv = boost::source(*eIt, src->getGraphImpl());
		Vertex tv = boost::target(*eIt, src->getGraphImpl());
		Element sse = src->getSceneGraphElement(sv);
		Element ste = src->getSceneGraphElement(tv);
		if (!sse || !ste)
			continue;
		dst->connectElements(
			outMap[sse],
			outMap[ste]
		);
	}
}

}} // namespace

#endif /* SCENEGRAPHHELPER_HPP_ */
