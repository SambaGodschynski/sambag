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
#include <vector>
namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class ColumnView.
  * aka Miller columns.
  */
class ColumnView : public AContainer {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef List<std::string> ListType;
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ColumnView> Ptr;
	//-------------------------------------------------------------------------
	typedef std::vector<ListType> Lists;
	//-------------------------------------------------------------------------
	ui::AComponentUIPtr createComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	ColumnView();
	//-------------------------------------------------------------------------
	virtual void postConstructor();
private:
	//-------------------------------------------------------------------------
	ScrollPane::Ptr listScrollPane;
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
	//-------------------------------------------------------------------------
	/**
	 * appends a new list to trail.
	 */
	void addList();
	//-------------------------------------------------------------------------
	/**
	 * remove last added list
	 */
	void removeList();
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(ColumnView)
}; // ColumnView
}}} // namespace(s)

#endif /* SAMBAG_COLUMNVIEW_H */
