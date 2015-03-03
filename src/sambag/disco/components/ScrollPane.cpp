/*
 * ScrollPane.cpp
 *
 *  Created on: Thu Jul  5 16:50:53 2012
 *      Author: Johannes Unger
 */

#include "ScrollPane.hpp"
#include "ui/ALookAndFeel.hpp"
#include "Viewport.hpp"
#include "ScrollPaneLayout.hpp"
#include "IScrollable.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class ScrollPane::MyScrollbar
//=============================================================================
class ScrollPane::MyScrollbar : public Scrollbar {
private:
	//-------------------------------------------------------------------------
	typedef Scrollbar Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ScrollPane::MyScrollbar> Ptr;
	//-------------------------------------------------------------------------
	ScrollPane &parent;
	//-------------------------------------------------------------------------
	MyScrollbar(ScrollPane &parent, Scrollbar::Orientation o) :
		Scrollbar(o), parent(parent) {}
	//-------------------------------------------------------------------------
	/**
	 * Set to true when the unit increment has been explicitly set.
	 * If this is false the viewport's view is obtained and if it
	 * is an instance of <code>Scrollable</code> the unit increment
	 * from it is used.
	 */
	sambag::com::ArithmeticWrapper<bool> unitIncrementSet;
	//-------------------------------------------------------------------------
	/**
	 * Set to true when the block increment has been explicitly set.
	 * If this is false the viewport's view is obtained and if it
	 * is an instance of <code>Scrollable</code> the block increment
	 * from it is used.
	 */
	sambag::com::ArithmeticWrapper<bool> blockIncrementSet;
public:
	//-------------------------------------------------------------------------
	Coordinate getUnitIncrement(Direction direction) const
	{
		Viewport::Ptr vp = parent.getViewport();
		IScrollable::Ptr sc =
				boost::dynamic_pointer_cast<IScrollable>(vp->getView());
		if (!unitIncrementSet && vp && sc) {
			Rectangle vr = vp->getViewRect();
			return sc->getScrollableUnitIncrement(vr, getOrientation(),
					direction);
		} else {
			return Super::getUnitIncrement(direction);
		}
	}
	//-------------------------------------------------------------------------
	void setUnitIncrement(const Coordinate &unitIncrement) {
		unitIncrementSet = true;
		Super::setUnitIncrement(unitIncrement);
	}
	//-------------------------------------------------------------------------
	Coordinate getBlockIncrement(Direction direction) const{
		Viewport::Ptr vp = parent.getViewport();
		if (blockIncrementSet || !vp) {
			return Super::getBlockIncrement(direction);
		}
		IScrollable::Ptr sc =
				boost::dynamic_pointer_cast<IScrollable>(vp->getView());
		if (sc) {
			Rectangle vr = vp->getViewRect();
			return sc->getScrollableBlockIncrement(vr, getOrientation(),
					direction);
		}
		if (getOrientation() == VERTICAL) {
			return vp->getExtentSize().height();
		} else {
			return vp->getExtentSize().width();
		}
	}
	//-------------------------------------------------------------------------
	void setBlockIncrement(const Coordinate &blockIncrement) {
		blockIncrementSet = true;
		Super::setBlockIncrement(blockIncrement);
	}
	//-------------------------------------------------------------------------
	static Ptr create(ScrollPane &parent, Scrollbar::Orientation o) {
		Ptr res(new MyScrollbar(parent, o));
		res->self = res;
		res->postConstructor();
		return res;
	}
};
//=============================================================================
//  Class ScrollPane
//=============================================================================
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_VERTICAL_SCROLLBAR_POLICY =
		"vertical scrollbar policy";
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_HORIZONTAL_SCROLLBAR_POLICY =
		"horizontal scrollbar policy";
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_VERTICAL_SCROLLBAR =
		"vertical scrollbar";
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_HORIZONTAL_SCROLLBAR =
		"horizontal scrollbar";
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_VIEWPORT = "viewport";
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_VIEWPORT_BORDER;
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_ROWHEADER;
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_COLUMNHEADER;
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_CORNER;
//-----------------------------------------------------------------------------
const std::string ScrollPane::PROPERTY_WHEELSCROLLING_ENABLED;
//-----------------------------------------------------------------------------
ScrollPane::ScrollPane() :
	verticalScrollBarPolicy(VERTICAL_SCROLLBAR_AS_NEEDED),
	horizontalScrollBarPolicy(HORIZONTAL_SCROLLBAR_AS_NEEDED)
{
}
//-----------------------------------------------------------------------------
void ScrollPane::postConstructor() {
	Super::postConstructor();
	setLayout(ScrollPaneLayout::create());
	setVerticalScrollBarPolicy(verticalScrollBarPolicy);
	setHorizontalScrollBarPolicy(horizontalScrollBarPolicy);
	setViewport(createViewport());
	setVerticalScrollBar(createVerticalScrollBar());
	setHorizontalScrollBar(createHorizontalScrollBar());
	setOpaque(true);
	setName("ScrollPane");
}
//-----------------------------------------------------------------------------
void ScrollPane::setLayout(ALayoutManagerPtr layout) {
	ScrollPaneLayout::Ptr _layout =
			boost::dynamic_pointer_cast<ScrollPaneLayout>(layout);
	if (_layout) {
		Super::setLayout(layout);
		_layout->syncWithScrollPane(getPtr());
		return;
	} else if (!layout) {
		Super::setLayout(layout);
		return;
	}
	SAMBAG_THROW(com::exceptions::IllegalArgumentException,
		"LayoutManager must be a ScrollPaneLayout");
}
//-----------------------------------------------------------------------------
ScrollbarPtr ScrollPane::createHorizontalScrollBar() {
	return MyScrollbar::create(*this, Scrollbar::HORIZONTAL);
}
//-----------------------------------------------------------------------------
ScrollbarPtr ScrollPane::createVerticalScrollBar() {
	return MyScrollbar::create(*this, Scrollbar::VERTICAL);
}
//-----------------------------------------------------------------------------
void ScrollPane::setHorizontalScrollBar(ScrollbarPtr scrollbar) {
	AScrollbarPtr old = getHorizontalScrollBar();
	horizontalScrollBar = scrollbar;
	if (horizontalScrollBar) {
		addBack(horizontalScrollBar, HORIZONTAL_SCROLLBAR);
	} else if (!old) {
		remove(old);
	}
	firePropertyChanged(PROPERTY_HORIZONTAL_SCROLLBAR, old,
		(AScrollbarPtr)horizontalScrollBar);

	revalidate();
	redraw();
}
//-----------------------------------------------------------------------------
void ScrollPane::setVerticalScrollBar(ScrollbarPtr scrollbar) {
	AScrollbarPtr old = getVerticalScrollBar();
	verticalScrollBar = scrollbar;
	addBack(verticalScrollBar, VERTICAL_SCROLLBAR);
	firePropertyChanged(PROPERTY_VERTICAL_SCROLLBAR, old,
			(AScrollbarPtr)verticalScrollBar);

	revalidate();
	redraw();
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPane::createViewport() {
	return Viewport::create();
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPane::getViewport() const {
	return viewport;
}
//-----------------------------------------------------------------------------
void ScrollPane::setViewport(ViewportPtr viewport) {
	ViewportPtr old = getViewport();
	this->viewport = viewport;
	if (viewport) {
		addBack(viewport, VIEWPORT);
	} else if (old) {
		remove(old);
	}
	firePropertyChanged(PROPERTY_VIEWPORT, old, viewport);
	revalidate();
	redraw();
}
//-----------------------------------------------------------------------------
ScrollPane::Ptr ScrollPane::create(AComponentPtr view,
		ScrollPane::VerticalScrollbarPolicy vsbPolicy,
		ScrollPane::HorizontalScrollbarPolicy hsbPolicy)
{
	Ptr res(new ScrollPane());
	res->self = res;
	res->postConstructor();
	if (view) {
		res->setViewportView(view);
	}
	return res;
}
//-----------------------------------------------------------------------------
ui::AComponentUIPtr
ScrollPane::createComponentUI(ui::ALookAndFeelPtr laf) const
{
	return laf->getUI<ScrollPane>();
}
//-----------------------------------------------------------------------------
bool ScrollPane::isValidateRoot() const {
	return true;
}
//-----------------------------------------------------------------------------
ScrollPane::VerticalScrollbarPolicy
ScrollPane::getVerticalScrollBarPolicy() const
{
	return verticalScrollBarPolicy;
}
//-----------------------------------------------------------------------------
void ScrollPane::setVerticalScrollBarPolicy(
		ScrollPane::VerticalScrollbarPolicy policy)
{
	VerticalScrollbarPolicy old = verticalScrollBarPolicy;
	verticalScrollBarPolicy = policy;
	firePropertyChanged(PROPERTY_VERTICAL_SCROLLBAR_POLICY, old, policy);
	revalidate();
	redraw();
}
//-----------------------------------------------------------------------------
ScrollPane::HorizontalScrollbarPolicy
ScrollPane::getHorizontalScrollBarPolicy() const
{
	return horizontalScrollBarPolicy;
}
//-----------------------------------------------------------------------------
void ScrollPane::setHorizontalScrollBarPolicy(
		ScrollPane::HorizontalScrollbarPolicy policy)
{
	HorizontalScrollbarPolicy old = horizontalScrollBarPolicy;
	horizontalScrollBarPolicy = policy;
	firePropertyChanged(PROPERTY_HORIZONTAL_SCROLLBAR_POLICY, old, policy);
	revalidate();
	redraw();
}
//-----------------------------------------------------------------------------
IBorder::Ptr ScrollPane::getViewportBorder() const {
	return viewportBorder;
}
//-----------------------------------------------------------------------------
void ScrollPane::setViewportBorder(IBorder::Ptr viewportBorder) {
	IBorder::Ptr oldValue = this->viewportBorder;
	this->viewportBorder = viewportBorder;
	firePropertyChanged(PROPERTY_VIEWPORT_BORDER, oldValue, viewportBorder);
}
//-----------------------------------------------------------------------------
Rectangle ScrollPane::getViewportBorderBounds() const {
	Rectangle borderR(getSize());

	Insets insets = getInsets();
	borderR.x(insets.left());
	borderR.y(insets.top());
	borderR.width( borderR.width() - insets.left() + insets.right());
	borderR.height( borderR.height() - insets.top() + insets.bottom());

	/* If there's a visible column header remove the space it
	 * needs from the top of borderR.
	 */
	Viewport::Ptr colHead = getColumnHeader();
	if (colHead && colHead->isVisible()) {
		Coordinate colHeadHeight = colHead->getHeight();
		borderR.y( borderR.y() + colHeadHeight);
		borderR.height( borderR.height() - colHeadHeight);
	}

	/* If there's a visible row header remove the space it needs
	 * from the left of borderR.
	 */
	Viewport::Ptr rowHead = getRowHeader();
	if (rowHead && rowHead->isVisible()) {
		Coordinate rowHeadWidth = rowHead->getWidth();
		borderR.x( borderR.x() + rowHeadWidth);
		borderR.width( borderR.width() - rowHeadWidth);
	}

	/* If there's a visible vertical scrollbar remove the space it needs
	 * from the width of borderR.
	 */
	AScrollbarPtr vsb = getVerticalScrollBar();
	if (vsb && vsb->isVisible()) {
		Coordinate vsbWidth = vsb->getWidth();
		borderR.width( borderR.width() - vsbWidth);
	}

	/* If there's a visible horizontal scrollbar remove the space it needs
	 * from the height of borderR.
	 */
	AScrollbarPtr hsb = getHorizontalScrollBar();
	if (hsb && hsb->isVisible()) {
		borderR.height( borderR.height() - hsb->getHeight());
	}

	return borderR;
}
//-----------------------------------------------------------------------------
void ScrollPane::setViewportView(AComponentPtr view) {
	if (!getViewport()) {
		setViewport( createViewport());
	}
	getViewport()->setView(view);
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPane::getRowHeader() const {
	return rowHeader;
}
//-----------------------------------------------------------------------------
void ScrollPane::setRowHeader(ViewportPtr rowHeader) {
	Viewport::Ptr old = getRowHeader();
	this->rowHeader = rowHeader;
	if (rowHeader) {
		addBack(rowHeader, ROW_HEADER);
	} else if (old) {
		remove(old);
	}
	firePropertyChanged(PROPERTY_ROWHEADER, old, rowHeader);
	revalidate();
	redraw();
}
//-----------------------------------------------------------------------------
void ScrollPane::setRowHeaderView(AComponentPtr view) {
	if (!getRowHeader()) {
		setRowHeader(createViewport());
	}
	getRowHeader()->setView(view);
}
//-----------------------------------------------------------------------------
ViewportPtr ScrollPane::getColumnHeader() const {
	return columnHeader;
}
//-----------------------------------------------------------------------------
void ScrollPane::setColumnHeader(ViewportPtr columnHeader) {
	ViewportPtr old = getColumnHeader();
	this->columnHeader = columnHeader;
	if (columnHeader) {
		addBack(columnHeader, COLUMN_HEADER);
	} else if (old) {
		remove(old);
	}
	firePropertyChanged(PROPERTY_COLUMNHEADER, old, columnHeader);
	revalidate();
	redraw();
}
//-----------------------------------------------------------------------------
void ScrollPane::setColumnHeaderView(AComponentPtr view) {
	if (!getColumnHeader()) {
		setColumnHeader(createViewport());
	}
	getColumnHeader()->setView(view);
}
//-----------------------------------------------------------------------------
AComponentPtr ScrollPane::getCorner(Area loc) {
	switch (loc) {
	case LOWER_LEADING_CORNER:
	case LOWER_LEFT_CORNER:
		return lowerLeft;
	case LOWER_TRAILING_CORNER:
	case LOWER_RIGHT_CORNER:
		return lowerRight;
	case UPPER_LEADING_CORNER:
	case UPPER_LEFT_CORNER:
		return upperLeft;
	case UPPER_TRAILING_CORNER:
	case UPPER_RIGHT_CORNER:
		return upperRight;
	default:
		SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
			"bad location.");
	}
	return AComponentPtr();
}
//-----------------------------------------------------------------------------
void ScrollPane::setCorner(ScrollPane::Area loc, AComponentPtr corner) {
	AComponentPtr old;
	switch (loc) {
	case LOWER_LEADING_CORNER:
	case LOWER_LEFT_CORNER:
		old = lowerLeft;
		lowerLeft = corner;
		break;
	case LOWER_TRAILING_CORNER:
	case LOWER_RIGHT_CORNER:
		old = lowerLeft;
		lowerRight = corner;
		break;
	case UPPER_LEADING_CORNER:
	case UPPER_LEFT_CORNER:
		old = lowerLeft;
		upperLeft = corner;
		break;
	case UPPER_TRAILING_CORNER:
	case UPPER_RIGHT_CORNER:
		old = lowerLeft;
		upperRight = corner;
		break;
	default:
		SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
			"bad location.");
	}
	if (old) {
		remove(old);
	}
	if (corner) {
		addBack(corner, loc);
	}
	firePropertyChanged(PROPERTY_CORNER, old, corner);
	revalidate();
	redraw();
}
//-----------------------------------------------------------------------------
std::string ScrollPane::paramString() const {
	SAMBA_LOG_NOT_YET_IMPL();
	return "";
}
//-----------------------------------------------------------------------------
void ScrollPane::setBounds(const Rectangle &r) {
	Super::setBounds(r);
	//validate();
}
//-----------------------------------------------------------------------------
void ScrollPane::resetViewPort() {
//	if (viewPort) {
//		viewPort.removeChangeListener(this);
//		viewPort.removePropertyChangeListener(this);
//	}
//	viewPort = JScrollPane.this.getViewport();
//	if (viewPort) {
//		viewPort.addChangeListener(this);
//		viewPort.addPropertyChangeListener(this);
//	}
}

}}} // ScrollPane::namespace(s)
