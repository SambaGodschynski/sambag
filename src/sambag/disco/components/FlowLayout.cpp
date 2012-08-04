/*
 * FlowLayout.cpp
 *
 *  Created on: Tue May 29 16:55:54 2012
 *      Author: Johannes Unger
 */

#include "FlowLayout.hpp"
#include "AContainer.hpp"
#include <algorithm>
#include <sstream>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class FlowLayout
//=============================================================================
//-----------------------------------------------------------------------------
int FlowLayout::moveComponents(AContainerPtr target, Coordinate x,
		Coordinate y, Coordinate width, Coordinate height,
		int rowStart, int rowEnd, bool ltr,
		bool useBaseline, const FlowLayout::Indices &ascent,
		const FlowLayout::Indices &descent) const
{
	switch (align) {
	case LEFT:
		x = x + ltr ? (Coordinate)0 : width;
		break;
	case CENTER:
		x = x + width / 2.;
		break;
	case RIGHT:
		x = x + ltr ? width : (Coordinate)0;
		break;
	}
	Coordinate maxAscent = 0;
	Coordinate nonbaselineHeight = 0;
	Coordinate baselineOffset = 0;
	if (useBaseline) {
		int maxDescent = 0;
		for (int i = rowStart; i < rowEnd; ++i) {
			AComponent::Ptr m = target->getComponent(i);
			if (m->isVisible()) {
				if (ascent[i] >= 0) {
					maxAscent = std::max(maxAscent, (Coordinate)ascent[i]);
					maxDescent = std::max(maxDescent, descent[i]);
				} else {
					nonbaselineHeight = std::max(m->getHeight(),
							nonbaselineHeight);
				}
			}
		}
		height = std::max((Coordinate)(maxAscent + maxDescent), nonbaselineHeight);
		baselineOffset = (height - maxAscent - maxDescent) / 2.;
	}
	for (int i = rowStart; i < rowEnd; ++i) {
		AComponent::Ptr m = target->getComponent(i);
		if (m->isVisible()) {
			Coordinate cy;
			if (useBaseline && ascent[i] >= 0) {
				cy = y + baselineOffset + maxAscent - ascent[i];
			} else {
				cy = y + (height - m->getHeight()) / 2.;
			}
			if (ltr) {
				m->setLocation(x, cy);
			} else {
				m->setLocation(target->getWidth() - x - m->getWidth(), cy);
			}
			x = x + m->getWidth() + hgap;
		}
	}
	return (int)height;
}
//-----------------------------------------------------------------------------
Dimension FlowLayout::preferredLayoutSize(AContainerPtr target) {
	Dimension dim;
	SAMBAG_BEGIN_SYNCHRONIZED(target->getTreeLock())
	size_t nmembers = target->getComponentCount();
	bool firstVisibleComponent = true;
	bool useBaseline = getAlignOnBaseline();
	Coordinate maxAscent = 0;
	Coordinate maxDescent = 0;

	for (size_t i = 0; i < nmembers; ++i) {
		AComponentPtr m = target->getComponent(i);
		if (m->isVisible()) {
			Dimension d = m->getPreferredSize();
			dim.height( std::max(dim.height(), d.height()) );
			if (firstVisibleComponent) {
				firstVisibleComponent = false;
			} else {
				dim.width( dim.width() + hgap );
			}
			dim.width ( dim.width() + d.width() );
			if (useBaseline) {
				Coordinate baseline = m->getBaseLine(d.width(), d.height());
				if (baseline >= 0) {
					maxAscent = std::max(maxAscent, baseline);
					maxDescent = std::max(maxDescent, (Coordinate)(d.height() - baseline));
				}
			}
		}
	}
	if (useBaseline) {
		dim.height( std::max((Coordinate)(maxAscent + maxDescent), dim.height()) );
	}
	Insets insets = target->getInsets();
	dim.width( dim.width() + insets.left() + insets.right() + hgap * 2 );
	dim.height( dim.height() + insets.top() + insets.bottom() + vgap * 2 );
	SAMBAG_END_SYNCHRONIZED
	return dim;
}
//-----------------------------------------------------------------------------
Dimension FlowLayout::minimumLayoutSize(AContainerPtr target) {
	Dimension dim;
	SAMBAG_BEGIN_SYNCHRONIZED(target->getTreeLock())
	bool useBaseline = getAlignOnBaseline();
	size_t nmembers = target->getComponentCount();
	Coordinate maxAscent = 0;
	Coordinate maxDescent = 0;
	bool firstVisibleComponent = true;

	for (size_t i = 0; i < nmembers; i++) {
		AComponent::Ptr m = target->getComponent(i);
		if (m->isVisible()) {
			Dimension d = m->getMinimumSize();
			dim.height( std::max(dim.height(), d.height()) );
			if (firstVisibleComponent) {
				firstVisibleComponent = false;
			} else {
				dim.width( dim.width() + hgap );
			}
			dim.width(dim.width() + d.width() );
			if (useBaseline) {
				Coordinate baseline = m->getBaseLine(d.width(), d.height());
				if (baseline >= 0) {
					maxAscent = std::max(maxAscent, (Coordinate)baseline);
					maxDescent = std::max(maxDescent, (Coordinate)(dim.height() - baseline));
				}
			}
		}
	}

	if (useBaseline) {
		dim.height( std::max( (Coordinate)(maxAscent + maxDescent), dim.height()) );
	}

	Insets insets = target->getInsets();
	dim.width ( dim.width() + insets.left() + insets.right() + hgap * 2 );
	dim.height ( dim.width() + insets.top() + insets.bottom() + vgap * 2 );
	SAMBAG_END_SYNCHRONIZED
	return dim;
}
//-----------------------------------------------------------------------------
void FlowLayout::layoutContainer(AContainerPtr target) {
	SAMBAG_BEGIN_SYNCHRONIZED(target->getTreeLock())
	Insets insets = target->getInsets();
	Coordinate maxwidth = target->getWidth() - (insets.left() + insets.right() + hgap * 2);
	size_t nmembers = target->getComponentCount();
	Coordinate x = 0, y = insets.top() + vgap;
	Coordinate rowh = 0, start = 0;

	bool ltr = true; //target.getComponentOrientation().isLeftToRight();

	bool useBaseline = getAlignOnBaseline();
	Indices ascent;
	Indices descent;

	if (useBaseline) {
		ascent = Indices(nmembers);
		descent = Indices(nmembers);
	}

	for (size_t i = 0; i < nmembers; i++) {
		AComponent::Ptr m = target->getComponent(i);
		if (m->isVisible()) {
			Dimension d = m->getPreferredSize();
			m->setSize(d);

			if (useBaseline) {
				int baseline = (int)(m->getBaseLine(d.width(), d.height()));
				if (baseline >= 0) {
					ascent[i] = baseline;
					descent[i] = (int)(d.height() - baseline);
				} else {
					ascent[i] = -1;
				}
			}
			if ((x == 0) || ((x + d.width()) <= maxwidth)) {
				if (x > 0) {
					x = x + hgap;
				}
				x = x + d.width();
				rowh = std::max(rowh, d.height());
			} else {
				rowh = moveComponents(target, insets.left() + hgap, y,
						maxwidth - x, rowh, start, i, ltr, useBaseline, ascent,
						descent);
				x = d.width();
				y = y + vgap + rowh;
				rowh = d.height();
				start = i;
			}
		}
	}
	moveComponents(target, insets.left() + hgap, y, maxwidth - x, rowh, start,
			nmembers, ltr, useBaseline, ascent, descent);
	SAMBAG_END_SYNCHRONIZED
}
//-----------------------------------------------------------------------------
std::string FlowLayout::toString() const {
	std::stringstream ss;
	std::string str;
	switch (align) {
	case LEFT:
		str = ",align=left";
		break;
	case CENTER:
		str = ",align=center";
		break;
	case RIGHT:
		str = ",align=right";
		break;
	}
	ss << "FlowLayout [hgap=" << hgap << ",vgap=" << vgap << str << "]";
	return ss.str();
}

}}} // namespace(s)
