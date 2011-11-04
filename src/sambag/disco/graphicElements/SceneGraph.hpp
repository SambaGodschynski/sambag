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
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/copy.hpp>
#include <boost/algorithm/string.hpp>

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
	typedef graphicElements::Style Style;
	//-------------------------------------------------------------------------
	typedef Matrix Transformation;
	//-------------------------------------------------------------------------
	typedef int OrderNumber;
	//-------------------------------------------------------------------------
	typedef std::string Id;
	//-------------------------------------------------------------------------
	typedef std::string Class;
	//-------------------------------------------------------------------------
	typedef std::string Tag;
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
	/**
	 * IDRAWABLE: id-name, CLASS: class name
	 */
	struct node_vname_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	/**
	 * IDRAWABLE: (svg)tag-name
	 */
	struct node_vname2_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	// order number:
	struct node_order_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	typedef boost::property<node_object_t, SceneGraphElement,
			boost::property<node_style_t, Style,
			boost::property<node_transformation_t, Transformation,
			boost::property<node_vtype_t, VertexType,
			boost::property<node_order_t, OrderNumber,
			boost::property<node_vname_t, std::string,
			boost::property<node_vname2_t, std::string
			> > > > > > > vertexProperties;
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
	typedef boost::property_map<G, node_vname2_t>::type VertexName2Map;
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
	typedef boost::unordered_map<Id, Vertex> Id2Vertex;
	//-------------------------------------------------------------------------
	typedef boost::unordered_map<Class, Vertex> Class2Vertex;
	//-------------------------------------------------------------------------
	typedef std::list<IProcessListObject::Ptr> ProcessList;
private:
	//-------------------------------------------------------------------------
	ProcessList processList;
	//-------------------------------------------------------------------------
	template <typename Container, typename Filter>
	class BFSVisitor : public boost::bfs_visitor<> {
	private:
		//---------------------------------------------------------------------
		const Filter &filter;
		//---------------------------------------------------------------------
		const SceneGraph &sg;
		//---------------------------------------------------------------------
		Container &container;
		//---------------------------------------------------------------------
		bool firstDiscover;
	public:
		//---------------------------------------------------------------------
		BFSVisitor(const SceneGraph &sg, Container &container, const Filter &filter)
		: filter(filter), sg(sg), container(container), firstDiscover(true) {}
		//---------------------------------------------------------------------
		template <class Vertex, class Graph>
		void discover_vertex(const Vertex &u, Graph &g) {
			if (firstDiscover) { // ignore start vertex
				firstDiscover = false;
				return;
			}
			SceneGraph::SceneGraphElement el = sg.getSceneGraphElement(u);
			if (filter.filter(el))
				container.push_back(el);
		}
	};
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
	VertexName2Map vertexName2Map;
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
		vertexName2Map = get( node_vname2_t(), g );
	}
public:
	//-------------------------------------------------------------------------
	/**
	 * forces process list recalculation
	 */
	void update() {
		processList.clear();
	}
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
	void draw(IDrawContext::Ptr);
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
	 * creates process list if list is empty and graph::num_vertices > 0.
	 * @return process list
	 */
	const ProcessList & getProcessList() {
		if ( boost::num_vertices(g) > 0 && processList.empty() )
			createProcessList(processList);
		return processList;
	}
	//-------------------------------------------------------------------------
	/**
	 * return a "list" of all graph contained elements sorted topological.
	 * @param out container
	 */
	template<typename Container>
	void createProcessList(Container &out) const;
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
	void findParentsByType(const Vertex& v, VertexType type, Container &out) const;
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
		vertexNameMap[v] = id;
		return (id2Vertex.insert( std::make_pair(id, v) )).second;

	}
	//-------------------------------------------------------------------------
	void setTagName(const SceneGraphElement & el, const Tag &tagName) {
		Vertex v = getRelatedVertex(el);
		if (v==NULL_VERTEX)
			return;
		vertexName2Map[v] = tagName;
	}
	//-------------------------------------------------------------------------
	struct AllElements {
		bool filter(const SceneGraphElement & el) const { return true; }
	};
	//-------------------------------------------------------------------------
	struct TagFilter {
		Tag tag;
		const SceneGraph &g;
		TagFilter(const Tag &tag, const SceneGraph &g) : tag(tag), g(g) {}
		bool filter(const SceneGraphElement & el) const {
			using namespace boost::algorithm;
			if (to_lower_copy(tag) == to_lower_copy(g.getTagName(el)))
				return true;
			return false;
		}
	};
	//-------------------------------------------------------------------------
	struct ClassFilter {
		Class _class;
		const SceneGraph &g;
		ClassFilter(const Class &_class, const SceneGraph &g) :
			_class(_class), g(g) {}
		bool filter(const SceneGraphElement & el) const {
			using namespace boost::algorithm;
			std::list<Class> classes;
			g.getClassNames(el, classes);
			for_each(const Class &cn, classes) {
				if (to_lower_copy(_class) == to_lower_copy(cn))
					return true;
			}
			return false;
		}
	};
	//-------------------------------------------------------------------------
	struct IdFilter {
		Id id;
		const SceneGraph &g;
		IdFilter(const Id &id, const SceneGraph &g) :
			id(id), g(g) {}
		bool filter(const SceneGraphElement & el) const {
			using namespace boost::algorithm;
			if (to_lower_copy(id) == to_lower_copy(g.getIdName(el)))
				return true;
			return false;
		}
	};
	//-------------------------------------------------------------------------
	template <typename Container, typename Filter>
	void getChildren(const SceneGraphElement & el,
			Container &c,
			const Filter &filter)
	{
		Vertex v = getRelatedVertex(el);
		if (v==NULL_VERTEX)
			return;
		BFSVisitor<Container, Filter> bfsVis(*this, c, filter);
		boost::breadth_first_search(g, v, boost::visitor(bfsVis));
	}
	//-------------------------------------------------------------------------
	template <typename Container>
	void getChildren(const SceneGraphElement & el,
			Container &c)
	{
		AllElements filter;
		getChildren(el, c, filter);
	}
	//-------------------------------------------------------------------------
	template <typename Container>
	void getChildrenByTag(const SceneGraphElement & el,
			const Tag &tagName,
			Container &c)
	{
		TagFilter filter(tagName, *this);
		getChildren(el, c, filter);
	}
	//-------------------------------------------------------------------------
	template <typename Container>
	void getChildrenByClass(const SceneGraphElement & el,
			const Class &className,
			Container &c)
	{
		ClassFilter filter(className, *this);
		getChildren(el, c, filter);
	}
	//-------------------------------------------------------------------------
	template <typename Container>
	void getChildrenById(const SceneGraphElement & el,
			const Id &id,
			Container &c)
	{
		IdFilter filter(id, *this);
		getChildren(el, c, filter);
	}
	//-------------------------------------------------------------------------
	Tag getTagName(const SceneGraphElement & el) const {
		Vertex v = getRelatedVertex(el);
		if (v == NULL_VERTEX)
			return "";
		return vertexName2Map[v];
	}
	//-------------------------------------------------------------------------
	template <typename Container>
	void getClassNames(const SceneGraphElement & el, Container &c) const {
		Vertex v = getRelatedVertex(el);
		if (v == NULL_VERTEX)
			return;
		std::list<Vertex> l;
		findParentsByType(v, CLASS, l);
		for_each(const Vertex &it, l) {
			c.push_back(vertexNameMap[it]);
		}
	}
	//-------------------------------------------------------------------------
	Id getIdName(const SceneGraphElement & el) const {
		Vertex v = getRelatedVertex(el);
		if (v == NULL_VERTEX)
			return "";
		return vertexNameMap[v];
	}
	//-------------------------------------------------------------------------
	IDrawable::Ptr getElementById(const Id &id) const;
	//-------------------------------------------------------------------------
	template <typename Container>
	void getElementsByClass(const Class & className, Container &c);
	//-------------------------------------------------------------------------
	void adoptStyleEdges(const SceneGraphElement &src,
			const SceneGraphElement &dst)
	{
		Vertex srcV = getRelatedVertex(src);
		Vertex dstV = getRelatedVertex(dst);
		if (srcV==NULL_VERTEX || dstV==NULL_VERTEX )
			return;
		std::list<Vertex> vertices;
		findParentsByType(srcV, SceneGraph::STYLE, vertices);
		for_each(Vertex v, vertices) {
			boost::add_edge(v, dstV, g);
		}
	}
	//-------------------------------------------------------------------------
	void adoptTransformEdges(const SceneGraphElement &src,
			const SceneGraphElement &dst)
	{
		Vertex srcV = getRelatedVertex(src);
		Vertex dstV = getRelatedVertex(dst);
		if (srcV==NULL_VERTEX || dstV==NULL_VERTEX )
			return;
		std::list<Vertex> vertices;
		findParentsByType(srcV, SceneGraph::TRANSFORM, vertices);
		for_each(const Vertex &v, vertices) {
			boost::add_edge(v, dstV, g);
		}
	}
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
	Vertex startVertex;
	//-------------------------------------------------------------------------
	bool startVertexFinished;
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
	}
	//-------------------------------------------------------------------------
	template <class Vertex, class Graph>
	void discover_drawable(const Vertex &v, Graph &g) {
		typename SceneGraph::SceneGraphElement obj =
				sceneGraph.getSceneGraphElement(v);
		// only need to restore if has child objects in graph
		if ( boost::out_degree(v, g) > 0 ) {
			RestoreContextState::Ptr re = RestoreContextState::create();
			container.push_back(re);
		}
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
void SceneGraph::createProcessList (Container &out) const
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
void SceneGraph::findParentsByType(const Vertex& v, VertexType type, Container &out) const
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
