/*
 * SceneGraph.hpp
 *
 *  Created on: 19.10.2011
 *      Author: samba
 */

#ifndef SCENEGRAPH_HPP_
#define SCENEGRAPH_HPP_

#include <boost/graph/vector_as_graph.hpp>
#include "sambag/disco/IDrawContext.hpp"
#include "sambag/disco/IDrawable.hpp"
#include "Style.hpp"
#include "sambag/com/Common.hpp"
#include <boost/utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/unordered_map.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/copy.hpp>
#include <boost/algorithm/string.hpp>

namespace sambag { namespace disco { namespace svg { namespace graphicElements {
//=============================================================================
/**
 * @class interface for ProcessListProcessors.
 */
struct IProcessListObject {
//=============================================================================
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
    //-------------------------------------------------------------------------
    virtual ~IProcessListObject() {}
};
//=============================================================================
/**
 * @class restores context state such as transformations
 */
struct DoNothing : public IProcessListObject {
//=============================================================================
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<DoNothing> Ptr;
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
    //-------------------------------------------------------------------------
    virtual ~DoNothing(){}
};
//=============================================================================
/**
 * @class performs a iDrawObject->draw()
 */
struct ProcessDrawable : public IProcessListObject {
//=============================================================================
	//-------------------------------------------------------------------------
	virtual ~ProcessDrawable() {}
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<ProcessDrawable> Ptr;
    //-------------------------------------------------------------------------
    IDrawable::Ptr drawable;
    //-------------------------------------------------------------------------
    // reset context state after draw. otherwise a RestoreContextState
    // will do it later.
    const bool resetContextState;
    //-------------------------------------------------------------------------
    boost::shared_ptr<Style> style;
    //-------------------------------------------------------------------------
    IPattern::Ptr fill, stroke;
    //-------------------------------------------------------------------------
    boost::shared_ptr<Matrix> transformation;
    //-------------------------------------------------------------------------
    ProcessDrawable(IDrawable::Ptr drawable,
		    bool resetContextState,
		    boost::shared_ptr<Style> style,
		    boost::shared_ptr<Matrix> transformation ) :
	drawable(drawable),
	resetContextState(resetContextState),
	style(style),
	transformation(transformation){}
    //-------------------------------------------------------------------------
    IDrawable::Ptr getDrawable() const {
        return drawable;
    }
    //-------------------------------------------------------------------------
    virtual std::string toString() const {
	return (resetContextState ? "DrawAndRestoreContextState(" : "Draw(")
	    + drawable->toString() + ")";
    }
    //-------------------------------------------------------------------------
    static Ptr create(
	IDrawable::Ptr drawable,
	bool resetContextState,
	boost::shared_ptr<Style> style,
	boost::shared_ptr<Matrix> transformation )
	{
	    Ptr neu( new ProcessDrawable(drawable,
					 resetContextState,
					 style,
					 transformation));
	    return neu;
	}
    //-------------------------------------------------------------------------
    virtual void perform(IDrawContext::Ptr context);
    //-------------------------------------------------------------------------
    virtual Rectangle getBounds(IDrawContext::Ptr context) const;
};
//=============================================================================
/**
 * @class restores context state such as transformations
 */
struct RestoreContextState : public IProcessListObject {
//=============================================================================
	//-------------------------------------------------------------------------
	virtual ~RestoreContextState() {}
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<RestoreContextState> Ptr;
    //-------------------------------------------------------------------------
    RestoreContextState(){};
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
    typedef boost::shared_ptr<graphicElements::Style> StylePtr;
    //-------------------------------------------------------------------------
    typedef boost::shared_ptr<Matrix> MatrixPtr;
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
       boost::property<node_style_t, StylePtr,
       boost::property<node_transformation_t, MatrixPtr,
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
    typedef boost::unordered_map<SceneGraphElement, Rectangle> Element2Bounds;
    //-------------------------------------------------------------------------
    typedef boost::unordered_map<Id, Vertex> Id2Vertex;
    //-------------------------------------------------------------------------
    typedef boost::unordered_map<Class, Vertex> Class2Vertex;
    //-------------------------------------------------------------------------
    typedef boost::unordered_multimap<Tag, Vertex> Tag2Vertex;
    //-------------------------------------------------------------------------
    typedef std::list<IProcessListObject::Ptr> ProcessList;
private:
    //-------------------------------------------------------------------------
    void computeBoundingBoxes(IDrawContext::Ptr);
    Rectangle computeBoundingBox(IDrawable::Ptr parent);
    //-------------------------------------------------------------------------
    ProcessList processList;
    //-------------------------------------------------------------------------
    template <typename _Container, typename Filter>
    class BFSVisitor : public boost::bfs_visitor<> {
    private:
	//---------------------------------------------------------------------
	const Filter &filter;
	//---------------------------------------------------------------------
	const SceneGraph &sg;
	//---------------------------------------------------------------------
	_Container &container;
	//---------------------------------------------------------------------
	bool firstDiscover;
    public:
	//---------------------------------------------------------------------
	BFSVisitor(const SceneGraph &sg, _Container &container, const Filter &filter)
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
    Element2Bounds element2Bounds;
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
    Tag2Vertex tag2Vertex;
    //-------------------------------------------------------------------------
    void initMaps() {
	vertexElementMap = get( node_object_t(), g );
	vertexTypeMap = get( node_vtype_t(), g );
	vertexOrderMap  = get( node_order_t(), g );
	vertexTransformationMap = get( node_transformation_t(), g );
	vertexStyleMap = get( node_style_t(), g );
	vertexNameMap = get( node_vname_t(), g );
	vertexName2Map = get( node_vname2_t(), g );
    }
    //-------------------------------------------------------------------------
    SceneGraph(){
	initMaps();
    }
public:
    //-------------------------------------------------------------------------
    /**
     * @brief forces process list recalculation
     * @deprecated use invalidate() instead
     */
    void update() {
	processList.clear();
	element2Bounds.clear();
    }
    //-------------------------------------------------------------------------
    /**
     * @brief invalidates bounds and scene
     */
    void invalidate() {
	processList.clear();
	element2Bounds.clear();
    }
    //-------------------------------------------------------------------------
    /**
     * @brief invalidates bounds only
     */
    void invalidateBounds() {
	element2Bounds.clear();
    }
    //-------------------------------------------------------------------------
    /**
     * @brief validates bounds and scene
     * @param size is used to solve relative svg coordinates
     */
    void validate(const Dimension &size) {
        getProcessList();
        validateBounds(size);
    }
    //-------------------------------------------------------------------------
    /**
     * @brief validates bounds only
     * @param size is used to solve relative svg coordinates
     */
    void validateBounds(const Dimension &size);
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
    Vertex getRelatedVertex(SceneGraphElement el) const;
    //-------------------------------------------------------------------------
    SceneGraphElement getRootElement() {
	if (boost::num_vertices(g) == 0) {
	    return SceneGraphElement();
	}
	return getSceneGraphElement(*(boost::vertices(g).first));
    }
    //-------------------------------------------------------------------------
    /**
     * Assumes that graph contains vertex.
     * @param v
     * @return
     */
    SceneGraphElement getSceneGraphElement(const Vertex &v) const;
    //-------------------------------------------------------------------------
    void draw(IDrawContext::Ptr);
    //-------------------------------------------------------------------------
    /**
     * creates a transformation node and relates it to el
     * @param el a SceneGraphElement of this graph object
     * @param m a transformation matrix
     */
    bool setTransfomationTo(SceneGraphElement el, const Matrix &m);
    //-------------------------------------------------------------------------
    /**
     * creates a style node and relates it to el
     * @param el a SceneGraphElement of this graph object
     * @param s a @see Style object
     */
    bool setStyleTo(SceneGraphElement el, const graphicElements::Style &s);
    //-------------------------------------------------------------------------
    /**
     * creates a transformation node and relates it to el
     * @param el a SceneGraphElement of this graph object
     * @param m a transformation matrix
     */
    bool setTransfomationRefTo(SceneGraphElement el, MatrixPtr m);
    //-------------------------------------------------------------------------
    /**
     * creates a style node and relates it to el
     * @param el a SceneGraphElement of this graph object
     * @param s a @see Style object
     */
    bool setStyleRefTo(SceneGraphElement el, StylePtr s);
    //-------------------------------------------------------------------------
    /**
     * @param el
     * @param className
     * @return true when element registration was successfull
     */
    bool registerElementClass(SceneGraphElement el, const Class &className );
    //-------------------------------------------------------------------------
    MatrixPtr getTransformationRef(Vertex v) const;
    //-------------------------------------------------------------------------
    MatrixPtr getTransformationRef(SceneGraphElement el) const;
    //-------------------------------------------------------------------------
    /**
     * @return the transformation matrix of an element
     * @note this does not considering the scene tree
     * Use @see calculateTransformation() for this purpose.
     */
    Matrix getTransformationOf(SceneGraphElement el) const {
	MatrixPtr res = getTransformationRef(el);
	if (!res)
	    return NULL_MATRIX;
	return *(res.get());
    }
    //-------------------------------------------------------------------------
    /**
     * @return the style related to an element
     * @note this style does not considering the scene tree, which means
     * if a parent element is filled red and the target element isn't filled,
     * the resulting style has no fill attribute. 
     * Use @see calculateStyle() instead.
     */
    graphicElements::Style getStyleOf(SceneGraphElement el) const {
	StylePtr res = getStyleRef(el);
	if (!res)
	    return graphicElements::Style::getNullStyle();
	return *(res.get());
    }
    //-------------------------------------------------------------------------
    /**
     * @brief calculates the style of an element considering the scene
     * tree.
     */
    graphicElements::Style calculateStyle(SceneGraphElement el);
    //-------------------------------------------------------------------------
    /**
     * @brief calculates the transformatin of an element considering the scene
     * tree.
     */
    Matrix calculateTransformation(SceneGraphElement el); 
    //-------------------------------------------------------------------------
    StylePtr getStyleRef(Vertex v) const;
    //-------------------------------------------------------------------------
    StylePtr getStyleRef(SceneGraphElement el) const;
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
	if ( boost::num_vertices(g) > 0 && processList.empty() ) {
            createProcessList(processList);
	}
	return processList;
    }
    //-------------------------------------------------------------------------
    /**
     * return a "list" of all graph contained elements sorted topological.
     * @param out container
     */
    template<typename _Container>
    void createProcessList(_Container &out);
    //-------------------------------------------------------------------------
    /**
     * for testing and debugging
     * @return
     */
    std::string processListAsString();
    //-------------------------------------------------------------------------
    /**
     * fills _Container with all parent vertices which matches to VertexType.
     * @param v
     * @param out
     */
    template<typename _Container>
    void findParentsByType(const Vertex& v, VertexType type, _Container &out) const;
    //-------------------------------------------------------------------------
    size_t inDegreeOf(const Vertex& v, VertexType type) const;
    //-------------------------------------------------------------------------
    bool addElement( SceneGraphElement ptr );
    //-------------------------------------------------------------------------
    bool connectElements(SceneGraphElement from, SceneGraphElement to);
    //-------------------------------------------------------------------------
    bool registerElementId(SceneGraphElement el, const Id &id) {
	Vertex v = getRelatedVertex(el);
	if (v==NULL_VERTEX)
	    return false;
	vertexNameMap[v] = id;
	return (id2Vertex.insert( std::make_pair(id, v) )).second;

    }
    //-------------------------------------------------------------------------
    void setTagName(SceneGraphElement el, const Tag &tagName) {
	Vertex v = getRelatedVertex(el);
	if (v==NULL_VERTEX)
	    return;
	vertexName2Map[v] = tagName;
	tag2Vertex.insert(std::make_pair(tagName, v));
    }
    //-------------------------------------------------------------------------
    struct AllElements {
	bool filter(SceneGraphElement el) const { return true; }
    };
    //-------------------------------------------------------------------------
    struct TagFilter {
	Tag tag;
	const SceneGraph &g;
	TagFilter(const Tag &tag, const SceneGraph &g) : tag(tag), g(g) {}
	bool filter(SceneGraphElement el) const {
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
	bool filter(SceneGraphElement el) const {
	    using namespace boost::algorithm;
	    std::list<Class> classes;
	    g.getClassNames(el, classes);
	    boost_for_each(const Class &cn, classes) {
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
	bool filter(SceneGraphElement el) const {
	    using namespace boost::algorithm;
	    if (to_lower_copy(id) == to_lower_copy(g.getIdName(el)))
		return true;
	    return false;
	}
    };
    //-------------------------------------------------------------------------
    template <typename _Container, typename Filter>
    void getChildren(SceneGraphElement el,
		     _Container &c,
		     const Filter &filter, bool deep = true ) const
	{
	    Vertex v = getRelatedVertex(el);
	    if (v==NULL_VERTEX)
		return;
	    if (deep) {
		BFSVisitor<_Container, Filter> bfsVis(*this, c, filter);
		boost::breadth_first_search(g, v, boost::visitor(bfsVis));
		return;
	    }
	    AdjacencyIterator it, end;
	    boost::tie(it, end) = boost::adjacent_vertices(v, g);
	    for (; it!=end; ++it) {
		SceneGraphElement el = getSceneGraphElement(*it);
		if (filter.filter(el))
		    c.push_back(el);
	    }
	}
    //-------------------------------------------------------------------------
    template <typename _Container>
    void getChildren(SceneGraphElement el,
		     _Container &c,
		     bool deep = true) const
	{
	    AllElements filter;
	    getChildren(el, c, filter, deep);
	}
    //-------------------------------------------------------------------------
    template <typename _Container>
    void getChildrenByTag(SceneGraphElement el,
			  const Tag &tagName,
			  _Container &c,
			  bool deep = true) const
	{
	    TagFilter filter(tagName, *this);
	    getChildren(el, c, filter, deep);
	}
    //-------------------------------------------------------------------------
    template <typename _Container>
    void getChildrenByClass(SceneGraphElement el,
			    const Class &className,
			    _Container &c,
			    bool deep = true) const
	{
	    ClassFilter filter(className, *this);
	    getChildren(el, c, filter, deep);
	}
    //-------------------------------------------------------------------------
    template <typename _Container>
    void getChildrenById(SceneGraphElement el,
			 const Id &id,
			 _Container &c,
			 bool deep = true) const
	{
	    IdFilter filter(id, *this);
	    getChildren(el, c, filter, deep);
	}
    //-------------------------------------------------------------------------
    Tag getTagName(SceneGraphElement el) const {
	Vertex v = getRelatedVertex(el);
	if (v == NULL_VERTEX)
	    return "";
	return vertexName2Map[v];
    }
    //-------------------------------------------------------------------------
    template <typename _Container>
    void getClassNames(SceneGraphElement el, _Container &c) const {
	Vertex v = getRelatedVertex(el);
	if (v == NULL_VERTEX)
	    return;
	std::list<Vertex> l;
	findParentsByType(v, CLASS, l);
	boost_for_each(const Vertex &it, l) {
	    c.push_back(vertexNameMap[it]);
	}
    }
    //-------------------------------------------------------------------------
    Id getIdName(SceneGraphElement el) const {
	Vertex v = getRelatedVertex(el);
	if (v == NULL_VERTEX)
	    return "";
	return vertexNameMap[v];
    }
    //-------------------------------------------------------------------------
    IDrawable::Ptr getElementById(const Id &id) const;
    //-------------------------------------------------------------------------
    template <typename _Container>
    void getElementsByClass(const Class & className, _Container &c);
    //-------------------------------------------------------------------------
    template <typename _Container>
    void getElementsByTag(const Tag & tagName, _Container &c);
    //-------------------------------------------------------------------------
    /**
     * appends edge from src's style node to dst
     * @param src
     * @param dst
     */
    void adoptStyleEdges(SceneGraphElement src,
			 SceneGraphElement dst)
	{
	    Vertex srcV = getRelatedVertex(src);
	    Vertex dstV = getRelatedVertex(dst);
	    if (srcV==NULL_VERTEX || dstV==NULL_VERTEX )
		return;
	    std::list<Vertex> vertices;
	    findParentsByType(srcV, SceneGraph::STYLE, vertices);
	    boost_for_each(Vertex v, vertices) {
		boost::add_edge(v, dstV, g);
	    }
	}
    //-------------------------------------------------------------------------
    /**
     * appends edge from src's transformation node to dst
     * @param src
     * @param dst
     */
    void adoptTransformEdges(SceneGraphElement src,
			     SceneGraphElement dst)
	{
	    Vertex srcV = getRelatedVertex(src);
	    Vertex dstV = getRelatedVertex(dst);
	    if (srcV==NULL_VERTEX || dstV==NULL_VERTEX )
		return;
	    std::list<Vertex> vertices;
	    findParentsByType(srcV, SceneGraph::TRANSFORM, vertices);
	    boost_for_each(const Vertex &v, vertices) {
		boost::add_edge(v, dstV, g);
	    }
	}
    //-------------------------------------------------------------------------
    /**
     * @return bounds or NULL_RECTANGLE if obj not found
     * @deprecated context isn't used anymore
     */
    Rectangle getBoundingBox(SceneGraphElement obj, IDrawContext::Ptr) const;
    //-------------------------------------------------------------------------
    /**
     * @return bounds or NULL_RECTANGLE if obj not found
     */
    Rectangle getBoundingBox(SceneGraphElement obj) const;
};
//=============================================================================
/**
 *  @class DFSVisitor
 *  fills a container with @see ProcessListProcessor objects, topological sorted.
 *  http://www.4divisions.com/forx/wiki/doku.php?id=wiki:scenegraph&#building_process_list
 */
//=============================================================================
template <typename _Container>
class DFSVisitor : public boost::dfs_visitor<> {
private:
    //-------------------------------------------------------------------------
    _Container &container;
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
	ProcessDrawable::Ptr cmd = ProcessDrawable::create(obj,
							   numOutEdges==0,
							   sceneGraph.getStyleRef(obj),
							   sceneGraph.getTransformationRef(obj)
	    );
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
    DFSVisitor(const SceneGraph &sc, _Container &container, Vertex startVertex) :
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
	if G2 type is a "delegate as graph" type. ( for all I know )
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
template<typename _Container>
void SceneGraph::createProcessList (_Container &out)
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
{
	/**
	 * @brief copies org. graph into a "delegate as graph" graph.
	 * where the edge _Container is a set<vertex> to affect in which order
	 * the depth_first_search alg. selects the out_edges.
	 * see: http://www.4divisions.com/forx/wiki/doku.php?id=wiki:scenegraph#building_process_list
	 */
	if (boost::num_vertices(g)==0)
		return;
	Vertex startVertex =  *(boost::vertices(g).first);
	DFSVisitor<_Container> vis(*this, out, startVertex);
	typedef CompareNodeOrder<Vertex> Comparator;
	typedef std::set<Vertex, Comparator> Set;
	typedef std::vector<Set> G2;
	G2 g2;
	Comparator cmp(this);
	copyGraph<G, G2, Set, Comparator>(g,g2,cmp); // O(n)

	boost::depth_first_search(
		g2
		, boost::visitor(vis)
	);
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template<typename _Container>
void SceneGraph::findParentsByType(const Vertex& v, VertexType type, _Container &out) const
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
template <typename _Container>
void SceneGraph::getElementsByClass(const Class & className, _Container &c) {
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
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template <typename _Container>
void SceneGraph::getElementsByTag(const Tag & tagName, _Container &c) {
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Tag2Vertex::const_iterator it, end;
	boost::tie(it, end) = tag2Vertex.equal_range(tagName);
	for (; it!=end; ++it) {
		c.push_back(getSceneGraphElement(it->second));
	}
}
}}}} // namespace


#endif /* SCENEGRAPH_HPP_ */
