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

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class DefaultTreeModel.
  * A tree model impl. using boost::bgl
  */
template <class _NodeDataType>
class DefaultTreeModel {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef _NodeDataType NodeDataType;
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
public:
	//-------------------------------------------------------------------------
	typedef Vertex Node;
private:
	//-------------------------------------------------------------------------
	template <class VertexContainer, class EdgeContainer>
	void removeNodeImpl(Node node, VertexContainer &vertices,
			EdgeContainer &edges);
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
	 * removes node and it's children
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
	size_t size() const {
		return boost::num_vertices(tree);
	}
}; // DefaultTreeModel
///////////////////////////////////////////////////////////////////////////////
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
		const DefaultTreeModel<NT>::NodeDataType &rootData)
{
	vertexDataMap = get( node_data_t(), tree );
	root = add_vertex(tree);
	vertexDataMap[root] = rootData;
}
//-----------------------------------------------------------------------------
template <class NT>
typename DefaultTreeModel<NT>::Node DefaultTreeModel<NT>::addNode(
		DefaultTreeModel<NT>::Node parent,
		const DefaultTreeModel<NT>::NodeDataType &data)
{
	const Vertex &u = add_vertex(tree);
	vertexDataMap[u] = data;
	boost::add_edge(parent, u, tree);
	return u;
}
//-----------------------------------------------------------------------------
template <class NT>
template <class VertexContainer, class EdgeContainer>
void DefaultTreeModel<NT>::removeNodeImpl(DefaultTreeModel<NT>::Node node,
		VertexContainer &vertices, EdgeContainer &edges)
{
	// remove children
	AdjacencyIterator it, end;
	boost::tie(it, end) = boost::adjacent_vertices(node, tree);
	for (; it!=end; ++it) {
		removeNodeImpl(*it, vertices, edges);
	}
	// remove in-edge(s)
	InEdgeIterator iit, ie;
	boost::tie(iit,ie) = boost::in_edges(node, tree);
	for ( ; iit!=ie; ++iit) {
		edges.push_back(*iit);
	}
	vertices.push_back(node);
}
//-----------------------------------------------------------------------------
template <class NT>
void DefaultTreeModel<NT>::removeNode(
		DefaultTreeModel<NT>::Node node)
{
	if (node==root)
		return;
	// first collect elements to remove, then remove.
	std::list<Node> nodeList;
	std::list<Edge> edgeList;
	removeNodeImpl(node, nodeList, edgeList);
	BOOST_FOREACH(Edge edge, edgeList) {
		boost::remove_edge(edge, tree);
	}
	BOOST_FOREACH(Node node, nodeList) {
		boost::remove_vertex(node, tree);
	}
}
//-----------------------------------------------------------------------------
template <class NT>
typename DefaultTreeModel<NT>::Node DefaultTreeModel<NT>::
getParent(DefaultTreeModel<NT>::Node node) const
{
}
//-----------------------------------------------------------------------------
template <class NT>
template <class NodeContainer>
void DefaultTreeModel<NT>::getChildren(
		DefaultTreeModel<NT>::Node node,
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
getNodeData(DefaultTreeModel<NT>::Node node) const
{
	return vertexDataMap[node];
}
//-----------------------------------------------------------------------------
template <class NT>
void DefaultTreeModel<NT>::setNodeData(DefaultTreeModel<NT>::Node node,
		const DefaultTreeModel<NT>::NodeDataType &v)
{
	vertexDataMap[node] = v;
}
}}} // namespace(s)

#endif /* SAMBAG_DEFAULTTREEMODEL_H */
