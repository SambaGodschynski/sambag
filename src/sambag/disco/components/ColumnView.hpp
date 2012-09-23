/*
 * ColumnView.hpp
 *
 *  Created on: Mon Sep 17 16:38:46 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_COLUMNVIEW_H
#define SAMBAG_COLUMNVIEW_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"
#include "Panel.hpp"
#include "ScrollPane.hpp"
#include "List.hpp"
#include "Forward.hpp"
#include "GridLayout.hpp"
#include "ui/ALookAndFeel.hpp"
#include "ui/UIManager.hpp"
#include "Panel.hpp"
#include "BorderLayout.hpp"
#include <vector>

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class ColumnView.
  * Container for lists similar to miller columns.
  */
template <class _ListType>
class ColumnView : public AContainer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef _ListType ListType;
	//-------------------------------------------------------------------------
	typedef ColumnView<ListType> Class;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ListType> ListTypePtr;
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ColumnView> Ptr;
	//-------------------------------------------------------------------------
	typedef std::vector<ListTypePtr> Lists;
	//-------------------------------------------------------------------------
	ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	ColumnView();
	//-------------------------------------------------------------------------
	virtual void postConstructor();
private:
	//-------------------------------------------------------------------------
	void adjustLayout();
	//-------------------------------------------------------------------------
	ScrollPane::Ptr listScrollPane;
	//-------------------------------------------------------------------------
	GridLayout::Ptr listPaneLayout;
	//-------------------------------------------------------------------------
	AContainer::Ptr listPane;
	//-------------------------------------------------------------------------
	AContainer::Ptr createList();
	//-------------------------------------------------------------------------
	void initMainPane();
	//-------------------------------------------------------------------------
	AContainer::Ptr createListPane();
	//-------------------------------------------------------------------------
	ScrollPane::Ptr createListScrollPane(AContainer::Ptr viewPortView);
	//-------------------------------------------------------------------------
	void initView();
	//-------------------------------------------------------------------------
	ScrollPane::Ptr listView;
	//-------------------------------------------------------------------------
	Lists lists;
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(Class)
	//-------------------------------------------------------------------------
	ListTypePtr getList(size_t index) const {
		return lists.at(index);
	}
	//-------------------------------------------------------------------------
	size_t getNumLists() const {
		return lists.size();
	}
	//-------------------------------------------------------------------------
	/**
	 * appends a new list to trail.
	 * @param insert on index or -1 for appending.
	 */
	ListTypePtr addList();
	//-------------------------------------------------------------------------
	/**
	 * remove last added list
	 */
	void removeList();
}; // ColumnView
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class LT>
ColumnView<LT>::ColumnView() {
	setName("ColumnView");
}
//-----------------------------------------------------------------------------
template <class LT>
ui::AComponentUIPtr
ColumnView<LT>::createComponentUI(ui::ALookAndFeelPtr laf) const
{
	return laf->getUI<Class>();
}
//-----------------------------------------------------------------------------
template <class LT>
void ColumnView<LT>::postConstructor() {
	Super::postConstructor();
	initView();
}
//-----------------------------------------------------------------------------
namespace {
	SAMBAG_PROPERTY_TAG(FixedColumWitdh, "ColumnView.fixedColumnWidth");
}
template <class LT>
AContainer::Ptr ColumnView<LT>::createList() {
	ListTypePtr list = ListType::create();
	ScrollPane::Ptr pane = ScrollPane::create(list);
	Coordinate width =
			ui::getUIPropertyCached<FixedColumWitdh>((Coordinate)120.);
	list->setFixedCellWidth(width);
	list->setFont(getFont());

	lists.push_back(list);

	pane->setVerticalScrollBarPolicy(ScrollPane::VERTICAL_SCROLLBAR_ALWAYS);
	return pane;
}
//-----------------------------------------------------------------------------
template <class LT>
void ColumnView<LT>::initMainPane() {
	getPtr()->setLayout(BorderLayout::create());
}
//-----------------------------------------------------------------------------
template <class LT>
AContainer::Ptr ColumnView<LT>::createListPane() {
	Panel::Ptr panel = Panel::create();
	listPaneLayout = GridLayout::create(0, 1);
	panel->setLayout(listPaneLayout);
	return panel;
}
//-----------------------------------------------------------------------------
template <class LT>
ScrollPane::Ptr ColumnView<LT>::createListScrollPane(AContainer::Ptr view)
{
	ScrollPane::Ptr pane = ScrollPane::create(view);
	return pane;
}
//-----------------------------------------------------------------------------
template <class LT>
void ColumnView<LT>::adjustLayout() {
	if ((int)lists.size() != listPaneLayout->getColumns()) {
		listPaneLayout->setColumns(lists.size());
	}
}
//-----------------------------------------------------------------------------
template <class LT>
typename ColumnView<LT>::ListTypePtr ColumnView<LT>::addList() {
	listPane->add(createList());
	adjustLayout();
	validate();
	return getList(lists.size() - 1);
}
//-----------------------------------------------------------------------------
template <class LT>
void ColumnView<LT>::removeList() {
	if (lists.empty())
		return;
	ListTypePtr list = getList(lists.size()-1);
	listPane->remove(list->getParent());
	typename Lists::iterator it = lists.begin() + (lists.size()-1);
	lists.erase(it);
	adjustLayout();
	validate();
}
//-----------------------------------------------------------------------------
template <class LT>
void ColumnView<LT>::initView() {
	initMainPane();
	listPane = createListPane();
	listScrollPane = createListScrollPane(listPane);
	getPtr()->add(listScrollPane);
}
}}} // namespace(s)

#endif /* SAMBAG_COLUMNVIEW_H */
