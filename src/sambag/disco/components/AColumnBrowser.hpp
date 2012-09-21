/*
 * AColumnBrowser.hpp
 *
 *  Created on: Thu Sep 20 20:14:13 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_ACOLUMNBROWSER_H
#define SAMBAG_ACOLUMNBROWSER_H

#include <boost/shared_ptr.hpp>
#include "ColumnView.hpp"
#include <sambag/com/ArithmeticWrapper.hpp>
#include <boost/foreach.hpp>
#include "Forward.hpp"
#include <sambag/com/events/Events.hpp>
#include "ui/UIManager.hpp"
#include "ui/ALookAndFeel.hpp"
#include <list>
#include "AList.hpp"
#include "DefaultListCellRenderer.hpp"
#include "DefaultListModel.hpp"
#include "DefaultListSelectionModel.hpp"

namespace sambag { namespace disco { namespace components {
namespace sce = sambag::com::events;
template <class TreeModel>
class AColumnBrowser;
template <class TreeModel>
struct SelectionPathChanged {
	typedef boost::shared_ptr<AColumnBrowser<TreeModel> > ColumnBrowserPtr;
	ColumnBrowserPtr src;
	SelectionPathChanged(ColumnBrowserPtr src) : src(src) {}
};
//=============================================================================
// @class ColumnBrowserListCellRenderer
//=============================================================================
template <class T>
class ColumnBrowserListCellRenderer :
	public DefaultListCellRenderer<T>
{
public:
	//-------------------------------------------------------------------------
	typedef DefaultListCellRenderer<T> Super;
	//-------------------------------------------------------------------------
	typedef ColumnBrowserListCellRenderer<T> Class;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Class> Ptr;
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(Class)
	//-------------------------------------------------------------------------
	template <class ListType>
	AComponentPtr getListCellRendererComponent(
			boost::shared_ptr<ListType> list, // the list
			const T &value, // value to display
			int index, // cell index
			bool isSelected, // is the cell selected
			bool cellHasFocus // does the cell have focus
	)
	{
		if (isSelected) {
			Super::setBackground(list->getSelectionBackground());
			Super::setForeground(list->getSelectionForeground());
		} else {
			Super::setBackground(list->getBackground());
			Super::setForeground(list->getForeground());
		}
		if (!value.parent)
			return getPtr();
		if (value.parent->isFolder(value.node)) {
			setText(sambag::com::toString(value.data) + " >");
		} else {
			setText(sambag::com::toString(value.data));
		}
		Super::setEnabled(list->isEnabled());
		Super::setFont(list->getFont());

		return getPtr();

	}
};
//=============================================================================
// @class ColumnBrowserList
//=============================================================================
template <class T>
class ColumnBrowserList :
	public AList<  T,
					ColumnBrowserListCellRenderer,
					DefaultListModel,
					DefaultListSelectionModel
				>
{
public:
	//-------------------------------------------------------------------------
	typedef ColumnBrowserList<T> Class;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<Class> Ptr;
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(Class)
	//-------------------------------------------------------------------------
	ui::AComponentUIPtr
	createComponentUI(ui::ALookAndFeelPtr laf) const {
		return laf->getUI<Class>();
	}

};
//=============================================================================
/** 
  * @class AColumnBrowser.
  * TODO: CellRenderer as template policy
  */
template <class TreeModel>
class AColumnBrowser : public AContainer,
	public TreeModel,
	public sce::EventSender<SelectionPathChanged<TreeModel> >
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef TreeModel Model;
	//-------------------------------------------------------------------------
	typedef AColumnBrowser<Model> Class;
	//-------------------------------------------------------------------------
	typedef SelectionPathChanged<TreeModel> Event;
	//-------------------------------------------------------------------------
	typedef typename Model::NodeDataType ModelDataType;
	//-------------------------------------------------------------------------
	typedef typename TreeModel::Node Node;
	//-------------------------------------------------------------------------
	struct Entry {
		Class *parent;
		ModelDataType data;
		Node node;
		Entry() : parent(NULL) {}
		Entry(Class *parent, const ModelDataType &data, Node node)
		: parent(parent), data(data), node(node)
		{
		}
		bool operator==(const Entry &b) const {
			return parent==b.parent && node==b.node && data==b.data;
		}
		bool operator!=(const Entry &b) const {
			return !(*this==b);
		}
		operator ModelDataType() const {
			return data;
		}
	};
	//-------------------------------------------------------------------------
	typedef ColumnBrowserList<Entry> ListType;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ListType> ListTypePtr;
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef ColumnView<ListType> ColumnViewClass;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<AColumnBrowser> Ptr;
	//-------------------------------------------------------------------------
	typedef std::vector<Node> Path;
protected:
	//-------------------------------------------------------------------------
	AColumnBrowser() {}
	//-------------------------------------------------------------------------
	virtual void postConstructor();
	//-------------------------------------------------------------------------
	void addList(int listIndex);
	//-------------------------------------------------------------------------
	typedef typename ListType::ListSelectionModel::Event ListSelectionEvent;
	//-------------------------------------------------------------------------
	void onSelectionChanged(void *src,
		const ListSelectionEvent &ev, int listIndex);
	//-------------------------------------------------------------------------
	enum {ListNotFound = -2, NotEnoughLists = -1};
	int getListIndex(const Node &node) const;
private:
	//-------------------------------------------------------------------------
	void updateList(Node parent, int listIndex);
	//-------------------------------------------------------------------------
	void updateLists(const Path &path);
	//-------------------------------------------------------------------------
	Path selectionPath;
	//-------------------------------------------------------------------------
	void init();
	//-------------------------------------------------------------------------
	typename ColumnViewClass::Ptr columnView;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<int> currListIndex;
public:
	//-------------------------------------------------------------------------
	void updateLists() {
		updateLists(selectionPath);
	}
	//-------------------------------------------------------------------------
	std::string selectionPathToString() const;
	//-------------------------------------------------------------------------
	const Path & getSelectionPath() const {
		return selectionPath;
	}
	//-------------------------------------------------------------------------
	Ptr getPtr() const {
		return boost::shared_dynamic_cast<AColumnBrowser>(Super::getPtr());
	}
	//-------------------------------------------------------------------------
	bool isFolder(Node node) const {
		return Model::getNumChildren(node) > 0;
	}
}; // AColumnBrowser
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class TM>
int AColumnBrowser<TM>::getListIndex(const Node &node) const {
	if (selectionPath.size() == 1) // root
		return 0;
	int c=0;
	int index=ListNotFound;
	BOOST_FOREACH(const Node &n, selectionPath) {
		if (n==node) {
			index = c+1;
			break;
		}
		++c;
	}
	if (index==ListNotFound)
		return ListNotFound;
	if (index > (int)columnView->getNumLists())
		return NotEnoughLists;
	return index;
}
//-----------------------------------------------------------------------------
template <class TM>
void AColumnBrowser<TM>::postConstructor() {
	Super::postConstructor();
	init();
}
//-----------------------------------------------------------------------------
template <class TM>
void AColumnBrowser<TM>::addList(int listIndex)
{
	Coordinate fontSize = 20.;
	ui::getUIManager().getProperty("ColumnBrowser.fontSize", fontSize);
	columnView->setFont(columnView->getFont().setSize(fontSize));
	Coordinate fixedColumnWitdh = 120.;
	ui::getUIManager().getProperty("ColumnBrowser.fixedColumnWidth", fixedColumnWitdh);
	columnView->setFont(columnView->getFont().setSize(fontSize));

	ListTypePtr list = columnView->addList();
	list->setFixedCellWidth(fixedColumnWitdh);
	list->sce::EventSender<ListSelectionEvent>::addTrackedEventListener(
		boost::bind(&AColumnBrowser::onSelectionChanged,this,_1,_2,listIndex),
		getPtr()
	);
}
//-----------------------------------------------------------------------------
template <class TM>
void AColumnBrowser<TM>::init() {
	setLayout(BorderLayout::create());
	columnView = ColumnViewClass::create();
	selectionPath.reserve(10);
	selectionPath.push_back(Model::getRootNode());
	Model::setNodeData(Model::getRootNode(), "root");
	int numInitLists = 3;
	ui::getUIManager().getProperty("AColumnBrowser.numInitLists", numInitLists);
	for (int i=0; i<numInitLists; ++i) {
		addList(i);
	}
	add(columnView);
}
//-----------------------------------------------------------------------------
template <class TM>
void AColumnBrowser<TM>::updateList(AColumnBrowser<TM>::Node parent,
		int listIndex)
{
	ListTypePtr list = columnView->getList(listIndex);
	list->clear();
	int num = Model::getNumChildren(parent);
	if (num==0)
		return;
	std::vector<Node> nodes;
	nodes.reserve(num);
	Model::getChildren(parent, nodes);
	for (int i=0; i<(int)nodes.size(); ++i) {
		const ModelDataType &data = Model::getNodeData(nodes[i]);
		list->addElement(Entry(this, data, nodes[i]));
	}
	list->redraw();
}
//-----------------------------------------------------------------------------
template <class TM>
void AColumnBrowser<TM>::updateLists(const AColumnBrowser<TM>::Path &path)
{
	// root
	for (int i=0; i<(int)path.size(); ++i) {
		updateList(path[i], i);
	}
}
//-----------------------------------------------------------------------------
template <class TM>
void AColumnBrowser<TM>::onSelectionChanged(void *src,
	const ListSelectionEvent &ev, int listIndex)
{
	ListTypePtr list = columnView->getList(listIndex);
	Entry e = list->ListType::ListModel::get(ev.getFirstIndex());
	int currSelPathIndex = listIndex+1; // root is on index 0
	if ( currSelPathIndex > (int)selectionPath.size()) {
		// can always be one greater
		selectionPath.push_back(e.node);
	} else {
		while (currSelPathIndex < (int)selectionPath.size()) {
			typename Path::iterator it = selectionPath.end() - 1;
			selectionPath.erase(it);
		}
		selectionPath[currSelPathIndex] = e.node;
	}

	// if listIndex<numLists-1 remove all lists > listIndex

	// update
	updateLists(selectionPath);

	EventSender<Event>::notifyListeners (
		this, SelectionPathChanged<TM>(getPtr())
	);
}
//-----------------------------------------------------------------------------
template <class TM>
std::string AColumnBrowser<TM>::selectionPathToString() const {
	if (selectionPath.empty())
		return "{}";
	std::stringstream ss;
	ss<<Model::getNodeData(selectionPath[0]);
	for (int i=1; i<(int)selectionPath.size(); ++i) {
		ss<<"->"<<Model::getNodeData(selectionPath[i]);
	}
	return "{" + ss.str() + "}";
}
}}} // namespace(s)
#endif /* SAMBAG_ACOLUMNBROWSER_H */
