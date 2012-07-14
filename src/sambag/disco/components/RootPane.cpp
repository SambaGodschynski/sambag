/*
 * RootPane.cpp
 *
 *  Created on: Wed May 30 14:16:30 2012
 *      Author: Johannes Unger
 */

#include "RootPane.hpp"
#include <sambag/disco/IDiscoFactory.hpp>
#include "FlowLayout.hpp"
#include "ui/UIManager.hpp"
#include "ui/ALookAndFeel.hpp"
#include "ui/basic/BasicLookAndFeel.hpp"
#include <sambag/disco/components/Window.hpp>
#include <sambag/com/exceptions/IllegalArgumentException.hpp>
#include <sambag/disco/svg/HtmlColors.hpp>
#include "BorderLayout.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class RootPane::RootLayout
//=============================================================================
class RootPane::RootLayout : public ALayoutManager {
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<RootLayout> Ptr;
	//-------------------------------------------------------------------------
private:
	//-------------------------------------------------------------------------
	RootPane &parentRoot;
	//-------------------------------------------------------------------------
	RootLayout(RootPane &parent) : parentRoot(parent) {}
public:
	//-------------------------------------------------------------------------
	static Ptr create(RootPane &parent) {
		Ptr res(new RootLayout(parent));
		return res;
	}
	//-------------------------------------------------------------------------
	/**
	 * Lays out the specified container.
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param parent
	 */
	virtual void layoutContainer(AContainerPtr parent) {
		Rectangle b = parent->getBounds();
		Insets i = parentRoot.getInsets();
		Coordinate contentY = 0;
		Coordinate w = b.width() - i.right() - i.left();
		Coordinate h = b.height() - i.top() - i.bottom();

//		if (layeredPane != null) {
//			layeredPane.setBounds(i.left, i.top, w, h);
//		}
//		if (glassPane != null) {
//			glassPane.setBounds(i.left, i.top, w, h);
//		}
		// Note: This is laying out the children in the layeredPane,
		// technically, these are not our children.
//		if (menuBar != null && menuBar.isVisible()) {
//			Dimension mbd = menuBar.getPreferredSize();
//			menuBar.setBounds(0, 0, w, mbd.height);
//			contentY += mbd.height;
//		}
		if (parentRoot.contentPane) {
			parentRoot.contentPane->setBounds(0, contentY, w, h - contentY);
		}
	}
	//-------------------------------------------------------------------------
	/**
	 * Calculates the minimum size dimensions for the specified container,
	 * given the components it contains.
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param parent
	 * @return
	 */
	virtual Dimension minimumLayoutSize(AContainerPtr parent) {
		Dimension rd, mbd;
		Insets i = parentRoot.getInsets();
		if (parentRoot.contentPane) {
			rd = parentRoot.contentPane->getMinimumSize();
		} else {
			rd = parent->getSize();
		}
//		if (menuBar != null && menuBar.isVisible()) {
//			mbd = menuBar.getMinimumSize();
//		} else {
//			mbd = new Dimension(0, 0);
//		}
		return Dimension(std::max(rd.width(), mbd.width()) + i.left() + i.right(),
				rd.height() + mbd.height() + i.top() + i.bottom());
	}
	//-------------------------------------------------------------------------
	/**
	 * Calculates the maximum size dimensions for the specified container,
	 * given the components it contains.
	 * @param parent
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @return
	 */
	virtual Dimension maximumLayoutSize(AContainerPtr parent) {
		Dimension rd, mbd;
		Insets i = parentRoot.getInsets();
//		if (menuBar != null && menuBar.isVisible()) {
//			mbd = menuBar.getMaximumSize();
//		} else {
//			mbd = new Dimension(0, 0);
//		}
		if (parentRoot.contentPane) {
			rd = parentRoot.contentPane->getMaximumSize();
		} else {
			rd = parent->getSize();
		}
		return Dimension(std::min(rd.width(), mbd.width()) + i.left() + i.right(),
				rd.height() + mbd.height() + i.top() + i.bottom());
	}
	//-------------------------------------------------------------------------
	/**
	 * Calculates the preferred size dimensions for the specified container,
	 * given the components it contains.
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param parent
	 * @return
	 */
	virtual Dimension preferredLayoutSize(AContainerPtr parent) {
		Dimension rd, mbd;
		Insets i = parentRoot.getInsets();

		if (parentRoot.contentPane) {
			rd = parentRoot.contentPane->getPreferredSize();
		} else {
			rd = parent->getSize();
		}
//		if (menuBar != null && menuBar.isVisible()) {
//			mbd = menuBar.getPreferredSize();
//		} else {
//			mbd = new Dimension(0, 0);
//		}
		return Dimension(std::max(rd.width(), mbd.width()) + i.left() + i.right(),
				rd.height() + mbd.height() + i.top() + i.bottom());
	}
	//-------------------------------------------------------------------------
	/**
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param target
	 * @return
	 */
	virtual Coordinate getLayoutAlignmentY(AContainerPtr target) {
		return 0.;
	}
	//-------------------------------------------------------------------------
	/**
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param target
	 * @return
	 */
	virtual Coordinate getLayoutAlignmentX(AContainerPtr target) {
		return 0.;
	}
};
//=============================================================================
//  Class RootPane
//=============================================================================
//-----------------------------------------------------------------------------
const std::string RootPane::PROPERTY_SURFACE = "surface";
//-----------------------------------------------------------------------------
RootPane::RootPane(ISurface::Ptr surface) : surface(surface) {
	setName("RootPane");
}
//-----------------------------------------------------------------------------
Panel::Ptr RootPane::createContentPane() {
	Panel::Ptr contentPane = Panel::create();
	contentPane->setName(getName() + " conetent pane");
	contentPane->setLayout(BorderLayout::create());
	return contentPane;
}
//-----------------------------------------------------------------------------
Panel::Ptr RootPane::getContentPane() const {
	return contentPane;
}
//-----------------------------------------------------------------------------
void RootPane::setContentPane(Panel::Ptr content) {
	if (!content)
		SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
				"contentPane cannot be set to null.");
	contentPane = content;
	add(contentPane);
}
//-----------------------------------------------------------------------------
void RootPane::constructorAlt() {
	setBackground(svg::HtmlColors::getColor("AliceBlue"));
	setForeground(ColorRGBA(0,0,0,1));
	setLayout(RootLayout::create(*this));
	setContentPane(createContentPane());
}
//-----------------------------------------------------------------------------
ui::ALookAndFeelPtr RootPane::getCurrentLookAndFeel() const {
	return ui::UIManager::instance().getLookAndFeel(getPtr());
}
//-----------------------------------------------------------------------------
void RootPane::setSurface(ISurface::Ptr _surface) {
	ISurface::Ptr old = _surface;
	surface = _surface;
	firePropertyChanged(PROPERTY_SURFACE, old, _surface);
}
//-----------------------------------------------------------------------------
IDrawContext::Ptr RootPane::getDrawContext() const {
	if (!surface) {
		getDiscoFactory()->createRecordingSurface();
	}
	return getDiscoFactory()->createContext(surface);
}
//-----------------------------------------------------------------------------
RootPane::Ptr RootPane::create(ISurface::Ptr surface)
{
	Ptr res(new RootPane(surface));
	res->self = res;
	ui::UIManager::instance().installLookAndFeel(res,
		ui::basic::BasicLookAndFeel::create()
	);
	res->constructorAlt();
	return res;
}
//-----------------------------------------------------------------------------
bool RootPane::isShowing() const {
	return true;
}
//-----------------------------------------------------------------------------
void RootPane::processMouseEvent(const events::MouseEvent &ev) {
	using namespace events;
	AContainer::processMouseEvent(ev);
}
//-----------------------------------------------------------------------------
bool RootPane::isValidateRoot() const {
	return true;
}
//-----------------------------------------------------------------------------
bool RootPane::isDisplayable() const {
	return true;
}
//-----------------------------------------------------------------------------
void RootPane::addTag(AComponent::Ptr comp, const std::string &tag) {
	tagMap.insert(std::make_pair(tag, comp));
}
}}} // namespace(s)
