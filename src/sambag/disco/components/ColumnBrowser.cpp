/*
 * ColumnBrowser.cpp
 *
 *  Created on: Wed Sep 19 11:20:11 2012
 *      Author: Johannes Unger
 */

#include "ColumnBrowser.hpp"
#include "ui/ALookAndFeel.hpp"
#include "BorderLayout.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class ColumnBrowser
//=============================================================================
//-----------------------------------------------------------------------------
ColumnBrowser::ColumnBrowser() {
	setName("ColumnBrowser");
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr
ColumnBrowser::createComponentUI(ui::ALookAndFeelPtr laf) const {
	return laf->getUI<ColumnBrowser>();
}
//-----------------------------------------------------------------------------
void ColumnBrowser::postConstructor() {
	Super::postConstructor();
	init();
}
//-----------------------------------------------------------------------------
void ColumnBrowser::installListListener(ListTypePtr list, int listIndex)
{
	list->sce::EventSender<ListSelectionEvent>::addTrackedEventListener(
		boost::bind(&ColumnBrowser::onSelectionChanged,this,_1,_2,listIndex),
		getPtr()
	);
}
//-----------------------------------------------------------------------------
void ColumnBrowser::init() {
	setLayout(BorderLayout::create());
	columnView = ColumnViewClass::create();
	int numInitLists = 3;
	ui::getUIManager().getProperty("ColumnBrowser.numInitLists", numInitLists);
	for (int i=0; i<numInitLists; ++i) {
		ListTypePtr list = columnView->addList();
		installListListener(list, i);
	}
	add(columnView);
}
//-----------------------------------------------------------------------------
void ColumnBrowser::onSelectionChanged(void *src,
	const ListSelectionEvent &ev, int listIndex)
{
	EventSender<ColumnBrowserSelectionPathChanged>::notifyListeners (
		this, ColumnBrowserSelectionPathChanged(getPtr())
	);
}
}}} // namespace(s)
