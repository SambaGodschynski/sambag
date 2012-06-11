/*
 * BoxLayout.cpp
 *
 *  Created on: Mon Jun 11 14:06:51 2012
 *      Author: Johannes Unger
 */

#include "BoxLayout.hpp"
#include <sambag/com/exceptions/IllegalArgumentException.hpp>
#include <algorithm>
#include "AContainer.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class BoxLayout
//=============================================================================
//-----------------------------------------------------------------------------
BoxLayout::BoxLayout(AContainerPtr target, Axis axis) :
		target(target), axis(axis)
{
	if (axis != X_AXIS && axis != Y_AXIS && axis != LINE_AXIS && axis
			!= PAGE_AXIS) {
		SAMBAG_THROW(sambag::com::exceptions::IllegalArgumentException,
				"invalid axes");
	}
}
//-----------------------------------------------------------------------------
BoxLayout::Axis BoxLayout::getAxis() const {
	return axis;
}
//-----------------------------------------------------------------------------
Coordinate BoxLayout::getLayoutAlignmentX(AContainerPtr target) {
	checkContainer(target);
	checkRequests();
	return xTotal.alignment;
}
//-----------------------------------------------------------------------------
Coordinate BoxLayout::getLayoutAlignmentY(AContainerPtr target) {
	checkContainer(target);
	checkRequests();
	return yTotal.alignment;
}
//-----------------------------------------------------------------------------
void BoxLayout::invalidateLayout(AContainerPtr target) {
	checkContainer(target);
	xChildren.clear();
	yChildren.clear();
	xTotal = NULL_NUMBER;
	yTotal = NULL_NUMBER;
}
//-----------------------------------------------------------------------------
void BoxLayout::layoutContainer(AContainerPtr target) {
	checkContainer(target);
	size_t nChildren = target->getComponentCount();
	SizeRequirements::Ints xOffsets = SizeRequirements::Ints(nChildren, 0);
	SizeRequirements::Ints xSpans = SizeRequirements::Ints(nChildren, 0);
	SizeRequirements::Ints yOffsets = SizeRequirements::Ints(nChildren, 0);
	SizeRequirements::Ints ySpans = SizeRequirements::Ints(nChildren, 0);

	Dimension alloc = target->getSize();
	Insets in = target->getInsets();
	alloc.width( alloc.width() - in.left() + in.right() );
	alloc.height(alloc.height() - in.top() + in.bottom() );

	Axis absoluteAxis = resolveAxis(axis);

	// determine the child placements
	SAMBAG_BEGIN_SYNCHRONIZED(lock)
		checkRequests();
		if (absoluteAxis == X_AXIS) {
			SizeRequirements::calculateTiledPositions(alloc.width(), xTotal,
					xChildren, xOffsets,
					xSpans);
			SizeRequirements::calculateAlignedPositions(alloc.height(), yTotal,
					yChildren, yOffsets,
					ySpans);
		} else {
			SizeRequirements::calculateAlignedPositions(alloc.width(), xTotal,
					xChildren, xOffsets,
					xSpans);
			SizeRequirements::calculateTiledPositions(alloc.height(), yTotal,
					yChildren, yOffsets,
					ySpans);
		}
	SAMBAG_END_SYNCHRONIZED

	// flush changes to the container
	for (size_t i = 0; i < nChildren; ++i) {
		AComponentPtr c = target->getComponent(i);
		c->setBounds(
				(Coordinate) std::min((long) in.left() + (long) xOffsets[i],
						LONG_MAX),
				(Coordinate) std::min((long) in.top() + (long) yOffsets[i],
						LONG_MAX), xSpans[i], ySpans[i]);

	}
}
//-----------------------------------------------------------------------------
Dimension BoxLayout::maximumLayoutSize(AContainerPtr target) {
	Dimension size;
	SAMBAG_BEGIN_SYNCHRONIZED(lock)
		checkContainer(target);
		checkRequests();
		size = Dimension(xTotal.maximum, yTotal.maximum);
	SAMBAG_END_SYNCHRONIZED

	Insets insets = target->getInsets();
	size.width( (int) std::min(
			(long) size.width() + (long) insets.left() + (long) insets.right(),
			LONG_MAX));
	size.height( (int) std::min(
			(long) size.height() + (long) insets.top() + (long) insets.bottom(),
			LONG_MAX));
	return size;
}
//-----------------------------------------------------------------------------
Dimension BoxLayout::minimumLayoutSize(AContainerPtr target) {
	Dimension size;
	SAMBAG_BEGIN_SYNCHRONIZED(lock)
		checkContainer(target);
		checkRequests();
		size = Dimension(xTotal.minimum, yTotal.minimum);
	SAMBAG_END_SYNCHRONIZED

	Insets insets = target->getInsets();
	size.width( (int) std::min(
			(long) size.width() + (long) insets.left() + (long) insets.right(),
			LONG_MAX));
	size.height( (int) std::min(
			(long) size.height() + (long) insets.top() + (long) insets.bottom(),
			LONG_MAX));
	return size;
}
//-----------------------------------------------------------------------------
Dimension BoxLayout::preferredLayoutSize(AContainerPtr target) {
	Dimension size;
	SAMBAG_BEGIN_SYNCHRONIZED(lock)
		checkContainer(target);
		checkRequests();
		size = Dimension(xTotal.preferred, yTotal.preferred);
	SAMBAG_END_SYNCHRONIZED

	Insets insets = target->getInsets();
	size.width( (int) std::min(
			(long) size.width() + (long) insets.left() + (long) insets.right(),
			LONG_MAX));
	size.height( (int) std::min(
			(long) size.height() + (long) insets.top() + (long) insets.bottom(),
			LONG_MAX));
	return size;
}
//-----------------------------------------------------------------------------
void BoxLayout::checkContainer(AComponentPtr comp) const {
	if (target != comp) {
		SAMBAG_THROW( sambag::com::exceptions::IllegalArgumentException,
			"BoxLayout can't be shared");
	}
}
//-----------------------------------------------------------------------------
void BoxLayout::checkRequests() {
	if (xChildren.empty() || yChildren.empty()) {
		// The requests have been invalidated... recalculate
		// the request information.
		size_t n = target->getComponentCount();
		xChildren = SizeRequirements::Container(n);
		yChildren = SizeRequirements::Container(n);
		for (size_t i = 0; i < n; ++i) {
			AComponentPtr c = target->getComponent(i);
			if (!c->isVisible()) {
				xChildren[i] = SizeRequirements(0, 0, 0, c->getAlignmentX());
				yChildren[i] = SizeRequirements(0, 0, 0, c->getAlignmentY());
				continue;
			}
			Dimension min = c->getMinimumSize();
			Dimension typ = c->getPreferredSize();
			Dimension max = c->getMaximumSize();
			xChildren[i] = SizeRequirements(min.width(), typ.width(),
					max.width(), c->getAlignmentX());
			yChildren[i] = SizeRequirements(min.height(), typ.height(),
					max.height(), c->getAlignmentY());
		}

		// Resolve axis to an absolute value (either X_AXIS or Y_AXIS)
		Axis absoluteAxis = resolveAxis(axis);

		if (absoluteAxis == X_AXIS) {
			xTotal = SizeRequirements::getTiledSizeRequirements(xChildren);
			yTotal = SizeRequirements::getAlignedSizeRequirements(yChildren);
		} else {
			xTotal = SizeRequirements::getAlignedSizeRequirements(xChildren);
			yTotal = SizeRequirements::getTiledSizeRequirements(yChildren);
		}
	}
}
//-----------------------------------------------------------------------------
void BoxLayout::addLayoutComponent(AComponentPtr comp) {
	invalidateLayout(comp->getParent());
}
//-----------------------------------------------------------------------------
void BoxLayout::removeLayoutComponent(AComponentPtr comp) {
	invalidateLayout(comp->getParent());
}
//-------------------------------------------------------------------------
BoxLayout::Axis BoxLayout::resolveAxis( Axis axis ) {
	Axis absoluteAxis;
	const bool isHorizontal = true;
	if (axis == LINE_AXIS) {
		absoluteAxis = isHorizontal ? X_AXIS : Y_AXIS;
	} else if (axis == PAGE_AXIS) {
		absoluteAxis = isHorizontal ? Y_AXIS : X_AXIS;
	} else {
		absoluteAxis = axis;
	}
	return absoluteAxis;
}
//-----------------------------------------------------------------------------
std::string BoxLayout::toString() const {
	if (!target)
		return "BoxLayout for NULL";
	return "BoxLayout for " + target->toString();
}

}}} // BoxLayout::namespace(s)
