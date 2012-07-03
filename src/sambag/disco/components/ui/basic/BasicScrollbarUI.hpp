/*
 * BasicScrollbarUI.hpp
 *
 *  Created on: Tue Jul  3 10:11:59 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICSCROLLBARUI_H
#define SAMBAG_BASICSCROLLBARUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>
#include <sambag/disco/components/ALayoutManager.hpp>
#include <sambag/disco/components/Scrollbar.hpp>

namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicScrollbarUI.
  */
template <class ComponentModell>
class BasicScrollbarUI :
	public AComponentUI,
	public ALayoutManager
{
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicScrollbarUI> Ptr;
	//-------------------------------------------------------------------------
	typedef AComponentUI Super;
protected:
	//-------------------------------------------------------------------------
	sambag::com::ArithmeticWrapper<float, 16> scrollBarWidth;
	//-------------------------------------------------------------------------
	void paintTrack(IDrawContext::Ptr cn,
			AComponentPtr c, const Rectangle &trackBounds);
	//-------------------------------------------------------------------------
	ColorRGBA trackColor;
private:
public:
	//-------------------------------------------------------------------------
	virtual bool contains(AComponent::Ptr c, const Point2D &p);
	//-------------------------------------------------------------------------
	/**
	 * Configures the specified component appropriately for the look and feel.
	 * @param c
	 */
	virtual void installUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new BasicScrollbarUI<ComponentModell>());
	}
	//-------------------------------------------------------------------------
	/**
	 * Paints the specified component appropriately for the look and feel.
	 * @param cn
	 * @param c
	 */
	virtual void draw(IDrawContext::Ptr cn, AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Returns the specified component's maximum size appropriate for the
	 * look and feel.
	 * @param c
	 * @return
	 */
	virtual Dimension getMaximumSize(AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Returns the specified component's preferred size appropriate for the
	 * look and feel.
	 * @param c
	 * @return
	 */
	virtual Dimension getPreferredSize(AComponentPtr c);
	//-------------------------------------------------------------------------
	// LayoutManager impl.
	virtual Dimension preferredLayoutSize(AContainerPtr scrollbarContainer)
	{
		return getPreferredSize(scrollbarContainer);
	}
	//-------------------------------------------------------------------------
	virtual Dimension minimumLayoutSize(AContainerPtr scrollbarContainer) {
		return getMinimumSize(scrollbarContainer);
}
}; // BasicScrollbarUI
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class M>
bool BasicScrollbarUI<M>::contains(AComponent::Ptr c, const Point2D &p) {
	return Super::contains(c, p);
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::installUI(AComponentPtr c) {

}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::draw(IDrawContext::Ptr cn, AComponentPtr c) {

}
//-----------------------------------------------------------------------------
template <class M>
Dimension BasicScrollbarUI<M>::getMaximumSize(AComponentPtr c) {
	return Dimension(INT_MAX, INT_MAX);
}
//-----------------------------------------------------------------------------
template <class M>
Dimension BasicScrollbarUI<M>::getPreferredSize(AComponentPtr c) {
	typename AScrollbar<M>::Ptr scrB =
			boost::shared_dynamic_cast< AScrollbar<M> >(c);
	SAMBAG_ASSERT(scrB);
	return (
			scrB->getOrientation() == AScrollbar<M>::VERTICAL) ? Dimension(
		(double)scrollBarWidth, 48.) : Dimension(48., (double)scrollBarWidth
	);
}
//-----------------------------------------------------------------------------
template <class M>
void BasicScrollbarUI<M>::paintTrack(IDrawContext::Ptr cn,
		AComponentPtr c, const Rectangle &trackBounds)
{
	cn->setFillColor(trackColor);
	cn->rect(trackBounds);
	cn->fill();

//	if (trackHighlight == DECREASE_HIGHLIGHT) {
//		paintDecreaseHighlight( g);
//	} else if (trackHighlight == INCREASE_HIGHLIGHT) {
//		paintIncreaseHighlight( g);
//	}
}
}}}}} // namespace(s)

#endif /* SAMBAG_BASICSCROLLBARUI_H */
