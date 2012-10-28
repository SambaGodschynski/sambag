/*
 * DefaultTreeModel.hpp
 *
 *  Created on: Thu Sep 20 10:36:42 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DEFAULTTREEMODEL_H
#define SAMBAG_DEFAULTTREEMODEL_H

#include <boost/shared_ptr.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/foreach.hpp>
#include <sambag/com/events/Events.hpp>
#include <sambag/com/ArithmeticWrapper.hpp>

namespace sambag { namespace disco { namespace components {
namespace sce = sambag::com::events;
struct TreeModelStateChanged {};
//=============================================================================
/** 
  * @class DefaultTreeModel.
  * A tree model impl. using boost::bgl
  */
template <class _NodeDataType>
class DefaultTreeModel : sce::EventSender<TreeModelStateChanged> {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef _NodeDataType NodeDataType;
	//-------------------------------------------------------------------------
	typedef TreeModelStateChanged Event;
protected:
	//-------------------------------------------------------------------------
	struct node_data_t { typedef boost::vertex_property_tag kind; };
	//-------------------------------------------------------------------------
	typedef boost::property<node_data_t, NodeDataType> vertexProperties;
public:
	//-------------------------------------------------------------------------
	/**
	 * Graph
	 */
	typedef boost::adjacency_list<boost::vecS,
			boost::vecS,
			boost::bidirectionalS,
			vertexProperties> TreeType;
protected:
	//-------------------------------------------------------------------------
	TreeType tree;
	//-------------------------------------------------------------------------
	typedef typename
			boost::property_map<TreeType, node_data_t>::type VertexDataMap;
	//-------------------------------------------------------------------------
	typedef typename boost::graph_traits<TreeType>::vertex_descriptor Vertex;
	//-------------------------------------------------------------------------
	typedef typename boost::graph_traits<TreeType>::edge_descriptor Edge;
	//-------------------------------------------------------------------------
	typedef typename
			boost::graph_traits<TreeType>::vertex_iterator VertexIterator;
	//-------------------------------------------------------------------------
	typedef typename
			boost::graph_traits<TreeType>::edge_iterator EdgeIterator;
	//-------------------------------------------------------------------------
	typedef typename
			boost::graph_traits<TreeType>::in_edge_iterator InEdgeIterator;
	//-------------------------------------------------------------------------
	typedef typename
			boost::graph_traits<TreeType>::out_edge_iterator OutEdgeIterator;
	//-------------------------------------------------------------------------
	typedef typename
			boost::graph_traits<TreeType>::adjacency_iterator AdjacencyIterator;
	//-------------------------------------------------------------------------
	typedef typename TreeType::inv_adjacency_iterator InvAdjacencyIterator;
	//-------------------------------------------------------------------------
	VertexDataMap vertexDataMap;
	//-------------------------------------------------------------------------
	Vertex root;
	//-------------------------------------------------------------------------
	static const Vertex NULL_NODE;
public:
	//-------------------------------------------------------------------------
	typedef Vertex Node;
private:
	//-------------------------------------------------------------------------
	void removeNodeImpl(Node node);
	//-------------------------------------------------------------------------
	/**
	 * @deprecated
	 */
	size_t sizeImpl(Node node) const;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<int, 1> numNodes;
public:
	//-------------------------------------------------------------------------
	static const NodeDataType NULL_NODE_DATA;
	//-------------------------------------------------------------------------
	DefaultTreeModel();
	//-------------------------------------------------------------------------
	/**
	 * @param rootData
	 */
	DefaultTreeModel(const NodeDataType &rootData);
	//-------------------------------------------------------------------------
	/**
	 *
	 * @return
	 */
	Node getRootNode() const {
		return root;
	}
	//-------------------------------------------------------------------------
	/**
	 *
	 * @return
	 */
	const TreeType & getTreeImpl() const {
		return tree;
	}
	//-------------------------------------------------------------------------
	const NodeDataType & getNodeData(Node node) const;
	//-------------------------------------------------------------------------
	void setNodeData(Node node, const NodeDataType &v);
	//-------------------------------------------------------------------------
	/**
	 * appends node to parent
	 * @param parent
	 * @param node
	 */
	Node addNode(Node parent, const NodeDataType &data);
	//-------------------------------------------------------------------------
	/**
	 * appends node to parent
	 * @param parent
	 * @param node
	 */
	Node addNode(Node parent);
	//-------------------------------------------------------------------------
	/**
	 * removes node and it's children
	 * @note: this method removes all nodes connections only. So all concerning
	 * nodes still exists (dead) in tree. This is because bgl::remove_vertex 
	 * function makes DefaultTreeModel::Node objects invalid.
	 * If this getting a problem the Node type has to getting a unique id with
	 * extra mapping.
	 * @param node
	 */
	void removeNode(Node node);
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param
	 * @return
	 */
	Node getParent(Node node) const;
	//-------------------------------------------------------------------------
	size_t getNumChildren(Node node) const {
		return boost::out_degree(node, tree);
	}
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param node
	 * @param c
	 */
	template <class NodeContainer>
	void getChildren(Node node, NodeContainer &c) const;
	//-------------------------------------------------------------------------
	/**
	 * @return number of nodes inclusive rootnode.
	 */
	size_t size() const;
}; // DefaultTreeModel
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class NT>
const typename DefaultTreeModel<NT>::Vertex
DefaultTreeModel<NT>::NULL_NODE = INT_MAX;
//-----------------------------------------------------------------------------
template <class NT>
const typename DefaultTreeModel<NT>::NodeDataType
DefaultTreeModel<NT>::NULL_NODE_DATA;
//-----------------------------------------------------------------------------
template <class NT>
DefaultTreeModel<NT>::DefaultTreeModel() {
	vertexDataMap = get( node_data_t(), tree );
	root = add_vertex(tree);
	vertexDataMap[root] = NULL_NODE_DATA;
}
//-----------------------------------------------------------------------------
template <class NT>
DefaultTreeModel<NT>::DefaultTreeModel(
		const typename DefaultTreeModel<NT>::NodeDataType &rootData)
{
	vertexDataMap = get( node_data_t(), tree );
	root = add_vertex(tree);
	vertexDataMap[root] = rootData;
}
//-----------------------------------------------------------------------------
template <class NT>
size_t DefaultTreeModel<NT>::sizeImpl(typename DefaultTreeModel<NT>::Node node)
const
{
	// remove children
	AdjacencyIterator it, end;
	boost::tie(it, end) = boost::adjacent_vertices(node, tree);
	size_t res = 1;
	for (; it!=end; ++it) {
		res += sizeImpl(*it);
	} 
	return res;
}
//-----------------------------------------------------------------------------
template <class NT>
size_t DefaultTreeModel<NT>::size() const
{
	return numNodes;
}
//-----------------------------------------------------------------------------
template <class NT>
typename DefaultTreeModel<NT>::Node DefaultTreeModel<NT>::addNode(
		typename DefaultTreeModel<NT>::Node parent,
		const typename DefaultTreeModel<NT>::NodeDataType &data)
{
	const Vertex &u = add_vertex(tree);
	vertexDataMap[u] = data;
	boost::add_edge(parent, u, tree);
	numNodes = numNodes + 1;
	EventSender<Event>::notifyListeners(this, Event());
	return u; 
}
//-----------------------------------------------------------------------------
template <class NT>
typename DefaultTreeModel<NT>::Node DefaultTreeModel<NT>::addNode(
		typename DefaultTreeModel<NT>::Node parent)
{
	const Vertex &u = add_vertex(tree);
	boost::add_edge(parent, u, tree);
	numNodes = numNodes + 1;
	EventSender<Event>::notifyListeners(this, Event());
	return u; 
}
//-----------------------------------------------------------------------------
template <class NT>
void DefaultTreeModel<NT>::removeNodeImpl(typename DefaultTreeModel<NT>::Node node)
{
	// remove children
	AdjacencyIterator it, end;
	boost::tie(it, end) = boost::adjacent_vertices(node, tree);
	
	while (it!=end) {
		removeNodeImpl(*it);
		boost::tie(it, end) = boost::adjacent_vertices(node, tree);
	}
	boost::clear_vertex(node, tree);
	numNodes = numNodes - 1;
}
//-----------------------------------------------------------------------------
template <class NT>
void DefaultTreeModel<NT>::removeNode(
		typename DefaultTreeModel<NT>::Node node)
{
	if (node==root)
		return;
	removeNodeImpl(node);
	EventSender<Event>::notifyListeners(this, Event());
}
//-----------------------------------------------------------------------------
template <class NT>
typename DefaultTreeModel<NT>::Node DefaultTreeModel<NT>::
getParent(typename DefaultTreeModel<NT>::Node node) const
{
}
//-----------------------------------------------------------------------------
template <class NT>
template <class NodeContainer>
void DefaultTreeModel<NT>::getChildren(
		typename DefaultTreeModel<NT>::Node node,
		NodeContainer &c) const
{
	AdjacencyIterator it, end;
	boost::tie(it, end) = boost::adjacent_vertices(node, tree);
	for (; it!=end; ++it) {
		c.push_back(*it);
	}
}
//-----------------------------------------------------------------------------
template <class NT>
const typename DefaultTreeModel<NT>::NodeDataType & DefaultTreeModel<NT>::
getNodeData(typename DefaultTreeModel<NT>::Node node) const
{
	return vertexDataMap[node];
}
//-----------------------------------------------------------------------------
template <class NT>
void DefaultTreeModel<NT>::setNodeData(typename DefaultTreeModel<NT>::Node node,
		const typename DefaultTreeModel<NT>::NodeDataType &v)
{
	vertexDataMap[node] = v;
	EventSender<Event>::notifyListeners(this, Event());
}
}}} // namespace(s)

#endif /* SAMBAG_DEFAULTTREEMODEL_H */
