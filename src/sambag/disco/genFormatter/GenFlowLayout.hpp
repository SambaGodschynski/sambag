/*
 * GenFlowLayout.hpp
 *
 *  Created on: Sat Dec  8 13:37:16 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_GENFLOWLAYOUT_H
#define SAMBAG_GENFLOWLAYOUT_H

namespace sambag { namespace disco { namespace genFormatter {
//=============================================================================
/** 
  * @class GenFlowLayout.
  */
template <class ComponentAccessPolicy>
class GenFlowLayout : public ComponentAccessPolicy {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 	
	typedef ComponentAccessPolicy AccessPolicy;
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 	
	typedef typename ComponentAccessPolicy::ComponentType ComponentType;
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	typedef typename ComponentAccessPolicy::ComponentContainer ComponentContainer;
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	typedef typename ComponentAccessPolicy::ValueType ValueType;
	//-------------------------------------------------------------------------
	/**
	 * This value indicates that each row of components
	 * should be left-justified.
	 */
	enum Aligment{LEFT, CENTER, RIGHT};
private:
	//-------------------------------------------------------------------------
	ComponentContainer objects;
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
	ValueType hgap;
	//-------------------------------------------------------------------------
	/**
	 * The flow layout manager allows a seperation of
	 * components with gaps.  The vertical gap will
	 * specify the space between rows and between the
	 * the rows and the borders of the <code>Container</code>.
	 */
	ValueType vgap;
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
	int moveComponents(const ValueType &x, const ValueType &y,
			const ValueType &width, const ValueType &height, int rowStart,
			int rowEnd);
	//-------------------------------------------------------------------------
	ValueType x, y, width, height;
public:
	//-------------------------------------------------------------------------
	GenFlowLayout() : vgap(0), 
		hgap(0), 
		align(LEFT),
		x(0),
		y(0),
		width( AccessPolicy::maxValue() ),
		height( AccessPolicy::maxValue() )
	{
	}
	//-------------------------------------------------------------------------
	const ValueType & getX() const {
		return x;	
	}
	//-------------------------------------------------------------------------
	const ValueType & getY() const {
		return y;	
	}
	//-------------------------------------------------------------------------
	const ValueType & getWidth() const {
		return width;	
	}
	//-------------------------------------------------------------------------
	const ValueType & getHeight() const {
		return height;	
	}
	//-------------------------------------------------------------------------
	void setX(const ValueType &val) {
		x = val;	
	}
	//-------------------------------------------------------------------------
	void setY(const ValueType &val) {
		y = val;	
	}
	//-------------------------------------------------------------------------
	void setWidth(const ValueType &val) {
		width = val;	
	}
	//-------------------------------------------------------------------------
	void setHeight(const ValueType &val) {
		height = val;	
	}
	//-------------------------------------------------------------------------
	const ComponentContainer & getComponents() const { return objects; }
	//-------------------------------------------------------------------------
	size_t getNumComponents() const { 
		return AccessPolicy::getSize(objects);
	}
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	void addComponent(ComponentType *obj);
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	const ComponentType * getComponent(size_t index) const {
		return AccessPolicy::get(objects, index);	
	}
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	ComponentType * getComponent(size_t index) {
		return AccessPolicy::get(objects, index);	
	}
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	void removeComponent(const ComponentType *obj);
	//-------------------------------------------------------------------------
	/**
 	 * @note Layout concept
	 */ 
	void layout();
	//-------------------------------------------------------------------------
	/**
	 * @return bounds of summary objects.
 	 * @note Layout concept
	 */ 
	void currentBounds(ValueType &x, 
		ValueType &y, 
		ValueType &width, 
		ValueType &height);
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
	const ValueType & getHgap() const {
		return hgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the horizontal gap between components and
	 * between the components and the borders of the
	 * <code>Container</code>.
	 */
	void setHgap(const ValueType &_hgap) {
		hgap = _hgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Gets the vertical gap between components and
	 * between the components and the borders of the
	 * <code>Container</code>.
	 */
	const ValueType & getVgap() const {
		return vgap;
	}
	//-------------------------------------------------------------------------
	/**
	 * Sets the vertical gap between components and between
	 * the components and the borders of the <code>Container</code>.
	 */
	void setVgap(const ValueType &_vgap) {
		vgap = _vgap;
	}
}; // GenFlowLayout
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class CA>
void GenFlowLayout<CA>::addComponent(ComponentType *obj)
{
	AccessPolicy::add(objects, obj);
}
//-----------------------------------------------------------------------------
template <class CA>
void GenFlowLayout<CA>::removeComponent(const ComponentType *obj)
{
	AccessPolicy::remove(objects, obj);
}
//-----------------------------------------------------------------------------
template <class CA>
void GenFlowLayout<CA>::layout()
{
	ValueType maxwidth = width;
	size_t nmembers = getNumComponents();
	ValueType x = 0, y = vgap;
	int rowh = 0, start = 0;

	for (size_t i = 0; i < nmembers; i++) {
		ComponentType & m = *(getComponent(i));
		ValueType cw = 0, ch = 0;
		AccessPolicy::getWidth(m, cw);	
		AccessPolicy::getHeight(m, ch);		
		
		if ((x == 0) || ((x + cw) <= maxwidth)) {
			if (x > 0) {
				x = x + hgap;
			}
			x = x + cw;
			rowh = std::max(rowh, (int)ch);
		} else {
			rowh = moveComponents(hgap, y,
					maxwidth - x, rowh, start, i);
			x = cw;
			y = y + vgap + rowh;
			rowh = (int)ch;
			start = i;
		}
	}
	moveComponents(hgap, y, maxwidth - x, rowh, start,
			nmembers);

	currentBounds(x,y,width,height);
}
//-----------------------------------------------------------------------------
template <class CA>
void GenFlowLayout<CA>::currentBounds(ValueType &x, 
	ValueType &y, 
	ValueType &width, 
	ValueType &height) 
{
	size_t nmembers = getNumComponents();
	if (nmembers == 0) {
		return;
	}
	ValueType x1, y1, x2, y2;
	x1 = y1 = AccessPolicy::maxValue(); 
	x2 = y2 = AccessPolicy::minValue(); 
	for (size_t i = 0; i < nmembers; i++) {
		ComponentType & m = *(getComponent(i));
		ValueType cx1 = 0, cy1 = 0, cx2 = 0, cy2 = 0;
		AccessPolicy::getX(m, cx1);	
		AccessPolicy::getY(m, cy1);	
		AccessPolicy::getWidth(m, cx2);	
		AccessPolicy::getHeight(m, cy2);
		cx2 = cx1 + cx2; // cx1 + witdh
		cy2 = cy1 + cy2; // cy1 + height

		x1 = std::min(cx1, x1);
		y1 = std::min(cy1, y1);
		x2 = std::max(cx2, x2);
		y2 = std::max(cy2, y2);
	}
	x=x1;
	y=y1;
	width = x2 - x1;
	height = y2 - y1;
}
//-----------------------------------------------------------------------------
template <class CA>
int GenFlowLayout<CA>::
moveComponents(const ValueType &x, const ValueType &y, const ValueType &width, 
	const ValueType &height, int rowStart, int rowEnd)
{
	ValueType _x = x;
	switch (align) {
	case LEFT:
		_x = x;
		break;
	case CENTER:
		_x = x + width / 2.;
		break;
	case RIGHT:
		_x = x + width;
		break;
	}
	for (int i = rowStart; i < rowEnd; ++i) {
		ComponentType & m = *(getComponent(i));
		ValueType cy;
		ValueType cw = 0;
		ValueType ch = 0;
		AccessPolicy::getWidth(m, cw);	
		AccessPolicy::getHeight(m, ch);	
		cy = y + (height - ch) / 2.;
		AccessPolicy::setLocation(m, _x + this->x, cy + this->y);
		_x = _x + cw + hgap;
	}
	return (int)height;
}
}}} // namespace(s)

#endif /* SAMBAG_GENFLOWLAYOUT_H */
