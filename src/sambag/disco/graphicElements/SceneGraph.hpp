/*
 * SceneGraph.hpp
 *
 *  Created on: 19.10.2011
 *      Author: samba
 */

#ifndef SCENEGRAPH_HPP_
#define SCENEGRAPH_HPP_

#include "sambag/disco/IDrawContext.hpp"
#include "sambag/disco/IDrawable.hpp"
#include "sambag/disco/graphicElements/Style.hpp"
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
class IProcessListObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<IProcessListObject> Ptr;
	//-------------------------------------------------------------------------
	virtual void perform(IDrawContext::Ptr context) = 0;
	//-------------------------------------------------------------------------
	/**
	 * for debugging and testing
	 * @return
	 */
	virtual std::string toString() const = 0;
};
//=============================================================================
/**
 * @class restores context state such as transformations
 */
class DoNothing : public IProcessListObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<DoNothing> Ptr;
private:
	//-------------------------------------------------------------------------
	DoNothing(){};
public:
	//-------------------------------------------------------------------------
	virtual std::string toString() const {
		return "DoNothing";
	}
	std::string name;
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr neu(new DoNothing());
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual void perform(IDrawContext::Ptr context) {}
};
//=============================================================================
/**
 * @class performs a iDrawObject->draw()
 */
class ProcessDrawable : public IProcessListObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ProcessDrawable> Ptr;
private:
	//-------------------------------------------------------------------------
	IDrawable::Ptr drawable;
	//-------------------------------------------------------------------------
	// reset context state after draw. otherwise a RestoreContextState
	// will do it later.
	const bool resetContextState;
	//-------------------------------------------------------------------------
	Style style;
	//-------------------------------------------------------------------------
	Matrix transformation;
	//-------------------------------------------------------------------------
	ProcessDrawable(IDrawable::Ptr drawable,
					bool resetContextState,
					const graphicElements::Style &style,
					const Matrix &transformation ) :
		drawable(drawable),
		resetContextState(resetContextState),
		style(style),
		transformation(transformation){}
public:
	//-------------------------------------------------------------------------
	virtual std::string toString() const {
		return (resetContextState ? "DrawAndRestoreContextState(" : "Draw(")
				+ drawable->toString() + ")";
	}
	//-------------------------------------------------------------------------
	static Ptr create(
			IDrawable::Ptr drawable,
			bool resetContextState,
			const graphicElements::Style &style,
			const Matrix &transformation )
	{
		Ptr neu( new ProcessDrawable(drawable,
									 resetContextState,
									 style,
									 transformation));
		return neu;
	}
	//-------------------------------------------------------------------------
	virtual void perform(IDrawContext::Ptr context);
};
//=============================================================================
/**
 * @class restores context state such as transformations
 */
class RestoreContextState : public IProcessListObject {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<RestoreContextState> Ptr;
private:
	//-------------------------------------------------------------------------
	RestoreContextState(){};
public:
	//-------------------------------------------------------------------------
	virtual std::string toString() const {
		return "RestoreContextState";
	}
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
/**
 * @class SceneGraph.
 * see http://www.4divisions.com/forx/wiki/doku.php?id=wiki:scenegraph
 */
class SceneGraph {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::vecS EdgeContainerType;
	//-------------------------------------------------------------------------
	typedef boost::vecS VertexContainerType;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<SceneGraph> Ptr;
	//-------------------------------------------------------------------------
	typedef boost::weak_ptr<SceneGraph> WPtr;
	//-------------------------------------------------------------------------
	// TODO: cleanup SceneGraphElement / IDrawable confusuion
	typedef IDrawable::Ptr SceneGraphElement;
	//-------------------------------------------------------------------------
	typedef graphicElements::Style StyleNode;
	//-------------------------------------------------------------------------
	typedef Matrix TransformationNode;
	//-------------------------------------------------------------------------
	typedef int OrderNumber;
	//-------------------------------------------------------------------------
	static const int NO_ORDER_NUMBER = -1;
	//-------------------------------------------------------------------------
	enum VertexType {
		IDRAWABLE,
		TRANSFORM,
		STYLE,
		CLASS
	};
	//-------------------------------------------------------------------------
	struct node_object_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	struct node_style_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	struct node_transformation_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	struct node_vtype_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	struct node_vname_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	// order number:
	struct node_order_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	typedef boost::property<node_object_t, SceneGraphElement,
			boost::property<node_style_t, StyleNode,
			boost::property<node_transformation_t, TransformationNode,
			boost::property<node_vtype_t, VertexType,
			boost::property<node_order_t, OrderNumber,
			boost::property<node_vname_t, std::string
			> > > > > >vertexProperties;
	//-------------------------------------------------------------------------
	/**
	 * Graph type
	 */
	typedef boost::adjacency_list<
			EdgeContainerType,
			VertexContainerType, 			// has to be vecS. Otherwise the processlist
									// creation and other things getting more complex.
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
	typedef boost::property_map<G, node_vname_t>::type VertexNameMap;
	//-------------------------------------------------------------------------
	typedef boost::property_map<G, node_order_t>::type VertexOrderMap;
	//-------------------------------------------------------------------------
	typedef boost::property_map<G, node_style_t>::type VertexStyleMap;
	//-------------------------------------------------------------------------
	typedef boost::property_map<G, node_transformation_t>::type VertexTransformationMap;
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
	typedef std::string Id;
	//-------------------------------------------------------------------------
	typedef std::string Class;
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<Id, Vertex> Id2Vertex;
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<Class, Vertex> Class2Vertex;
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
	VertexNameMap vertexNameMap;
	//-------------------------------------------------------------------------
	Element2Vertex element2Vertex;
	//-------------------------------------------------------------------------
	VertexOrderMap vertexOrderMap;
	//-------------------------------------------------------------------------
	VertexTransformationMap vertexTransformationMap;
	//-------------------------------------------------------------------------
	VertexStyleMap vertexStyleMap;
	//-------------------------------------------------------------------------
	Id2Vertex id2Vertex;
	//-------------------------------------------------------------------------
	Class2Vertex class2Vertex;
	//-------------------------------------------------------------------------
	SceneGraph(){
		vertexElementMap = get( node_object_t(), g );
		vertexTypeMap = get( node_vtype_t(), g );
		vertexOrderMap  = get( node_order_t(), g );
		vertexTransformationMap = get( node_transformation_t(), g );
		vertexStyleMap = get( node_style_t(), g );
		vertexNameMap = get( node_vname_t(), g );
	}
public:
	//-------------------------------------------------------------------------
	/**
	 * @return the graph implementation. ( the bgl object )
	 */
	const G & getGraphImpl() const { return g; }
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
	/**
	 * @param el
	 * @return NULL_VERTEX if not found
	 */
	Vertex getRelatedVertex(const SceneGraphElement &el) const;
	//-------------------------------------------------------------------------
	/**
	 * Assumes that graph contains vertex.
	* @param v
	* @return
	*/
	const SceneGraphElement & getSceneGraphElement(const Vertex &v) const;
	//-------------------------------------------------------------------------
	void createProcessListAndDraw(IDrawContext::Ptr) const;
	//-------------------------------------------------------------------------
	/**
	 * creates a transformation node and relates it to el
	 * @param el a SceneGraphElement of this graph object
	 * @param m a transformation matrix
	 */
	bool setTransfomationTo(const SceneGraphElement &el, const Matrix &m);
	//-------------------------------------------------------------------------
	/**
	* creates a style node and relates it to el
	* @param el a SceneGraphElement of this graph object
	* @param s a @see Style object
	*/
	bool setStyleTo(const SceneGraphElement &el, const graphicElements::Style &s);
	//-------------------------------------------------------------------------
	/**
	 * @param el
	 * @param className
	 * @return true when element registration was successfull
	 */
	bool registerElementClass(const SceneGraphElement &el, const Class &className );
	//-------------------------------------------------------------------------
	const Matrix & getTransformationOf(const SceneGraphElement &el) const;
	//-------------------------------------------------------------------------
	const graphicElements::Style & getStyleOf(const SceneGraphElement &el) const;
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
	 * for testing and debugging
	 * @return
	 */
	std::string processListAsString() const;
	//-------------------------------------------------------------------------
	/**
	 * fills Container with all parent vertices which matches to VertexType.
	 * @param v
	 * @param out
	 */
	template<typename Container>
	void findParentsWithType(const Vertex& v, VertexType type, Container &out) const;
	//-------------------------------------------------------------------------
	size_t inDegreeOf(const Vertex& v, VertexType type) const;
	//-------------------------------------------------------------------------
	bool addElement( IDrawable::Ptr ptr );
	//-------------------------------------------------------------------------
	bool connectElements(IDrawable::Ptr from, IDrawable::Ptr to);
	//-------------------------------------------------------------------------
	bool registerElementId(const SceneGraphElement & el, const Id &id) {
		Vertex v = getRelatedVertex(el);
		if (v==NULL_VERTEX)
			return false;
		return (id2Vertex.insert( std::make_pair(id, v) )).second;
	}
	//-------------------------------------------------------------------------
	IDrawable::Ptr getElementById(const Id &id) const;
	//-------------------------------------------------------------------------
	template <typename Container>
	void getElementsByClass(const Class & className, Container &c);
};
//=============================================================================
/**
 *  @class DFSVisitor
 *  fills a container with @see ProcessListProcessor objects, topological sorted.
 *  http://www.4divisions.com/forx/wiki/doku.php?id=wiki:scenegraph&#building_process_list
 */
//=============================================================================
using namespace boost;
template <typename Container>
class DFSVisitor : public boost::dfs_visitor<> {
private:
	//-------------------------------------------------------------------------
	Container &container;
	//-------------------------------------------------------------------------
	const SceneGraph &sceneGraph;
	//-------------------------------------------------------------------------
	typedef SceneGraph::Vertex Vertex;
	//-------------------------------------------------------------------------
	typedef typename Container::const_iterator Iterator;
	//-------------------------------------------------------------------------
	typedef std::list< std::pair<Vertex, Iterator> > RecycleMapBuilderStack;
	//-------------------------------------------------------------------------
	/**
	 * maps from vertex to a range in subProcessingList
	 * http://www.4divisions.com/forx/wiki/doku.php?id=wiki:scenegraph&#svg_s_use_problem
	 */
	typedef std::map<Vertex, std::pair<Iterator, Iterator> > RecycleMap;
	//-------------------------------------------------------------------------
	RecycleMapBuilderStack stack;
	//-------------------------------------------------------------------------
	RecycleMap recycleMap;
	//-------------------------------------------------------------------------
	Vertex startVertex;
	//-------------------------------------------------------------------------
	bool startVertexFinished;
	//-------------------------------------------------------------------------
	void updateRecycleMap(const Vertex &v) {
		// insert pre-processed entries if available
		if (stack.back().first == v) { // insert into recMap
			Iterator startIt = stack.back().second;
			Iterator endIt = --(container.end()); // current process list position
			stack.pop_back();
			std::list<Vertex> vertices;
			sceneGraph.findParentsWithType(v, SceneGraph::IDRAWABLE, vertices);
			for_each(const Vertex &iv, vertices) {
				recycleMap.insert(
					std::make_pair(iv, std::make_pair(startIt, endIt))
				);
			}
		}
	}
	//-------------------------------------------------------------------------
	void resolveFromRecycleMap(const Vertex &v) {
		// insert pre-processed entries if available
		typename RecycleMap::const_iterator mapIt = recycleMap.find(v);
		if (mapIt==recycleMap.end()) return;
		Iterator it = mapIt->second.first;
		Iterator endIt = mapIt->second.second;
		if (it==container.end()) { // reference to leaf => no startIt
			container.push_back(*endIt);
			return;
		}
		it++;
		for (; it!=endIt; ++it) {
			container.push_back(*it);
		}
}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void finish_drawable(const Vertex &v, Graph& g) {
		typename SceneGraph::SceneGraphElement obj =
				sceneGraph.getSceneGraphElement(v);
		if (!obj)
			return;
		size_t numOutEdges = boost::out_degree(v, g);
		com::Matrix tM = sceneGraph.getTransformationOf(obj);
		if (tM==com::NULL_MATRIX)
			tM = com::IDENTITY_MATRIX;
		ProcessDrawable::Ptr cmd = ProcessDrawable::create(obj,
				numOutEdges==0,
				sceneGraph.getStyleOf(obj),
				tM);
		container.push_back(cmd);
		updateRecycleMap(v);
	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void discover_drawable(const Vertex &v, Graph &g) {
		typename SceneGraph::SceneGraphElement obj =
				sceneGraph.getSceneGraphElement(v);
		bool isLeaf = false;
		// only need to restore if has child objects in graph
		if ( boost::out_degree(v, g) > 0 ) {
			RestoreContextState::Ptr re = RestoreContextState::create();
			container.push_back(re);
		} else {
			isLeaf = true;
		}
		if (sceneGraph.inDegreeOf(v, SceneGraph::IDRAWABLE) > 1) {
			if (!container.empty()) {
				if (isLeaf)
					stack.push_back(std::make_pair(v, container.end() ));
				else
					stack.push_back(std::make_pair(v, --(container.end()) ));
			}
		}
		resolveFromRecycleMap(v);
	}
public:
	//-------------------------------------------------------------------------
	DFSVisitor(const SceneGraph &sc, Container &container, Vertex startVertex) :
		container(container),
		sceneGraph(sc),
		startVertex(startVertex),
		startVertexFinished(false)
	{
	}
	//-------------------------------------------------------------------------
	template <class Edge, class Graph>
	void back_edge(Edge, Graph&) const {}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void discover_vertex(const Vertex &v, Graph &g) {
		if (startVertexFinished)
			return;
		SceneGraph::VertexType type = sceneGraph.getVertexType(v);
			switch (type) {
				case SceneGraph::IDRAWABLE :
					discover_drawable(v, g); break;
				case SceneGraph::TRANSFORM :
				case SceneGraph::STYLE :
				case SceneGraph::CLASS :
					break;
			}
	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void finish_vertex(const Vertex &v, Graph &g) {
		if (startVertexFinished)
			return;
		if (v==startVertex) {
			startVertexFinished = true;
		}
		SceneGraph::VertexType type = sceneGraph.getVertexType(v);
		switch (type) {
			case SceneGraph::IDRAWABLE :
				finish_drawable(v, g); break;
			case SceneGraph::TRANSFORM :
			case SceneGraph::STYLE :
			case SceneGraph::CLASS :
				break;
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
	Only works if G1 vertex_descr. type is vecS.
	Otherwise there is missing a mapping from G1 to G2 vertex.

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
 * Compares node_order_t property descending.
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
	 * see: http://www.4divisions.com/forx/wiki/doku.php?id=wiki:scenegraph#building_process_list
	 */
	if (boost::num_vertices(g)==0)
		return;
	Vertex startVertex =  *(boost::vertices(g).first);
	DFSVisitor<Container> vis(*this, out, startVertex);
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
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template <typename Container>
void SceneGraph::getElementsByClass(const Class & className, Container &c) {
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Class2Vertex::iterator clVIt = class2Vertex.find(className);
	if (clVIt==class2Vertex.end())
		return;
	AdjacencyIterator it, end;
	boost::tie(it, end) = boost::adjacent_vertices(clVIt->second, g);
	for (; it!=end; ++it) {
		c.push_back(getSceneGraphElement(*it));
	}
}
}}} // namespace


#endif /* SCENEGRAPH_HPP_ */
