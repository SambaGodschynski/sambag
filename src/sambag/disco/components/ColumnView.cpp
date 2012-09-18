/*
 * ColumnView.cpp
 *
 *  Created on: Mon Sep 17 16:38:46 2012
 *      Author: Johannes Unger
 */

#include "ColumnView.hpp"
#include "ui/ALookAndFeel.hpp"
#include "ui/UIManager.hpp"
#include "Panel.hpp"
#include "BorderLayout.hpp"
#include "GridLayout.hpp"
namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class ColumnView
//=============================================================================
//-----------------------------------------------------------------------------
ColumnView::ColumnView() {
	setName("ColumnView");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr ColumnView::createComponentUI(ui::ALookAndFeelPtr laf) const
{
	return laf->getUI<ColumnView>();
}
//-----------------------------------------------------------------------------
void ColumnView::postConstructor() {
	initView();
}
//-----------------------------------------------------------------------------
namespace {
	SAMBAG_PROPERTY_TAG(FixedColumWitdh, "ColumnView.fixedColumnWidth");
}
AContainer::Ptr ColumnView::createList() {
	ListType::Ptr list = ListType::create();
	ScrollPane::Ptr pane = ScrollPane::create(list);
	Coordinate width =
			ui::getUIPropertyCached<FixedColumWitdh>((Coordinate)120.);
	static int c = 0;
	c++;
	for (int i=0; i<c*10; ++i) {
		list->addElement("ergo " + com::toString(i));
	}
	pane->setVerticalScrollBarPolicy(ScrollPane::VERTICAL_SCROLLBAR_ALWAYS);
	return pane;
}
//-----------------------------------------------------------------------------
void ColumnView::initMainPane() {
	getPtr()->setLayout(BorderLayout::create());
}
//-----------------------------------------------------------------------------
AContainer::Ptr ColumnView::createListPane() {
	Panel::Ptr panel = Panel::create();
	panel->setLayout(GridLayout::create(0, 3));
	return panel;
}
//-----------------------------------------------------------------------------
ScrollPane::Ptr ColumnView::createListScrollPane(AContainer::Ptr view) {
	ScrollPane::Ptr pane = ScrollPane::create(view);
	return pane;
}
//-----------------------------------------------------------------------------
void ColumnView::addList() {
	listPane->add(createList());
	validate();
}
//-----------------------------------------------------------------------------
void ColumnView::removeList() {

}
//-----------------------------------------------------------------------------
void ColumnView::initView() {
	initMainPane();
	listPane = createListPane();
	listScrollPane = createListScrollPane(listPane);
	int numInitLists = 3;
	ui::getUIManager().getProperty("ColumnView.numInitLists", numInitLists);
	for (int i=0; i<numInitLists; ++i) {
		addList();
	}
	getPtr()->add(listScrollPane);
}
}}} // namespace(s)
