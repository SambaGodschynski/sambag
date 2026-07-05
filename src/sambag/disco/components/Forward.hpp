/*
 * Forward.hpp
 *
 *  Created on: May 21, 2012
 *      Author: samba
 */

#ifndef SAMBAG_DISCO_COMPONENTS_FORWARD_HPP_
#define SAMBAG_DISCO_COMPONENTS_FORWARD_HPP_

#include <memory>
#include <sambag/com/SharedOrWeak.hpp>

//=============================================================================
// Forward declarations of some important classes
//=============================================================================
namespace sambag { namespace disco { namespace components {
	class AComponent; typedef std::shared_ptr<AComponent> AComponentPtr;
					  typedef std::weak_ptr<AComponent> AComponentWPtr;
	class AContainer; typedef std::shared_ptr<AContainer> AContainerPtr;
					  typedef std::weak_ptr<AContainer> AContainerWPtr;
					  typedef sambag::com::SharedOrWeak<AComponent> AComponentSharedOrWeak;
	class ALayoutManager; typedef std::shared_ptr<ALayoutManager> ALayoutManagerPtr;
	class Panel; typedef std::shared_ptr<Panel> PanelPtr;
	class RootPane; typedef std::shared_ptr<RootPane> RootPanePtr;
	class Scrollbar; typedef std::shared_ptr<Scrollbar> ScrollbarPtr;
	class ScrollPane; typedef std::shared_ptr<ScrollPane> ScrollPanePtr;
					  typedef std::weak_ptr<ScrollPane> ScrollPaneWPtr;
	class Button; typedef std::shared_ptr<Button> ButtonPtr;
					  typedef std::weak_ptr<Button> ButtonWPtr;
	class Label; typedef std::shared_ptr<Label> LabelPtr;
					  typedef std::weak_ptr<Label> LabelWPtr;
	class Viewport; typedef std::shared_ptr<Viewport> ViewportPtr;
	class RedrawManager; typedef std::shared_ptr<RedrawManager> RedrawManagerPtr;
	class AWindowImpl; typedef std::shared_ptr<AWindowImpl> AWindowImplPtr;
	class Window; typedef std::shared_ptr<Window> WindowPtr;
	class Window; typedef std::weak_ptr<Window> WindowWPtr;
	class FramedWindow; typedef std::shared_ptr<FramedWindow> FramedWindowPtr;
	class PopupMenu; typedef std::shared_ptr<PopupMenu> PopupMenuPtr;
	class MenuSelectionManager;
	class Knob; typedef std::shared_ptr<Knob> KnobPtr;
namespace ui {
	class UIManager;
	class AComponentUI; typedef std::shared_ptr<AComponentUI> AComponentUIPtr;
	class ALookAndFeel; typedef std::shared_ptr<ALookAndFeel> ALookAndFeelPtr;
}
}}}

#endif /* SAMBAG_DISCO_COMPONENTS_FORWARD_HPP_ */
