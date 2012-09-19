/*
 * ColumnBrowser.hpp
 *
 *  Created on: Wed Sep 19 11:20:11 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_COLUMNBROWSER_H
#define SAMBAG_COLUMNBROWSER_H

#include <boost/shared_ptr.hpp>
#include "ColumnView.hpp"
#include <sambag/com/ArithmeticWrapper.hpp>
#include <boost/foreach.hpp>
#include "Forward.hpp"
#include <sambag/com/events/Events.hpp>

namespace sambag { namespace disco { namespace components {
namespace sce = sambag::com::events;
struct ColumnBrowserSelectionPathChanged {
	ColumnBrowserPtr src;
	ColumnBrowserSelectionPathChanged(ColumnBrowserPtr src) : src(src) {}
};
//=============================================================================
/** 
  * @class ColumnBrowser.
  */
class ColumnBrowser : public AContainer,
	public sce::EventSender<ColumnBrowserSelectionPathChanged>
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	struct ContentType {
		enum Type { Unknown, Folder, File };
		Type type;
		std::string name;
		ContentType(const std::string &name="", Type type=Unknown) :
			type(type), name(name)
		{
		}
		//---------------------------------------------------------------------
		bool operator==(const ContentType &b) const {
			return type == b.type && name == b.name;
		}
		//---------------------------------------------------------------------
		bool operator!=(const ContentType &b) const {
			return !(*this==b);
		}
	};
	//-------------------------------------------------------------------------
	typedef std::vector<ContentType> FolderContent;
	//-------------------------------------------------------------------------
	typedef List<ContentType> ListType;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<List<ContentType> > ListTypePtr;
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef ColumnView<ListType> ColumnViewClass;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ColumnBrowser> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr
	createComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	ColumnBrowser();
	//-------------------------------------------------------------------------
	virtual void postConstructor();
	//-------------------------------------------------------------------------
	void installListListener(ListTypePtr list, int listIndex);
	//-------------------------------------------------------------------------
	typedef ListType::ListSelectionModel::Event ListSelectionEvent;
	//-------------------------------------------------------------------------
	void onSelectionChanged(void *src,
		const ListSelectionEvent &ev, int listIndex);
private:
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<int> firstEmptyColumn;
	//-------------------------------------------------------------------------
	void init();
	//-------------------------------------------------------------------------
	ColumnViewClass::Ptr columnView;
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<int> currListIndex;
public:
	//-------------------------------------------------------------------------
	template <class ContentContainer>
	void getSelectionPath(ContentContainer &c) const;
	//-------------------------------------------------------------------------
	template <class ContentContainer>
	void setCurrentPathContent(const ContentContainer &c);
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(ColumnBrowser)
}; // ColumnBrowser
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class ContentContainer>
void ColumnBrowser::setCurrentPathContent(const ContentContainer &c) {
	if (currListIndex < 0 )
		return;
	ListTypePtr list = columnView->getList(currListIndex);
	list->ListType::ListModel::clear();
	BOOST_FOREACH(const ContentType &val, c) {
		list->addElement(val);
	}
}
//-----------------------------------------------------------------------------
template <class ContentContainer>
void ColumnBrowser::getSelectionPath(ContentContainer &c) const {
	for (int i=0; i<(int)columnView->getNumLists(); ++i) {
		ListTypePtr list = columnView->getList(i);
		int index = list->getSelectedIndex();
		if (index < 0)
			return;
		c.push_back(list->ListType::ListModel::get(index));
	}
}
}}} // namespace(s)
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
namespace sdc = sambag::disco::components;
inline std::ostream &
operator<<(std::ostream &os, const sdc::ColumnBrowser::ContentType &val) {
	os << val.name;
	//TODO: remove if statement below:
	if (val.type == sdc::ColumnBrowser::ContentType::Folder) {
		os<<"     >";
	}
	return os;
}
#endif /* SAMBAG_COLUMNBROWSER_H */
