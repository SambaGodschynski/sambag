/*
 * Forward.hpp
 *
 *  Created on: May 21, 2012
 *      Author: samba
 */

#ifndef FORWARD_HPP_
#define FORWARD_HPP_

#include <boost/shared_ptr.hpp>

//=============================================================================
// Forward declarations of some important classes
//=============================================================================
namespace sambag { namespace disco { namespace components {
	class AComponent; typedef boost::shared_ptr<AComponent> AComponentPtr;
	class AContainer; typedef boost::shared_ptr<AContainer> AContainerPtr;
	class ALayoutManager; typedef boost::shared_ptr<ALayoutManager> ALayoutManagerPtr;
	class Panel; typedef boost::shared_ptr<Panel> PanelPtr;
	class RootPane; typedef boost::shared_ptr<RootPane> RootPanePtr;
	class Scrollbar; typedef boost::shared_ptr<Scrollbar> ScrollbarPtr;
	class ScrollPane; typedef boost::shared_ptr<ScrollPane> ScrollPanePtr;
	class Viewport; typedef boost::shared_ptr<Viewport> ViewportPtr;
	class RedrawManager; typedef boost::shared_ptr<RedrawManager> RedrawManagerPtr;
	class AWindowImpl; typedef boost::shared_ptr<AWindowImpl> AWindowPtr;
	class Window; typedef boost::shared_ptr<Window> WindowPtr;
	class FramedWindow; typedef boost::shared_ptr<FramedWindow> FramedWindowPtr;
	class PopupMenu; typedef boost::shared_ptr<PopupMenu> PopupMenuPtr;
	class MenuSelectionManager;
namespace ui {
	class UIManager;
	class AComponentUI; typedef boost::shared_ptr<AComponentUI> AComponentUIPtr;
	class ALookAndFeel; typedef boost::shared_ptr<ALookAndFeel> ALookAndFeelPtr;
}
}}}

#endif /* FORWARD_HPP_ */
