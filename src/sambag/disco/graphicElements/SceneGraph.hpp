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
#include <boost/unordered_map.hpp>
#include <boost/graph/vector_as_graph.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/copy.hpp>

namespace sambag { namespace disco { namespace graphicElements {

//=============================================================================
/**
 * @class interface for ProcessListProcessors.
 */
class ProcessListProcessor {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ProcessListProcessor> Ptr;
	//-------------------------------------------------------------------------
	virtual void perform(IDrawContext::Ptr context) = 0;
};


//=============================================================================
/**
 * @class performs a iDrawObject->draw()
 */
class ProcessDrawable : public ProcessListProcessor {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ProcessDrawable> Ptr;
private:
	//-------------------------------------------------------------------------
	IDrawable::Ptr drawable;
	//-------------------------------------------------------------------------
	/**
	 * how many child nodes in scenegraph has the drawable object
	 */
	const size_t numChildren;
	//-------------------------------------------------------------------------
	ProcessDrawable(IDrawable::Ptr drawable, size_t numChildren ) :
		drawable(drawable), numChildren(numChildren){};
public:
	//-------------------------------------------------------------------------
	static Ptr create(IDrawable::Ptr drawable, size_t numChildren) {
		Ptr neu(new ProcessDrawable(drawable, numChildren));
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual void perform(IDrawContext::Ptr context);
};
//=============================================================================
/**
 * @class restores context state such as transformations
 */
class RestoreContextState : public ProcessListProcessor {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<RestoreContextState> Ptr;
private:
	//-------------------------------------------------------------------------
	RestoreContextState(){};
public:
	std::string name;
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new RestoreContextState());
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual void perform(IDrawContext::Ptr context);
};

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
	typedef IDrawable::Ptr SceneGraphElement;
	//-------------------------------------------------------------------------
	typedef size_t OrderNumber;
	//-------------------------------------------------------------------------
	enum VertexType {
		IDRAWABLE
	};
	//-------------------------------------------------------------------------
	struct node_object_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	struct node_vtype_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	// order number:
	struct node_order_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	typedef boost::property<node_object_t, SceneGraphElement,
			boost::property<node_vtype_t, VertexType,
			boost::property<node_order_t, OrderNumber
			> > > vertexProperties;
	//-------------------------------------------------------------------------
	/**
	 * Graph type
	 */
	typedef boost::adjacency_list<
			boost::listS,
			boost::vecS, 			// has to be vecS. Otherwise the processlist
									// creation is getting more complex.
			boost::bidirectionalS,
			vertexProperties > G;
	//-------------------------------------------------------------------------
	typedef boost::graph_traits<G>::vertex_descriptor Vertex;
	//-------------------------------------------------------------------------
	typedef boost::graph_traits<G>::edge_descriptor Edge;
	//-------------------------------------------------------------------------
	typedef boost::property_map<G, node_object_t>::type VertexElementMap;
	//-------------------------------------------------------------------------
	typedef boost::property_map<G, node_vtype_t>::type VertexTypeMap;
	//-------------------------------------------------------------------------
	typedef boost::property_map<G, node_order_t>::type VertexOrderMap;
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
	VertexTypeMap vertexTypeMap;
	//-------------------------------------------------------------------------
	Element2Vertex element2Vertex;
	//-------------------------------------------------------------------------
	VertexOrderMap vertexOrderMap;
	//-------------------------------------------------------------------------
	SceneGraph(){
		vertexElementMap = get( node_object_t(), g );
		vertexTypeMap = get( node_vtype_t(), g );
		vertexOrderMap  = get( node_order_t(), g );
	}
public:
	//-------------------------------------------------------------------------
	static const Vertex NULL_VERTEX;
	//-------------------------------------------------------------------------
	/**
	 * Assumes that graph contains vertex.
	 * @param vertex
	 * @return the order number
	 */
	VertexType getVertexType(const Vertex &v) const {
		return vertexTypeMap[v];
	}
	//-------------------------------------------------------------------------
	/**
	 * Assumes that graph contains vertex.
	* @param v vertex
	* @param ord order number
	* @return
	*/
	OrderNumber getVertexOrderNumber(const Vertex &v) const {
		return vertexOrderMap[v];
	}
	//-------------------------------------------------------------------------
	void setVertexOrderNumber(const Vertex &v, const OrderNumber &ord) {
		vertexOrderMap[v] = ord;
	}
	//-------------------------------------------------------------------------
	Vertex getRelatedVertex(const SceneGraphElement &el) const;
	//-------------------------------------------------------------------------
	/**
	 * Assumes that graph contains vertex.
	* @param v
	* @return
	*/
	const SceneGraphElement & getSceneGraphElement(const Vertex &v) const;
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
	void getProcessList(Container &out) const;
	//-------------------------------------------------------------------------
	/**
	 * fills Container with all parent vertices which matches to VertexType.
	 * @param v
	 * @param out
	 */
	template<typename Container>
	void findParentsWithType(const Vertex& v, VertexType type, Container &out) const;
	//-------------------------------------------------------------------------
	bool addElement( IDrawable::Ptr ptr );
	//-------------------------------------------------------------------------
	bool connectElements(IDrawable::Ptr from, IDrawable::Ptr to);
};
//=============================================================================
/**
 *  @class DFSVisitor
 *  fills a container with @see ProcessListProcessor objects, topological sorted.
 */
//=============================================================================
using namespace boost;
template <typename Container>
class DFSVisitor : public boost::dfs_visitor<> {
private:
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void finish_drawable(const Vertex &v, Graph& g) {
		typename SceneGraph::SceneGraphElement obj =
				sceneGraph.getSceneGraphElement(v);
		if (!obj)
			return;
		size_t numOutEdges = boost::out_degree(v, g);
		container.push_back(ProcessDrawable::create(obj, numOutEdges));

	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void discover_drawable(const Vertex &v, Graph &g) {
		// only need to restore if has child objects in graph
		if ( boost::out_degree(v, g) == 0 )
			return;

		typename SceneGraph::SceneGraphElement obj =
					sceneGraph.getSceneGraphElement(v);
		RestoreContextState::Ptr re = RestoreContextState::create();
		container.push_back(re);
	}
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
	void discover_vertex(const Vertex &v, Graph &g) {
		SceneGraph::VertexType type = sceneGraph.getVertexType(v);
			switch (type) {
				case SceneGraph::IDRAWABLE :
					discover_drawable(v, g); break;
			}
	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void finish_vertex(const Vertex &v, Graph &g) {
		SceneGraph::VertexType type = sceneGraph.getVertexType(v);
		switch (type) {
			case SceneGraph::IDRAWABLE :
				finish_drawable(v, g); break;
		}
	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void initialize_vertex(const Vertex &v, Graph&) {
	}
};
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
	@brief Copies grap src to graph dst. Assumes that dst is empty.
	Using this because the boost::copy_graph function dosen't works
	if G2 type is a "vector as graph" type. ( for all I know )
	Only works if G1 vertex_desc. type is vecS.
	Otherwise you have create a mapping from G1 to G2 vertex.

	@param src
	@param dst
*/
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template <typename G1, typename G2, typename Set, typename Comparator>
void copyGraph( const G1 &src, G2 &dst, const Comparator &cmp ) {
	typedef typename boost::graph_traits<G1>::vertex_iterator VertexIterator;
	typedef typename boost::graph_traits<G1>::edge_iterator EdgeIterator;

	using namespace boost;

	VertexIterator vi, vend;
	boost::tie(vi, vend) = vertices(src);
	for ( ; vi!=vend; ++vi ) {
		dst.push_back( Set(cmp) );

	}

	EdgeIterator ei, eend;
	boost::tie(ei, eend) = edges(src);
	for (; ei!=eend; ++ei) {
		add_edge(source(*ei,src), target(*ei,src), dst);
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
 * @class comparator for set.
 * Compares in order to the node_order_t property.
 */
template <class T> struct CompareNodeOrder {
	const SceneGraph *g; // reference dosen't work
	CompareNodeOrder( const SceneGraph *g = NULL ) : g(g) {}
	bool operator() (const T& x, const T& y) const {
		if (!g)
			return false;
		return g->getVertexOrderNumber(x)  >  g->getVertexOrderNumber(y);
	}
};
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template<typename Container>
void SceneGraph::getProcessList (Container &out) const
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
{
	/**
	 * @brief copies org. graph into a "vector as graph" graph.
	 * where the edge container is a set<vertex> to affect in which order
	 * the depth_first_search alg. selects the out_edges.
	 * see: http://www.4divisions.com/forx/wiki/doku.php?id=wiki:scenegraph
	 */
	if (boost::num_vertices(g)==0)
		return;
	DFSVisitor<Container> vis(*this, out);
	typedef CompareNodeOrder<Vertex> Comparator;
	typedef std::set<Vertex, Comparator > Set;
	typedef std::vector<Set> G2;
	G2 g2;
	Comparator cmp(this);
	copyGraph<G, G2, Set, Comparator>(g,g2,cmp); // O(n)

	boost::depth_first_search(
		g2,
		boost::visitor(vis)
	);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template<typename Container>
void SceneGraph::findParentsWithType(const Vertex& v, VertexType type, Container &out) const
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
{
	InvAdjacencyIterator it, end;
	boost::tie(it, end) = boost::inv_adjacent_vertices(v,g);
	for(; it!=end; ++it) {
		if (getVertexType(*it) == type)
			out.push_back(*it);
	}
}
}}} // namespace


#endif /* SCENEGRAPH_HPP_ */
