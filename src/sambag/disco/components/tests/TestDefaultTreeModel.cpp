/*
 * TestDefaultTreeModel.cpp
 *
 *  Created on: Thu Sep 20 10:36:33 2012
 *      Author: Johannes Unger
 */

#include "TestDefaultTreeModel.hpp"
#include <cppunit/config/SourcePrefix.h>
#include <sambag/disco/components/DefaultTreeModel.hpp>
#include <string>
#include <vector>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::TestDefaultTreeModel );

namespace tests {
//=============================================================================
//  Class TestDefaultTreeModel
//=============================================================================
//-----------------------------------------------------------------------------
void TestDefaultTreeModel::testConstructor() {
	using namespace sambag::disco::components;
	typedef DefaultTreeModel<std::string> Tree;
	Tree tree;
	CPPUNIT_ASSERT_EQUAL(
			Tree::NULL_NODE_DATA, tree.getNodeData(tree.getRootNode())
	);
	Tree tree02("root");
	CPPUNIT_ASSERT_EQUAL(
			std::string("root"), tree02.getNodeData(tree02.getRootNode())
	);
}
//-----------------------------------------------------------------------------
void TestDefaultTreeModel::testAddRemove() {
	// Root
	//  +
	//  +--------folder0
	//  +          +
	//  +          +-file1
	//  +--------folder1
	//  +          +
	//  +          +------folder2
	//  +          +        +
	//  +-file1    +        +-file1
	//             +-file1
	//
	using namespace sambag::disco::components;
	typedef DefaultTreeModel<std::string> Tree;
	Tree tree;
	Tree::Node tmp;
	Tree::Node folder1 = tree.addNode(tree.getRootNode(), "folder1");
	tmp = tree.addNode(tree.getRootNode(), "file1");
	tmp = tree.addNode(folder1, "file1");
	Tree::Node folder2 = tree.addNode(folder1, "folder2");
	tmp = tree.addNode(folder2, "file1");
	CPPUNIT_ASSERT_EQUAL((size_t)6, tree.size());
	CPPUNIT_ASSERT_EQUAL((size_t)2, tree.getNumChildren(tree.getRootNode()));
	std::vector<Tree::Node> nl;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<root
	tree.getChildren(tree.getRootNode(), nl);
	CPPUNIT_ASSERT_EQUAL((size_t)2, nl.size());
	CPPUNIT_ASSERT_EQUAL(tree.getNodeData(nl[0]), std::string("folder1"));
	CPPUNIT_ASSERT_EQUAL(tree.getNodeData(nl[1]), std::string("file1"));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<folder1
	nl.clear();
	tree.getChildren(folder1, nl);
	CPPUNIT_ASSERT_EQUAL((size_t)2, nl.size());
	CPPUNIT_ASSERT_EQUAL(tree.getNodeData(nl[0]), std::string("file1"));
	CPPUNIT_ASSERT_EQUAL(tree.getNodeData(nl[1]), std::string("folder2"));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<folder2
	nl.clear();
	tree.getChildren(folder2, nl);
	CPPUNIT_ASSERT_EQUAL((size_t)1, nl.size());
	CPPUNIT_ASSERT_EQUAL(tree.getNodeData(nl[0]), std::string("file1"));
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<remove folder1
	tree.removeNode(folder1);
	CPPUNIT_ASSERT_EQUAL((size_t)2, tree.size());
	nl.clear();
	tree.getChildren(tree.getRootNode(), nl);
	CPPUNIT_ASSERT_EQUAL((size_t)1, nl.size());
	CPPUNIT_ASSERT_EQUAL(tree.getNodeData(nl[0]), std::string("file1"));
}
} //namespace
