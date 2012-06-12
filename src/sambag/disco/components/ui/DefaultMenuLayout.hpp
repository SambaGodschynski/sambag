/*
 * DefaultMenuLayout.hpp
 *
 *  Created on: Mon Jun 11 22:13:11 2012
 *      Author: Johannes Unger
 */

#ifndef SAMBAG_DEFAULTMENULAYOUT_H
#define SAMBAG_DEFAULTMENULAYOUT_H

#include <boost/shared_ptr.hpp>
#include <sambag/disco/components/BoxLayout.hpp>
#include <sambag/disco/components/Forward.hpp>
namespace sambag { namespace disco { namespace components { namespace ui {
//=============================================================================
/** 
  * @class DefaultMenuLayout.
  */
class DefaultMenuLayout : public BoxLayout {
//=============================================================================
public:
	//-------------------------------------------------------------------------
	typedef boost::shared_ptr<DefaultMenuLayout> Ptr;
protected:
	//-------------------------------------------------------------------------
	DefaultMenuLayout(AContainerPtr target, Axis axis);
private:
public:
	//-------------------------------------------------------------------------
	static Ptr create(AContainerPtr target, Axis axis) {
		return Ptr(new DefaultMenuLayout(target, axis));
	}
	//-------------------------------------------------------------------------
	virtual Dimension preferredLayoutSize(AContainerPtr target) {
		/*if (target instanceof JPopupMenu) {
			JPopupMenu popupMenu = (JPopupMenu) target;
			sun.swing.MenuItemLayoutHelper.clearUsedClientProperties(popupMenu);
			if (popupMenu.getComponentCount() == 0) {
				return new Dimension(0, 0);
			}
		}*/

		// Make BoxLayout recalculate cached preferred sizes
		BoxLayout::invalidateLayout(target);
		return BoxLayout::preferredLayoutSize(target);
	}
}; // DefaultMenuLayout
}}}} // namespace(s)

#endif /* SAMBAG_DEFAULTMENULAYOUT_H */
