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
	void removeNode(Node node);
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param
	 * @return
	 */
	Node getParent(Node node) const;
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
namespace {
struct BFSVisitor : public boost::bfs_visitor<> {
	template <class Edge, class Graph>
	void back_edge(Edge, Graph&) const {}
	template <class Vertex, class Graph>
	void discover_vertex(const Vertex &v, Graph &g) {}
	template <class Vertex, class Graph>
	void finish_vertex(const Vertex &v, Graph &g) {}
	template <class Vertex, class Graph>
	void initialize_vertex(const Vertex &v, Graph&) {}
};
} // namespace
template <class NT>
void DefaultTreeModel<NT>::removeNode(
		DefaultTreeModel<NT>::Node node)
{
	boost::breadth_first_search(
			tree,
			node,
			boost::visitor(BFSVisitor())
	);

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
