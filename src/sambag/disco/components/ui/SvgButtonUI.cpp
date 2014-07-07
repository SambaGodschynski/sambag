#include "SvgButtonUI.hpp"
#include <sambag/disco/IDrawable.hpp>
#include <sambag/disco/svg/graphicElements/SceneGraph.hpp>
#include <sambag/disco/svg/SvgRoot.hpp>
#include <sambag/disco/components/ui/UIManager.hpp>
#include <exception>

namespace sambag { namespace disco {
namespace components { namespace ui {
//=============================================================================
// class SvgButtonUI : public AComponentUI
//=============================================================================
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SvgButtonUI::installButtonListeners() {
    getHandle()->com::events::EventSender<events::MouseEvent>::addTrackedEventListener(
		boost::bind(&SvgButtonUI::onMouse, this, _2),
		shared_from_this()
	);
    getIdle()->com::events::EventSender<events::MouseEvent>::addTrackedEventListener(
		boost::bind(&SvgButtonUI::onMouse, this, _2),
		shared_from_this()
	);
    getRollover()->com::events::EventSender<events::MouseEvent>::addTrackedEventListener(
		boost::bind(&SvgButtonUI::onMouse, this, _2),
		shared_from_this()
	);
    getPressed()->com::events::EventSender<events::MouseEvent>::addTrackedEventListener(
		boost::bind(&SvgButtonUI::onMouse, this, _2),
		shared_from_this()
	);
}
//-----------------------------------------------------------------------------
void SvgButtonUI::installModelListeners() {
    getModel()->com::events::EventSender<StateChanged>::addTrackedEventListener(
		boost::bind(&SvgButtonUI::onStateChanged, this, _2),
		shared_from_this()
	);
}
//-----------------------------------------------------------------------------
void SvgButtonUI::installListeners(AComponent::Ptr c) {
    installButtonListeners();
}
//-----------------------------------------------------------------------------
void SvgButtonUI::installUI(AComponentPtr c) {
    Super::installUI(c);
   	SvgComponent::Dummy::Ptr main =
        boost::dynamic_pointer_cast<SvgComponent::Dummy>(c);
    if (!main) {
        throw std::runtime_error("SvgButtonUI: wrong component");
    }
    this->main = main;
    installModel(main);
   	
    handle = getFirstChildOfClass(".disco-button-handle", main);
    idle = getFirstChildOfClass(".disco-button-handle-idle", getHandle());
    rollover = getFirstChildOfClass(".disco-button-handle-rollover", getHandle());
    pressed = getFirstChildOfClass(".disco-button-handle-pressed", getHandle());
    
    getRollover()->setVisible(false);
    getPressed()->setVisible(false);
    
    installListeners(c);
}
//-----------------------------------------------------------------------------
void SvgButtonUI::onMouse(const events::MouseEvent &ev) {
    enum { Filter = 
		events::MouseEvent::DISCO_MOUSE_PRESSED |
        events::MouseEvent::DISCO_MOUSE_RELEASED|
        events::MouseEvent::DISCO_MOUSE_ENTERED |
        events::MouseEvent::DISCO_MOUSE_EXITED
	};
	events::MouseEventSwitch<Filter>::delegate(ev, *this);
}
//-----------------------------------------------------------------------------
void SvgButtonUI::mousePressed(const events::MouseEvent &ev) {
    ModelPtr model = getModel();
	if (ev.getButtons() != events::MouseEvent::DISCO_BTN1)
		return;
	model->setButtonPressed(true);
}
//-----------------------------------------------------------------------------
void SvgButtonUI::mouseReleased(const events::MouseEvent &ev) {
	ModelPtr b = getModel();
	if (ev.getButtons() != events::MouseEvent::DISCO_BTN1)
			return;
	bool oldState = b->isButtonPressed();
	b->setButtonPressed(false);
	if (!oldState)
		return;
	if (!getHandle()->contains(ev.getLocation())) {// mouse moved out
		b->setButtonRollover(false);
		return;
	}
	const sambag::com::ICommand::Function &c =
			b->getButtonFunction();
	if (c) {
		c();
	}
	b->EventSender<events::ActionEvent>::notifyListeners(
			b.get(),
			events::ActionEvent(getHandle())
	);
}
//-----------------------------------------------------------------------------
void SvgButtonUI::onStateChanged(const StateChanged &ev) {
    AComponent::Ptr idle = getIdle(), pressed = getPressed(),
                    rollover = getRollover();
    
    
    ModelPtr model = getModel();
    if (model->isButtonPressed()) {
        pressed->setVisible(true);
        rollover->setVisible(false);
        idle->setVisible(false);
        return;
    }
    if (model->isButtonRollover()) {
        rollover->setVisible(true);
        idle->setVisible(false);
        pressed->setVisible(false);
        return;

    }
    idle->setVisible(true);
    pressed->setVisible(false);
    rollover->setVisible(false);

}
//-----------------------------------------------------------------------------
void SvgButtonUI::mouseEntered(const events::MouseEvent &ev) {
    ModelPtr model = getModel();
    model->setButtonRollover(true);
}
//-----------------------------------------------------------------------------
void SvgButtonUI::mouseExited(const events::MouseEvent &ev) {
    ModelPtr model = getModel();
	model->setButtonRollover(false);
	model->setButtonPressed(false);
}
}}}}