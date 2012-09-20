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
/** 
  * @class AColumnBrowser.
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
	typedef typename Model::NodeDataType ModelDataType;
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
	typedef boost::shared_ptr<AColumnBrowser> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr
	createComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	AColumnBrowser() {}
	//-------------------------------------------------------------------------
	virtual void postConstructor();
	//-------------------------------------------------------------------------
	void installListListener(ListTypePtr list, int listIndex);
	//-------------------------------------------------------------------------
	typedef typename ListType::ListSelectionModel::Event ListSelectionEvent;
	//-------------------------------------------------------------------------
	void onSelectionChanged(void *src,
		const ListSelectionEvent &ev, int listIndex);
private:
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<int> firstEmptyColumn;
	//-------------------------------------------------------------------------
	void init();
	//-------------------------------------------------------------------------
	typename ColumnViewClass::Ptr columnView;
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
	Ptr getPtr() const {
		return boost::shared_dynamic_cast<AColumnBrowser>(Super::getPtr());
	}
}; // AColumnBrowser
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class TM>
void AColumnBrowser<TM>::postConstructor() {
	Super::postConstructor();
	init();
}
//-----------------------------------------------------------------------------
template <class TM>
void AColumnBrowser<TM>::installListListener(ListTypePtr list, int listIndex)
{
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
	int numInitLists = 3;
	ui::getUIManager().getProperty("AColumnBrowser.numInitLists", numInitLists);
	for (int i=0; i<numInitLists; ++i) {
		ListTypePtr list = columnView->addList();
		installListListener(list, i);
	}
	add(columnView);
}
//-----------------------------------------------------------------------------
template <class TM>
void AColumnBrowser<TM>::onSelectionChanged(void *src,
	const ListSelectionEvent &ev, int listIndex)
{
	EventSender<SelectionPathChanged<TM> >::notifyListeners (
		this, SelectionPathChanged<TM>(getPtr())
	);
}
//-----------------------------------------------------------------------------
template <class TM>
template <class ContentContainer>
void AColumnBrowser<TM>::setCurrentPathContent(const ContentContainer &c) {
	if (currListIndex < 0 )
		return;
	ListTypePtr list = columnView->getList(currListIndex);
	list->ListType::ListModel::clear();
	BOOST_FOREACH(const ContentType &val, c) {
		list->addElement(val);
	}
}
//-----------------------------------------------------------------------------
template <class TM>
template <class ContentContainer>
void AColumnBrowser<TM>::getSelectionPath(ContentContainer &c) const {
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
template <class TM>
inline std::ostream &
operator<<(std::ostream &os,
		const typename sdc::AColumnBrowser<TM>::ContentType &val)
{
	os << val.name;
	//TODO: remove if statement below:
	if (val.type == sdc::AColumnBrowser<TM>::ContentType::Folder) {
		os<<"     >";
	}
	return os;
}
#endif /* SAMBAG_ACOLUMNBROWSER_H */
