/*
 * DiscoView.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: samba
 */

#include "DiscoView.hpp"
#include <sambag/disco/components/RootPane.hpp>
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <sambag/disco/components/RedrawManager.hpp>
#include <sambag/disco/components/Button.hpp>
#include <sambag/disco/components/MenuItem.hpp>
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/components/IWindowFactory.hpp>
#include <sambag/disco/components/Window.hpp>
#include <sambag/disco/components/FramedWindow.hpp>
#include <sambag/disco/components/windowImpl/X11Window.hpp>
#include <sambag/disco/components/BoxLayout.hpp>
#include <sambag/disco/components/Panel.hpp>
#include <sambag/disco/components/PopupMenu.hpp>
#include <sambag/disco/components/Label.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/timer/timer.hpp>
#include <assert.h>

namespace sd = sambag::disco;
namespace sdc = sambag::disco::components;

void trackMouse(void *src, const sdc::events::MouseEvent &ev);
void onMouse(void *src, const sdc::events::MouseEvent &ev);
void onButton(void *src, const sdc::events::ActionEvent &ev);
void onClearTxtField(void *src, const sdc::events::ActionEvent &ev);


sdc::FramedWindow::Ptr win;
sdc::FramedWindow::Ptr win2;
sdc::PopupMenu::Ptr popup;

static const std::string INPUT_LABEL = "inputLabel";
static const int INPUT_LABEL_SIZE = 40;

void createPopup() {
	using namespace sambag::disco::components;
	popup = PopupMenu::create(win2);
	const std::string CLEAR = "clear all";
	const std::string items[] = {"select all", "find sibling",
			"but I say immer", "helter selter", "add nachwuk banst...", CLEAR};
	const int NUM = sizeof(items) / sizeof(items[0]);
	for (int i = 0; i < NUM; ++i) {
		MenuItem::Ptr btn = MenuItem::create();
		btn->setText(items[i]);
		popup->add(btn);
		if (items[i] != CLEAR)
			continue;
		btn->EventSender<events::ActionEvent>::addEventListener(&onClearTxtField);
	}
}

void createACMEPane() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	win2 = sdc::FramedWindow::create(win);
	win2->setWindowBounds(Rectangle(110,100,430,280));

	win2->getRootPane()->EventSender<sdc::events::MouseEvent>::
			addEventListener(&onMouse);
	win2->getRootPane()->EventSender<sdc::events::MouseEvent>::
			addEventListener(&trackMouse);

	RootPanePtr con = win2->getRootPane();
	for (int i = 0; i < 10; ++i) {
		std::stringstream ss;
		ss << i;
		Button::Ptr btn = Button::create();
		btn->EventSender<sdc::events::ActionEvent>::
				addEventListener(&onButton);
		btn->setText(ss.str());
		btn->getFont().setFontFace("monospace");
		con->add(btn);
	}
	for (int i = 0; i < 26; ++i) {
		char c[] = {'A' + i, 0};
		Button::Ptr btn = Button::create();
		btn->EventSender<sdc::events::ActionEvent>::
				addEventListener(&onButton);
		btn->setText(std::string(c));
		btn->getFont().setFontFace("monospace");
		con->add(btn);
	}
	Label::Ptr label = Label::create();
	label->getFont().setFontFace("monospace").setSize(INPUT_LABEL_SIZE);
	con->add(label);
	con->addTag(label, INPUT_LABEL);
}

void onClearTxtField(void *src, const sdc::events::ActionEvent &ev) {
	using namespace sambag::disco::components;
	std::list<AComponentPtr> l;
	win2->getRootPane()->getComponentsByTag(INPUT_LABEL, l);
	if (l.empty())
		return;
	Label::Ptr label = boost::shared_dynamic_cast<Label>(l.back());
	if (!label)
		return;
	label->setText("");
	win2->getRootPane()->validate();
	label->getFont().setSize(INPUT_LABEL_SIZE);
}

void trackMouse(void *src, const sdc::events::MouseEvent &ev) {
	std::cout<<ev.toString()<<std::endl;
}

void onMouse(void *src, const sdc::events::MouseEvent &ev) {
	using namespace sambag::disco::components;
	if (ev.getType() != events::MouseEvent::MOUSE_CLICKED)
		return;
	if (ev.getButtons() != sdc::events::MouseEvent::BUTTON2) {
		if (popup)
			popup->hidePopup();
		return;
	}
	if (!popup) {
		createPopup();
	}
	if (!popup->isPopupVisible()) {
		popup->showPopup(
			win2->getRootPane()->getLocationOnScreen(ev.getLocation())
		);
	}
}

void onButton(void *src, const sdc::events::ActionEvent &ev) {
	using namespace sambag::disco::components;
	Button::Ptr b = boost::shared_dynamic_cast<Button>(ev.getSource());
	if (!b) {
		std::cout<<ev.getSource()->toString()<<std::endl;
		return;
	}
	std::list<AComponentPtr> l;
	win2->getRootPane()->getComponentsByTag(INPUT_LABEL, l);
	if (l.empty())
		return;
	Label::Ptr label = boost::shared_dynamic_cast<Label>(l.back());
	if (!label)
		return;
	std::string txt = label->getText() + b->getText();
	label->setText(txt);
	if (txt.length() > 15) {
		label->getFont().setSize( (15 / (float)txt.length()) * INPUT_LABEL_SIZE );
	}
	win2->getRootPane()->validate();
	win2->getRootPane()->redraw();
}

void onAhaClicked ( void *src, const sdc::events::ActionEvent &ac) {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	if (!win2) {
		createACMEPane();
	}
	win2->getRootPane()->validate();
	win2->open();
}

void onByeClicked ( void *src, const sdc::events::ActionEvent &ac) {
	if (win)
		win->close();
}

void onMainClose(void*, const sdc::OnCloseEvent &ev) {
	if (win2)
		win2->close();
}

int main() {
	std::cout<<"hi"<<std::endl;
	{ // extra scope (bye message should occur after releasing all objs)
		using namespace sambag::disco;
		using namespace sambag::disco::components;
		win = sdc::FramedWindow::create();
		win->setWindowBounds(Rectangle(100,100,230,200));
		win->setTitle("Messerschmitz");

		Button::Ptr btn = Button::create();
		btn->setText("hinzufügen und belassen");
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onAhaClicked);
		btn->getFont().setFontFace("monospace");
		win->getRootPane()->add(btn);

		btn = Button::create();
		btn->setText("tschüß");
		btn->getFont().setFontFace("monospace").setSize(50);
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onByeClicked);
		win->getRootPane()->add(btn);

		win->getRootPane()->validate();
		win->pack();
		win->open();
		sdc::Window::startMainLoop();
	}
	win.reset();
	std::cout<<"bye"<<std::endl;
}

