/*
 * FlowLayout.hpp
 *
 *  Created on: Tue May 29 16:55:54 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_FLOWLAYOUT_H
#define SAMBAG_FLOWLAYOUT_H

#include <boost/shared_ptr.hpp>
#include "ALayoutManager.hpp"
#include <sambag/com/ArithmeticWrapper.hpp>

namespace sambag { namespace disco { namespace components {

//=============================================================================
/** 
 * @class FlowLayout.
 * A flow layout arranges components in a directional flow, much
 * like lines of text in a paragraph.
 */
class FlowLayout : public ALayoutManager {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<FlowLayout> Ptr;
	//-------------------------------------------------------------------------
	/**
	 * This value indicates that each row of components
	 * should be left-justified.
	 */
	enum Aligment{LEFT, CENTER, RIGHT};
	//-------------------------------------------------------------------------
	Aligment align;
	//-------------------------------------------------------------------------
	/**
	 * The flow layout manager allows a seperation of
	 * components with gaps.  The horizontal gap will
	 * specify the space between components and between
	 * the components and the borders of the
	 * <code>Container</code>.
	 */
	Coordinate hgap;
	//-------------------------------------------------------------------------
	/**
	 * The flow layout manager allows a seperation of
	 * components with gaps.  The vertical gap will
	 * specify the space between rows and between the
	 * the rows and the borders of the <code>Container</code>.
	 */
	Coordinate vgap;
	//-------------------------------------------------------------------------
	/**
	 * If true, components will be aligned on their baseline.
	 */
	ArithmeticWrapper<bool> alignOnBaseline;
protected:
	//-------------------------------------------------------------------------
	typedef std::vector<int> Indices;
	//-------------------------------------------------------------------------
	FlowLayout(Aligment align, int hgap, int vgap) :
		hgap(hgap),
		vgap(vgap)
	{
		setAlignment(align);
	}
private:
	//-------------------------------------------------------------------------
	/**
	 * Centers the elements in the specified row, if there is any slack.
	 * @param target the component which needs to be moved
	 * @param x the x coordinate
	 * @param y the y coordinate
	 * @param width the width dimensions
	 * @param height the height dimensions
	 * @param rowStart the beginning of the row
	 * @param rowEnd the the ending of the row
	 * @param useBaseline Whether or not to align on baseline.
	 * @param ascent Ascent for the components. This is only valid if
	 *               useBaseline is true.
	 * @param descent Ascent for the components. This is only valid if
	 *               useBaseline is true.
	 * @return actual row height
	 */
	virtual int moveComponents(AContainerPtr target, Coordinate x, Coordinate y,
			Coordinate width, Coordinate height, int rowStart,
			int rowEnd, bool ltr, bool useBaseline, const Indices &ascent,
			const Indices &descent) const;
public:
	//-------------------------------------------------------------------------
	/**
	 * Creates a new flow layout manager with the indicated alignment
	 * and the indicated horizontal and vertical gaps.
	 * @param align
	 * @param hgap
	 * @param vgap
	 */
	static Ptr create(Aligment align = CENTER, int hgap = 5, int vgap = 5)
	{
		Ptr res(new FlowLayout(align, hgap, vgap));
		res->self = res;
		return res;
	}
	//-------------------------------------------------------------------------
	/**
	 * Gets the alignment for this layout.
	 * Possible values are <code>FlowLayout.LEFT</code>,
	 * <code>FlowLayout.RIGHT</code>, <code>FlowLayout.CENTER</code>,
	 * <code>FlowLayout.LEADING</code>,
	 * or <code>FlowLayout.TRAILING</code>.
	 */
	virtual Aligment getAlignment() const {
		return align;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the alignment for this layout.
	 * Possible values are
	 * <ul>
	 * <li><code>FlowLayout.LEFT</code>
	 * <li><code>FlowLayout.RIGHT</code>
	 * <li><code>FlowLayout.CENTER</code>
	 * <li><code>FlowLayout.LEADING</code>
	 * <li><code>FlowLayout.TRAILING</code>
	 * </ul>
	 */
	virtual void setAlignment(Aligment _align) {
		align = _align;
	}
	//-------------------------------------------------------------------------
	/**
	 * Gets the horizontal gap between components
	 * and between the components and the borders
	 * of the <code>Container</code>
	 */
	virtual Coordinate getHgap() const {
		return hgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the horizontal gap between components and
	 * between the components and the borders of the
	 * <code>Container</code>.
	 */
	virtual void setHgap(Coordinate _hgap) {
		hgap = _hgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Gets the vertical gap between components and
	 * between the components and the borders of the
	 * <code>Container</code>.
	 */
	virtual Coordinate getVgap() const {
		return vgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the vertical gap between components and between
	 * the components and the borders of the <code>Container</code>.
	 */
	virtual void setVgap(Coordinate _vgap) {
		vgap = _vgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets whether or not components should be vertically aligned along their
	 * baseline.  Components that do not have a baseline will be centered.
	 * The default is false.
	 */
	virtual void setAlignOnBaseline(bool _alignOnBaseline) {
		alignOnBaseline = _alignOnBaseline;
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns true if components are to be vertically aligned along
	 * their baseline.  The default is false.
	 */
	virtual bool getAlignOnBaseline() const {
		return alignOnBaseline;
	}
	//-------------------------------------------------------------------------
	/**
	 * Removes the specified component from the layout.
	 * Not used by this class.
	 * @param comp the component to remove
	 * @see       java.awt.Container#removeAll
	 */
	virtual void removeLayoutComponent(AComponentPtr comp) {
	}
	//-------------------------------------------------------------------------
	/**
	 * Returns the preferred dimensions for this layout given the
	 * <i>visible</i> components in the specified target container.
	 *
	 * @param target the container that needs to be laid out
	 * @return    the preferred dimensions to lay out the
	 *            subcomponents of the specified container
	 * @see Container
	 * @see #minimumLayoutSize
	 * @see       java.awt.Container#getPreferredSize
	 */
	virtual Dimension preferredLayoutSize(AContainerPtr target);
	//-------------------------------------------------------------------------
	/**
	 * Returns the minimum dimensions needed to layout the <i>visible</i>
	 * components contained in the specified target container.
	 * @param target the container that needs to be laid out
	 * @return    the minimum dimensions to lay out the
	 *            subcomponents of the specified container
	 */
	virtual Dimension minimumLayoutSize(AContainerPtr target);
	//-------------------------------------------------------------------------
	/**
	 * Lays out the container. This method lets each
	 * <i>visible</i> component take
	 * its preferred size by reshaping the components in the
	 * target container in order to satisfy the alignment of
	 * this <code>FlowLayout</code> object.
	 */
	virtual void layoutContainer(AContainerPtr target);
	//-------------------------------------------------------------------------
	/**
	 * Returns a string representation of this <code>FlowLayout</code>
	 * object and its values.
	 * @return     a string representation of this layout
	 */
	virtual std::string toString() const;
}; // FlowLayout
}}} // namespace(s)

#endif /* SAMBAG_FLOWLAYOUT_H */
