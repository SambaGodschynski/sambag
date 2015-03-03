/*
 * BasicPopupMenuUI.hpp
 *
 *  Created on: Mon Jun 11 11:14:11 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_BASICPOPUPMENUUI_H
#define SAMBAG_BASICPOPUPMENUUI_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>
namespace sambag { namespace disco {
namespace components { namespace ui { namespace basic {
//=============================================================================
/** 
  * @class BasicPopupMenuUI.
  */
template <class ComponentModell>
class BasicPopupMenuUI : public AComponentUI {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<BasicPopupMenuUI<ComponentModell> > Ptr;
protected:
	//-------------------------------------------------------------------------
	BasicPopupMenuUI();
private:
public:
	//-------------------------------------------------------------------------
	/**
	 * Configures the specified component appropriately for the look and feel.
	 * @param c
	 */
	virtual void installUI(AComponentPtr c);
	//-------------------------------------------------------------------------
	static Ptr create() {
		return Ptr(new BasicPopupMenuUI<ComponentModell>());
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
	virtual Dimension getMinimumSize(AComponentPtr c);
	//-------------------------------------------------------------------------
	/**
	 * Returns the specified component's preferred size appropriate for the
	 * look and feel.
	 * @param c
	 * @return
	 */
	virtual Dimension getPreferredSize(AComponentPtr c);
}; // BasicPopupMenuUI
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
template <class ComponentModell>
BasicPopupMenuUI<ComponentModell>::BasicPopupMenuUI() {
}
//-----------------------------------------------------------------------------
template <class ComponentModell>
void BasicPopupMenuUI<ComponentModell>::draw(IDrawContext::Ptr cn, AComponentPtr c) {

}
//-----------------------------------------------------------------------------
template <class ComponentModell>
void BasicPopupMenuUI<ComponentModell>::installUI(AComponentPtr c) {
	
}
//------------------------------------------------------------------------------
template <class ComponentModell>
Dimension BasicPopupMenuUI<ComponentModell>::getMaximumSize(AComponentPtr c) {
	return getPreferredSize(c);
}
//-----------------------------------------------------------------------------
template <class ComponentModell>
Dimension BasicPopupMenuUI<ComponentModell>::getMinimumSize(AComponentPtr c) {
	return getPreferredSize(c);
}
//-----------------------------------------------------------------------------
template <class ComponentModell>
Dimension BasicPopupMenuUI<ComponentModell>::getPreferredSize(AComponentPtr c) {
	return NULL_DIMENSION;
}


///////////////////////////////////////////////////////////////////////////////
}}}}} // namespace(s)

#endif /* SAMBAG_BasicPopupMenuUI_H */

