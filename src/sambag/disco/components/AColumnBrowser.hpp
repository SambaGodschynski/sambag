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
#include "Forward.hpp"

namespace sambag { namespace disco { namespace components {
namespace sce = sambag::com::events;
class SelectionPathChanged {
private:
	AContainerPtr src;
public:
	SelectionPathChanged(AContainerPtr src) : src(src) {}
	AContainerPtr getSource() const {
		return src;
	}
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
			setText("[" + sambag::com::toString(value.data) + "]");
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
template <
	class T, 
	template <class> class _ListCellRenderer,
	template <class> class _ListModel,
	class _ListSelectionModel
>
class ColumnBrowserList :
	public AList<  T,
		_ListCellRenderer,
		_ListModel,
		_ListSelectionModel
	>
{
public:
	//-------------------------------------------------------------------------
	typedef ColumnBrowserList<T, _ListCellRenderer, 
		_ListModel, _ListSelectionModel> Class;
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
  */
template <
	class TreeModel, 
	template <class> class _ListCellRenderer = ColumnBrowserListCellRenderer,
	template <class> class _ListModel = DefaultListModel,
	class _ListSelectionModel = DefaultListSelectionModel
>
class AColumnBrowser : public AContainer,
	public TreeModel,
	public sce::EventSender<SelectionPathChanged>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef TreeModel Model;
	//-------------------------------------------------------------------------
	typedef AColumnBrowser<Model, 
		_ListCellRenderer,
		_ListModel, 
		_ListSelectionModel> Class;
	//-------------------------------------------------------------------------
	typedef SelectionPathChanged Event;
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
	typedef ColumnBrowserList<Entry,
		_ListCellRenderer, _ListModel, _ListSelectionModel> ListType;
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
	//-------------------------------------------------------------------------
	Path oldPath;
private:
	//-------------------------------------------------------------------------
	void adjustListCount();
	//-------------------------------------------------------------------------
	void updateList(Node parent, int listIndex);
	//-------------------------------------------------------------------------
	Path selectionPath;
	//-------------------------------------------------------------------------
	void init();
	//-------------------------------------------------------------------------
	typename ColumnViewClass::Ptr columnView;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<int> currListIndex;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<int> numFixedLists;
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(Class)
	//-------------------------------------------------------------------------
	void updateLists();
	//-------------------------------------------------------------------------
	std::string selectionPathToString(const std::string & seperator = "/") const{
		return pathToString(getSelectionPath(), seperator);
	}
	//-------------------------------------------------------------------------
	std::string pathToString( const Path &path, 
		const std::string & seperator = "/" ) const;
	//-------------------------------------------------------------------------
	const Path & getSelectionPath() const {
		return selectionPath;
	}
	//-------------------------------------------------------------------------
	bool isFolder(Node node) const {
		return Model::getNumChildren(node) > 0;
	}
}; // AColumnBrowser
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class TM, 
	template <class> class CR,
	template <class> class LM,
	class LSM
>
int AColumnBrowser<TM, CR, LM, LSM>::getListIndex(const Node &node) const {
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
template <class TM, 
	template <class> class CR,
	template <class> class LM,
	class LSM
>
void AColumnBrowser<TM, CR, LM, LSM>::postConstructor() {
	Super::postConstructor();
	init();
}
//-----------------------------------------------------------------------------
template <class TM, 
	template <class> class CR,
	template <class> class LM,
	class LSM
>
void AColumnBrowser<TM, CR, LM, LSM>::addList(int listIndex)
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
template <class TM, 
	template <class> class CR,
	template <class> class LM,
	class LSM
>
void AColumnBrowser<TM, CR, LM, LSM>::init() {
	setLayout(BorderLayout::create());
	columnView = ColumnViewClass::create();
	selectionPath.reserve(10);
	selectionPath.push_back(Model::getRootNode());
	oldPath = selectionPath; 
	Model::setNodeData(Model::getRootNode(), "root");
	numFixedLists = 3;
	ui::getUIManager().getProperty("AColumnBrowser.numFixedLists", numFixedLists);
	for (int i=0; i<numFixedLists; ++i) {
		addList(i);
	}
	add(columnView);
}
//-----------------------------------------------------------------------------
template <class TM, 
	template <class> class CR,
	template <class> class LM,
	class LSM
>
void AColumnBrowser<TM, CR, LM, LSM>::adjustListCount() {
	int numLists = (int)columnView->getNumLists();
	int numNeededLists = (int)selectionPath.size();
	if (!isFolder(selectionPath.back())) {
		//last entry is no folder
		--numNeededLists;
	}
	if ( numNeededLists > numLists ) { // add
		for (int i=numLists; i<numNeededLists; ++i) {
			addList(i);
		}
		columnView->invalidate();
		columnView->validate();
	}
}
//-----------------------------------------------------------------------------
template <class TM, 
	template <class> class CR,
	template <class> class LM,
	class LSM
>
void AColumnBrowser<TM, CR, LM, LSM>::updateList(
	typename AColumnBrowser<TM, CR, LM, LSM>::Node parent, int listIndex)
{
	ListTypePtr list = columnView->getList(listIndex);
	if (!list)
		return;
	list->clear();
	int num = Model::getNumChildren(parent);
	if (num==0) {
		list->getParent()->invalidate();
		list->getParent()->validate();
		list->getParent()->redraw();
		return;
	}
	std::vector<Node> nodes;
	nodes.reserve(num);
	Model::getChildren(parent, nodes);
	for (int i=0; i<(int)nodes.size(); ++i) {
		const ModelDataType &data = Model::getNodeData(nodes[i]);
		list->addElement(Entry(this, data, nodes[i]));
	}
	list->getParent()->invalidate();
	list->getParent()->validate();
	list->getParent()->redraw();
}
//-----------------------------------------------------------------------------
template <class TM, 
	template <class> class CR,
	template <class> class LM,
	class LSM
>
void AColumnBrowser<TM, CR, LM, LSM>::updateLists()
{
	adjustListCount();
	if (oldPath.size() > selectionPath.size()) {
		for (int i = oldPath.size(); i>(int)selectionPath.size(); --i) {
			ListTypePtr list = columnView->getList(i-1);
			if (!list)
				continue;
			list->clear();
			list->redraw();
		}
	}
	// root
	for (int i=0; i<(int)selectionPath.size(); ++i) {
		updateList(selectionPath[i], i);
		/*if (i>=oldPath.size())
			continue;
		if (selectionPath[i]!=oldPath[i]) {
			ListTypePtr list = columnView->getList(i-1);
			if (!list)
				continue;
			list->clearSelection();
		}*/
	}
	if (selectionPath.size() > oldPath.size()) {
		// scroll to last list
		/*ScrollPane::Ptr scp = columnView->getScrollPane();
		ScrollPane::AScrollbarPtr scr = scp->getVerticalScrollBar();
		Point2D vp = scp->getViewport()->getViewPosition();
		std::cout<<scr->getMaximum()<<std::endl;
		std::cout<<scr->getExtent()<<std::endl;
		scp->getViewport()->setViewPosition(
			Point2D(
				scr->getMaximum(), 
				vp.y()
			)
		);*/
	}
	oldPath = selectionPath;
}
//-----------------------------------------------------------------------------
template <class TM, 
	template <class> class CR,
	template <class> class LM,
	class LSM
>
void AColumnBrowser<TM, CR, LM, LSM>::onSelectionChanged(void *src,
	const ListSelectionEvent &ev, int listIndex)
{
	ListTypePtr list = columnView->getList(listIndex);
	const Entry *e = list->getSelectedValue();
	if  (!e)
		return;
	int currSelPathIndex = listIndex+1; // root is on index 0
	if ( currSelPathIndex >= (int)selectionPath.size()) {
		// currSelPathIndex can always be one greater
		selectionPath.push_back(e->node);
	} else {
		while (currSelPathIndex < (int)selectionPath.size()-1) {
			typename Path::iterator it = selectionPath.end() - 1;
			selectionPath.erase(it);
		}
		selectionPath[currSelPathIndex] = e->node;
	}

	// if listIndex<numLists-1 remove all lists > listIndex

	// update
	updateLists();

	EventSender<Event>::notifyListeners (
		this, Event(getPtr())
	);
}
//-----------------------------------------------------------------------------
template <class TM, 
	template <class> class CR,
	template <class> class LM,
	class LSM
>
std::string 
AColumnBrowser<TM, CR, LM, LSM>::pathToString(
	const typename AColumnBrowser<TM, CR, LM, LSM>::Path &path,
	const std::string & seperator) const
{
	if (path.size()<=1) // only root inside
		return "";
	std::stringstream ss;
	ss<<Model::getNodeData(path[1]);
	for (int i=2; i<(int)path.size(); ++i) {
		ss<<seperator<<Model::getNodeData(path[i]);
	}
	return ss.str();
}
}}} // namespace(s)
#endif /* SAMBAG_ACOLUMNBROWSER_H */
