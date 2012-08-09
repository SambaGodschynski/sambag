/*
 * BorderLayout.cpp
 *
 *  Created on: Sat Jul 14 11:54:44 2012
 *      Author: Johannes Unger
 */

#include "BorderLayout.hpp"
#include "AComponent.hpp"
#include <sambag/com/exceptions/IllegalArgumentException.hpp>
#include "AContainer.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class BorderLayout
//=============================================================================
//-----------------------------------------------------------------------------
BorderLayout::BorderLayout(const Coordinate &hgap, const Coordinate &vgap)
: hgap(hgap), vgap(vgap)
{

}
//-----------------------------------------------------------------------------
void BorderLayout::addLayoutComponent(const AComponentSharedOrWeak &comp,
		ArbitraryType::Ptr _constraint)
{
	SAMBAG_BEGIN_SYNCHRONIZED (comp->getTreeLock())
		Constraint constraint = CENTER; // default location
		com::get(_constraint, constraint);
		switch (constraint) {
		case CENTER:
			center = comp;
			break;
		case NORTH:
			north = comp;
			break;
		case SOUTH:
			south = comp;
			break;
		case EAST:
			east = comp;
			break;
		case WEST:
			west = comp;
			break;
		case BEFORE_FIRST_LINE:
			firstLine = comp;
			break;
		case AFTER_LAST_LINE:
			lastLine = comp;
			break;
		case BEFORE_LINE_BEGINS:
			firstItem = comp;
			break;
		case AFTER_LINE_ENDS:
			lastItem = comp;
			break;
		default:
			SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
					"cannot add to layout: unknown constraint.");
		}
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void BorderLayout::layoutContainer(AContainerPtr parent) {
	SAMBAG_BEGIN_SYNCHRONIZED (parent->getTreeLock())
		Insets insets = parent->getInsets();
		Coordinate top = insets.top();
		Coordinate bottom = parent->getBounds().height() - insets.bottom();
		Coordinate left = insets.left();
		Coordinate right = parent->getBounds().width() - insets.right();
		Dimension dim;
		AComponent::Ptr c;
		if ((c = getChild(NORTH))) {
			c->setSize(right - left, c->getBounds().height());
			Dimension d = c->getPreferredSize();
			c->setBounds(left, top, right - left, d.height());
			top += d.height() + vgap;
		}
		if ((c = getChild(SOUTH))) {
			c->setSize(right - left, c->getBounds().height());
			Dimension d = c->getPreferredSize();
			c->setBounds(left, bottom - d.height(), right - left, d.height());
			bottom -= d.height() + vgap;
		}
		if ((c = getChild(EAST))) {
			c->setSize(c->getBounds().width(), bottom - top);
			Dimension d = c->getPreferredSize();
			c->setBounds(right - d.width(), top, d.width(), bottom - top);
			right -= d.width() + hgap;
		}
		if ((c = getChild(WEST))) {
			c->setSize(c->getBounds().width(), bottom - top);
			Dimension d = c->getPreferredSize();
			c->setBounds(left, top, d.width(), bottom - top);
			left += d.width() + hgap;
		}
		if ((c = getChild(CENTER))) {
			c->setBounds(left, top, right - left, bottom - top);
		}
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
Dimension BorderLayout::minimumLayoutSize(AContainerPtr parent) {
	SAMBAG_BEGIN_SYNCHRONIZED (parent->getTreeLock())
		Dimension dim;
		AComponent::Ptr c;
		if ((c = getChild(EAST))) {
			Dimension d = c->getMinimumSize();
			dim.width(dim.width() + d.width() + hgap);
			dim.height(std::max(d.height(), dim.height()));
		}
		if ((c = getChild(WEST))) {
			Dimension d = c->getMinimumSize();
			dim.width(dim.width() + d.width() + hgap);
			dim.height(std::max(d.height(), dim.height()));
		}
		if ((c = getChild(CENTER))) {
			Dimension d = c->getMinimumSize();
			dim.width(dim.width() + d.width());
			dim.height(std::max(d.height(), dim.height()));
		}
		if ((c = getChild(NORTH))) {
			Dimension d = c->getMinimumSize();
			dim.width(std::max(d.width(), dim.width()));
			dim.height(dim.height() + d.height() + vgap);
		}
		if ((c = getChild(SOUTH))) {
			Dimension d = c->getMinimumSize();
			dim.width(std::max(d.width(), dim.width()));
			dim.height(dim.height() + d.height() + vgap);
		}

		Insets insets = parent->getInsets();
		dim.width(dim.width() + insets.left() + insets.right());
		dim.height(dim.height() + insets.top() + insets.bottom());
		return dim;

	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
Dimension BorderLayout::preferredLayoutSize(AContainerPtr parent) {
	SAMBAG_BEGIN_SYNCHRONIZED (parent->getTreeLock())
		Dimension dim;
		AComponent::Ptr c;
		if ((c = getChild(EAST))) {
			Dimension d = c->getPreferredSize();
			dim.width(dim.width() + d.width() + hgap);
			dim.height(std::max(d.height(), dim.height()));
		}
		if ((c = getChild(WEST))) {
			Dimension d = c->getPreferredSize();
			dim.width(dim.width() + d.width() + hgap);
			dim.height(std::max(d.height(), dim.height()));
		}
		if ((c = getChild(CENTER))) {
			Dimension d = c->getPreferredSize();
			dim.width(dim.width() + d.width());
			dim.height(std::max(d.height(), dim.height()));
		}
		if ((c = getChild(NORTH))) {
			Dimension d = c->getPreferredSize();
			dim.width(std::max(d.width(), dim.width()));
			dim.height(dim.height() + d.height() + vgap);
		}
		if ((c = getChild(SOUTH))) {
			Dimension d = c->getPreferredSize();
			dim.width(std::max(d.width(), dim.width()));
			dim.height(dim.height() + d.height() + vgap);
		}

		Insets insets = parent->getInsets();
		dim.width(dim.width() + insets.left() + insets.right());
		dim.height(dim.height() + insets.top() + insets.bottom());
		return dim;

	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
void BorderLayout::removeLayoutComponent(AComponentPtr comp) {
	SAMBAG_BEGIN_SYNCHRONIZED (comp->getTreeLock())
		if (center == comp) {
			center = AComponent::Ptr();
		} else if (north == comp) {
			north = AComponent::Ptr();
		} else if (south == comp) {
			south = AComponent::Ptr();
		} else if (east == comp) {
			east = AComponent::Ptr();
		} else if (west == comp) {
			west = AComponent::Ptr();
		}
		if (firstLine == comp) {
			firstLine = AComponent::Ptr();
		} else if (lastLine == comp) {
			lastLine = AComponent::Ptr();
		} else if (firstItem == comp) {
			firstItem = AComponent::Ptr();
		} else if (lastItem == comp) {
			lastItem = AComponent::Ptr();
		}
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
AComponentPtr BorderLayout::getLayoutComponent(
		ArbitraryType::Ptr _constraint) const
{
	Constraint constraint = NONE;
	com::get(_constraint, constraint);
	switch (constraint) {
	case CENTER:
		return center;
	case NORTH:
		return north;
	case SOUTH:
		return south;
	case EAST:
		return east;
	case WEST:
		return west;
	case BEFORE_FIRST_LINE:
		return firstLine;
	case AFTER_LAST_LINE:
		return lastLine;
	case BEFORE_LINE_BEGINS:
		return firstItem;
	case AFTER_LINE_ENDS:
		return lastItem;
	default:
		SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
				"cannot get component: unknown constraint.");
	}
}
//-----------------------------------------------------------------------------
BorderLayout::Constraint
BorderLayout::getConstraints(AComponentPtr comp) const
{
	if (!comp) {
		return NONE;
	}
	if (center == comp) {
		return CENTER;
	} else if (north == comp) {
		return NORTH;
	} else if (south == comp) {
		return SOUTH;
	} else if (west == comp) {
		return WEST;
	} else if (east == comp) {
		return EAST;
	} else if (firstLine == comp) {
		return PAGE_START;
	} else if (lastLine == comp) {
		return PAGE_END;
	} else if (firstItem == comp) {
		return LINE_START;
	} else if (lastItem == comp) {
		return LINE_END;
	}
	return NONE;
}
//-----------------------------------------------------------------------------
void BorderLayout::invalidateLayout(AContainerPtr target) {
}
//-----------------------------------------------------------------------------
AComponentPtr BorderLayout::getChild(BorderLayout::Constraint key) const {
	AComponentPtr result;

	if (key == NORTH) {
		result = (firstLine) ? firstLine : north; // !! : SharedOrWeak has no bool operator
													// because it dosent work.
													// consider: a==b would always be true
													// when a==true and b==true
	} else if (key == SOUTH) {
		result = (lastLine) ? lastLine : south;
	} else if (key == WEST) {
		result = firstItem;
		if (!result) {
			result = west;
		}
	} else if (key == EAST) {
		result = lastItem;
		if (!result) {
			result = east;
		}
	} else if (key == CENTER) {
		result = center;
	}
	if (result && !result->isVisible()) {
		result = AComponentPtr();
	}
	return result;
}

}}} // namespace(s)
