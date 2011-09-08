/*
 * SceneGraph.hpp
 *
 *  Created on: 19.10.2011
 *      Author: samba
 */

#ifndef SCENEGRAPH_HPP_
#define SCENEGRAPH_HPP_

#include "sambag/disco/IDrawContext.hpp"
#include "sambag/com/Common.hpp"
#include <boost/utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/unordered_map.hpp>

namespace sambag { namespace disco { namespace graphicElements {
class GraphicElement;
typedef boost::shared_ptr<GraphicElement> GraphicElementPtr;
//=============================================================================
class SceneGraph {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SceneGraph> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<SceneGraph> WPtr;
	//-------------------------------------------------------------------------
	typedef GraphicElementPtr SceneGraphElement;
	//-------------------------------------------------------------------------
	struct node_object_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	typedef boost::property<node_object_t, SceneGraphElement> vertexProperty;
	//-------------------------------------------------------------------------
	/**
	 * Graph type
	 */
	typedef boost::adjacency_list<
			boost::listS,
			boost::vecS,
			boost::bidirectionalS,
			vertexProperty > G;
	//-------------------------------------------------------------------------
	typedef boost::graph_traits<G>::vertex_descriptor Vertex;
	//-------------------------------------------------------------------------
	typedef boost::graph_traits<G>::edge_descriptor Edge;
	//-------------------------------------------------------------------------
	typedef boost::property_map<G, node_object_t>::type VertexElementMap;
	//-------------------------------------------------------------------------
	typedef boost::property_map<G, node_object_t>::const_type ConstVertexElementMap;
	//-------------------------------------------------------------------------
	typedef boost::graph_traits<G>::vertex_iterator VertexIterator;
	//-------------------------------------------------------------------------
	typedef boost::graph_traits<G>::edge_iterator EdgeIterator;
	//-------------------------------------------------------------------------
	typedef boost::graph_traits<G>::in_edge_iterator InEdgeIterator;
	//-------------------------------------------------------------------------
	typedef boost::graph_traits<G>::out_edge_iterator OutEdgeIterator;
	//-------------------------------------------------------------------------
	typedef boost::graph_traits<G>::adjacency_iterator AdjacencyIterator;
	//-------------------------------------------------------------------------
	typedef G::inv_adjacency_iterator InvAdjacencyIterator;
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<SceneGraphElement, Vertex> Element2Vertex;
	//-------------------------------------------------------------------------
private:
	//-------------------------------------------------------------------------
	WPtr self;
	//-------------------------------------------------------------------------
	G g;
	//-------------------------------------------------------------------------
	VertexElementMap vertexElementMap;
	//-------------------------------------------------------------------------
	ConstVertexElementMap constVertexElementMap;
	//-------------------------------------------------------------------------
	Element2Vertex element2Vertex;
	//-------------------------------------------------------------------------
	SceneGraph(){
		vertexElementMap = get( node_object_t(), g );
		//constVertexElementMap = get( node_object_t(), g);
	}
public:
	//-------------------------------------------------------------------------
	const SceneGraphElement & getSceneGraphElement( const Vertex &v ) const;
	//-------------------------------------------------------------------------
	void draw(IDrawContext::Ptr) const;
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return self.lock();
	}
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new SceneGraph());
		neu->self=neu;
		return neu;
	}
	//-------------------------------------------------------------------------
	/**
	 * return a "list" of all graph contained elements sorted topological.
	 * @param out container
	 */
	template<typename Container>
	void getElementsSorted(Container &out) const;
	//-------------------------------------------------------------------------
	bool addElement( SceneGraphElement ptr );
	//-------------------------------------------------------------------------
	bool connectElements(SceneGraphElement from, SceneGraphElement to);
};
//=============================================================================
/**
 *  @class DFSVisitor
 *  fills a container with @see SceneGraphElement topological sorted.
 */
//=============================================================================
using namespace boost;
template <typename SceneGraph, typename Container>
class DFSVisitor : public boost::dfs_visitor<> {
private:
public:
	//-------------------------------------------------------------------------
	Container &container;
	//-------------------------------------------------------------------------
	const SceneGraph &sceneGraph;
	//-------------------------------------------------------------------------
	DFSVisitor(const SceneGraph &sc, Container &container) :
		container(container), sceneGraph(sc) {}
	//-------------------------------------------------------------------------
	template <class Edge, class Graph>
	void back_edge(Edge, Graph&) const {}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void discover_vertex(const Vertex &v, Graph&) {
	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void finish_vertex(const Vertex &v, Graph&) {
		typename SceneGraph::SceneGraphElement obj =
				sceneGraph.getSceneGraphElement(v);
		if (obj)
			container.push_back(obj);
	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void initialize_vertex(const Vertex &v, Graph&) {
	}
};
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// getElementsSorted
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template<typename Container>
void SceneGraph::getElementsSorted (Container &out) const
{
	DFSVisitor<SceneGraph, Container> vis(*this, out);
	boost::depth_first_search(
		g,
		boost::visitor(vis)
	);
}

}}} // namespace


#endif /* SCENEGRAPH_HPP_ */
