/*
 * BorderLayout.hpp
 *
 *  Created on: Sat Jul 14 11:54:44 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BORDERLAYOUT_H
#define SAMBAG_BORDERLAYOUT_H

#include <boost/shared_ptr.hpp>
#include "ALayoutManager.hpp"
namespace sambag { namespace disco { namespace components {
//=============================================================================
/** 
  * @class BorderLayout.
  */
class BorderLayout : public ALayoutManager {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BorderLayout> Ptr;
protected:
	//-------------------------------------------------------------------------
	/**
	 * Constructs a border layout with the horizontal gaps
	 * between components.
	 * The horizontal gap is specified by <code>hgap</code>.
	 *
	 * @see #getHgap()
	 * @see #setHgap(int)
	 */
	Coordinate hgap;
	//-------------------------------------------------------------------------
	/**
	 * Constructs a border layout with the vertical gaps
	 * between components.
	 * The vertical gap is specified by <code>vgap</code>.
	 *
	 * @see #getVgap()
	 * @see #setVgap(int)
	 */
	Coordinate vgap;
	//-------------------------------------------------------------------------
	/**
	 * Constant to specify components location to be the
	 *      north portion of the border layout.
	 * @see #getChild(String, boolean)
	 * @see #addLayoutComponent
	 * @see #getLayoutAlignmentX
	 * @see #getLayoutAlignmentY
	 * @see #removeLayoutComponent
	 */
	AComponentPtr north;
	//-------------------------------------------------------------------------
	/**
	 * Constant to specify components location to be the
	 *      west portion of the border layout.
	 * @see #getChild(String, boolean)
	 * @see #addLayoutComponent
	 * @see #getLayoutAlignmentX
	 * @see #getLayoutAlignmentY
	 * @see #removeLayoutComponent
	 */
	AComponentPtr west;
	//-------------------------------------------------------------------------
	/**
	 * Constant to specify components location to be the
	 *      east portion of the border layout.
	 * @see #getChild(String, boolean)
	 * @see #addLayoutComponent
	 * @see #getLayoutAlignmentX
	 * @see #getLayoutAlignmentY
	 * @see #removeLayoutComponent
	 */
	AComponentPtr east;
	//-------------------------------------------------------------------------
	/**
	 * Constant to specify components location to be the
	 *      south portion of the border layout.
	 * @see #getChild(String, boolean)
	 * @see #addLayoutComponent
	 * @see #getLayoutAlignmentX
	 * @see #getLayoutAlignmentY
	 * @see #removeLayoutComponent
	 */
	AComponentPtr south;
	//-------------------------------------------------------------------------
	/**
	 * Constant to specify components location to be the
	 *      center portion of the border layout.
	 * @see #getChild(String, boolean)
	 * @see #addLayoutComponent
	 * @see #getLayoutAlignmentX
	 * @see #getLayoutAlignmentY
	 * @see #removeLayoutComponent
	 */
	AComponentPtr center;
	//-------------------------------------------------------------------------
	/**
	 *
	 * A relative positioning constant, that can be used instead of
	 * north, south, east, west or center.
	 * mixing the two types of constants can lead to unpredicable results.  If
	 * you use both types, the relative constants will take precedence.
	 * For example, if you add components using both the <code>NORTH</code>
	 * and <code>BEFORE_FIRST_LINE</code> constants in a container whose
	 * orientation is <code>LEFT_TO_RIGHT</code>, only the
	 * <code>BEFORE_FIRST_LINE</code> will be layed out.
	 * This will be the same for lastLine, firstItem, lastItem.
	 */
	AComponentPtr firstLine;
	//-------------------------------------------------------------------------
	/**
	 * A relative positioning constant, that can be used instead of
	 * north, south, east, west or center.
	 * Please read Description for firstLine.
	 */
	AComponentPtr lastLine;
	//-------------------------------------------------------------------------
	/**
	 * A relative positioning constant, that can be used instead of
	 * north, south, east, west or center.
	 * Please read Description for firstLine.
	 * @serial
	 */
	AComponentPtr firstItem;
	//-------------------------------------------------------------------------
	/**
	 * A relative positioning constant, that can be used instead of
	 * north, south, east, west or center.
	 * Please read Description for firstLine.
	 * @serial
	 */
	AComponentPtr lastItem;
	//-------------------------------------------------------------------------
	BorderLayout(const Coordinate &hgap, const Coordinate &vgap);
public:
	//-------------------------------------------------------------------------
	/**
	 * Returns the horizontal gap between components.
	 */
	const Coordinate & getHgap() const {
		return hgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the horizontal gap between components.
	 * @param hgap the horizontal gap between components
	 */
	void setHgap(const Coordinate & hgap) {
		this->hgap = hgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the vertical gap between components.
	 * @since   JDK1.1
	 */
	const Coordinate & getVgap() const {
		return vgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the vertical gap between components.
	 * @param vgap the vertical gap between components
	 * @since   JDK1.1
	 */
	void setVgap(const Coordinate & vgap) {
		this->vgap = vgap;
	}
	//-------------------------------------------------------------------------
	enum Constraint {
		NONE,
		/**
		 * The north layout constraint (top of container).
		 */
		NORTH,
		/**
		 * The south layout constraint (bottom of container).
		 */
		SOUTH,
		/**
		 * The east layout constraint (right side of container).
		 */
		EAST,
		/**
		 * The west layout constraint (left side of container).
		 */
		WEST,
		/**
		 * The center layout constraint (middle of container).
		 */
		CENTER,
		/**
		 * Synonym for PAGE_START.  Exists for compatibility with previous
		 * versions.  PAGE_START is preferred.
		 *
		 * @see #PAGE_START
		 */
		BEFORE_FIRST_LINE,
		/**
		 * Synonym for PAGE_END.  Exists for compatibility with previous
		 * versions.  PAGE_END is preferred.
		 *
		 * @see #PAGE_END
		 */
		AFTER_LAST_LINE,
		/**
		 * Synonym for LINE_START.  Exists for compatibility with previous
		 * versions.  LINE_START is preferred.
		 *
		 * @see #LINE_START
		 */
		BEFORE_LINE_BEGINS,
		/**
		 * Synonym for LINE_END.  Exists for compatibility with previous
		 * versions.  LINE_END is preferred.
		 *
		 * @see #LINE_END
		 */
		AFTER_LINE_ENDS,
		/**
		 * The component comes before the first line of the layout's content.
		 * For Western, left-to-right and top-to-bottom orientations, this is
		 * equivalent to NORTH.
		 *
		 */
		PAGE_START,
		/**
		 * The component comes after the last line of the layout's content.
		 * For Western, left-to-right and top-to-bottom orientations, this is
		 * equivalent to SOUTH.
		 *
		 */
		PAGE_END,
		/**
		 * The component goes at the beginning of the line direction for the
		 * layout. For Western, left-to-right and top-to-bottom orientations,
		 * this is equivalent to WEST.
		 *
		 */
		LINE_START,
		/**
		 * The component goes at the end of the line direction for the
		 * layout. For Western, left-to-right and top-to-bottom orientations,
		 * this is equivalent to EAST.
		 */
		LINE_END
	};
	//-------------------------------------------------------------------------
	/**
	 * Gets the constraints for the specified component
	 *
	 * @param   comp the component to be queried
	 * @return  the constraint for the specified component,
	 *          or null if component is null or is not present
	 *          in this layout
	 * @see #addLayoutComponent(java.awt.Component, java.lang.Object)
	 * @since 1.5
	 */
	Constraint getConstraints(AComponentPtr comp) const;
	//-------------------------------------------------------------------------
	static Ptr create() {
		Ptr res(new BorderLayout(0,0));
		return res;
	}
	//-------------------------------------------------------------------------
	static Ptr create(const Coordinate &hgap, const Coordinate &vgap) {
		Ptr res(new BorderLayout(hgap,vgap));
		return res;
	}
	//-------------------------------------------------------------------------
	/**
	 * @param constraint
	 * @return
	 */
	virtual AComponentPtr getLayoutComponent(
		ArbitraryType::Ptr constraint) const;
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param comp
	 * @param constraint
	 */
	void addLayoutComponent(AComponentPtr comp,
			ArbitraryType::Ptr constraint = ArbitraryType::Ptr());
	//-------------------------------------------------------------------------
	/**
	 * Lays out the specified container.
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param parent
	 */
	virtual void layoutContainer(AContainerPtr parent);
	//-------------------------------------------------------------------------
	/**
	 * Calculates the minimum size dimensions for the specified container,
	 * given the components it contains.
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param parent
	 * @return
	 */
	virtual Dimension minimumLayoutSize(AContainerPtr parent);
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
		return Dimension(FLT_MAX, FLT_MAX);
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
	virtual Dimension preferredLayoutSize(AContainerPtr parent);
	//-------------------------------------------------------------------------
	/**
	 * Removes the specified component from the layout.
	 * @param comp
	 */
	virtual void removeLayoutComponent(AComponentPtr comp);
	//-------------------------------------------------------------------------
	/**
	 *
	 * @param comp
	 */
	virtual void invalidateLayout(AContainerPtr target);
	//-------------------------------------------------------------------------
	/**
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param target
	 * @return
	 */
	virtual Coordinate getLayoutAlignmentY(AContainerPtr target) {
		return 0.5;
	}
	//-------------------------------------------------------------------------
	/**
	 * @note: not const because its not sure that derived classes can comply
	 * the const rules.
	 * @param target
	 * @return
	 */
	virtual Coordinate getLayoutAlignmentX(AContainerPtr target) {
		return 0.5;
	}
	//-------------------------------------------------------------------------
	virtual std::string toString() const {
		return "BorderLayout";
	}
private:
	//-------------------------------------------------------------------------
	/**
	 * Get the component that corresponds to the given constraint location
	 *
	 * @param   key     The desired absolute position,
	 *                  either NORTH, SOUTH, EAST, or WEST.
	 * @param   ltr     Is the component line direction left-to-right?
	 */
	AComponentPtr getChild(Constraint) const;
}; // BorderLayout
}}} // namespace(s)

#endif /* SAMBAG_BORDERLAYOUT_H */
