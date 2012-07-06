/*
 * ScrollPane.hpp
 *
 *  Created on: Thu Jul  5 16:50:53 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_SCROLLPANE_H
#define SAMBAG_SCROLLPANE_H

#include <boost/shared_ptr.hpp>
#include "AContainer.hpp"

namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class ScrollPane.
  */
class ScrollPane : public AContainer {
//=============================================================================
public:
	enum Area {
		/**
		 * Identifies a "viewport" or display area, within which
		 * scrolled contents are visible.
		 */
		VIEWPORT,
		/** Identifies a vertical scrollbar. */
		VERTICAL_SCROLLBAR,
		/** Identifies a horizonal scrollbar. */
		HORIZONTAL_SCROLLBAR,
		/**
		 * Identifies the area along the left side of the viewport between the
		 * upper left corner and the lower left corner.
		 */
		ROW_HEADER,
		/**
		 * Identifies the area at the top the viewport between the
		 * upper left corner and the upper right corner.
		 */
		COLUMN_HEADER,
		/** Identifies the lower left corner of the viewport. */
		LOWER_LEFT_CORNER,
		/** Identifies the lower right corner of the viewport. */
		LOWER_RIGHT_CORNER,
		/** Identifies the upper left corner of the viewport. */
		UPPER_LEFT_CORNER,
		/** Identifies the upper right corner of the viewport. */
		UPPER_RIGHT_CORNER,
		/** Identifies the lower leading edge corner of the viewport.
		 * The leading edge is determined relative to the Scroll Pane's
		 * ComponentOrientation property.
		 */
		LOWER_LEADING_CORNER,
		/** Identifies the lower trailing edge corner of the viewport.
		 * The trailing edgeis determined relative to the
		 * Scroll Pane's ComponentOrientation property.
		 */
		LOWER_TRAILING_CORNER,
		/** Identifies the upper leading edge corner of the viewport.
		 * The leading edge is determined relative to the Scroll Pane's
		 * ComponentOrientation property.
		 */
		UPPER_LEADING_CORNER,
		/** Identifies the upper trailing edge corner of the viewport.
		 * The trailing edge is determined relative to the Scroll Pane's
		 * ComponentOrientation property.
		 */
		UPPER_TRAILING_CORNER
	};

	enum VerticalScrollbarPolicy {
		/**
		 * Used to set the vertical scroll bar policy so that
		 * vertical scrollbars are displayed only when needed.
		 */
		VERTICAL_SCROLLBAR_AS_NEEDED,
		/**
		 * Used to set the vertical scroll bar policy so that
		 * vertical scrollbars are never displayed.
		 */
		VERTICAL_SCROLLBAR_NEVER,
		/**
		 * Used to set the vertical scroll bar policy so that
		 * vertical scrollbars are always displayed.
		 */
		VERTICAL_SCROLLBAR_ALWAYS
	};

	enum HorizontalScrollbarPolicy {
		/**
		 * Used to set the horizontal scroll bar policy so that
		 * horizontal scrollbars are displayed only when needed.
		 */
		HORIZONTAL_SCROLLBAR_AS_NEEDED,
		/**
		 * Used to set the horizontal scroll bar policy so that
		 * horizontal scrollbars are never displayed.
		 */
		HORIZONTAL_SCROLLBAR_NEVER,
		/**
		 * Used to set the horizontal scroll bar policy so that
		 * horizontal scrollbars are always displayed.
		 */
		HORIZONTAL_SCROLLBAR_ALWAYS
	};
	//-------------------------------------------------------------------------
	/** Identifies the vertical scroll bar policy property. */
	static const std::string PROPERTY_VERTICAL_SCROLLBAR_POLICY;
	//-------------------------------------------------------------------------
	/** Identifies the horizontal scroll bar policy property. */
	static const std::string PROPERTY_HORIZONTAL_SCROLLBAR_POLICY;
	//-------------------------------------------------------------------------
	typedef AContainer Super;
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<ScrollPane> Ptr;
	//-------------------------------------------------------------------------
	virtual ui::AComponentUIPtr getComponentUI(ui::ALookAndFeelPtr laf) const;
protected:
	//-------------------------------------------------------------------------
	ScrollPane();
private:
public:
	//-------------------------------------------------------------------------
	SAMBAG_STD_STATIC_COMPONENT_CREATOR(ScrollPane)
}; // ScrollPane
}}} // namespace(s)

#endif /* SAMBAG_SCROLLPANE_H */
