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
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/IWindowFactory.hpp>
#include <sambag/disco/Window.hpp>
#include <sambag/disco/FramedWindow.hpp>
#include <sambag/disco/windowImpl/X11Window.hpp>
#include <sambag/disco/components/BoxLayout.hpp>
#include <sambag/disco/components/Panel.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/timer/timer.hpp>
#include <assert.h>

namespace sd = sambag::disco;
namespace sdc = sambag::disco::components;
void onMouse(void *src, const sdc::events::MouseEvent &ev) {
	std::cout<<ev.toString()<<std::endl;
}

sd::FramedWindow::Ptr win;
sd::FramedWindow::Ptr win2;

void onAhaClicked ( void *src, const sdc::events::ActionEvent &ac) {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	if (!win2) {
		win2 = sd::FramedWindow::create();
		win2->setBounds(Rectangle(110,110,230,200));

		AContainer::Ptr c = Panel::create();
		c->setLayout(
				BoxLayout::create(c, BoxLayout::Y_AXIS)
		);
		for (int i = 0; i < 10; ++i) {
			std::stringstream ss;
			ss << i + 1;
			Button::Ptr btn = Button::create();
			btn->setText(ss.str());
			c->add(btn);
		}
		win2->getRootPane()->add(c);
		///////////////////////////////////////////////////////////////////////
		c = Panel::create();
		c->setLayout(
				BoxLayout::create(c, BoxLayout::X_AXIS)
		);
		for (int i = 0; i < 10; ++i) {
			std::stringstream ss;
			char txt[] = {'a' + i, '\0'};
			ss << txt;
			Button::Ptr btn = Button::create();
			btn->setText(ss.str());
			c->add(btn);
		}
		win2->getRootPane()->add(c);

		win2->getRootPane()->validate();
	}
	win2->open();
}

void onByeClicked ( void *src, const sdc::events::ActionEvent &ac) {
	if (win2)
		win2->close();
	//win2.reset();
}

void onMainClose(void*, const sd::OnCloseEvent &ev) {
	if (win2)
		win2->close();
}

int main() {
	std::cout<<"hi"<<std::endl;
	{ // extra scope (bye message should occur after releasing all objs)
		using namespace sambag::disco;
		using namespace sambag::disco::components;
		win = sd::FramedWindow::create();
		win->setBounds(Rectangle(100,100,230,200));
		win->getRootPane()->EventSender<sdc::events::MouseEvent>::
				addEventListener(&onMouse);
		win->addOnCloseEventListener(&onMainClose);
		win->setTitle("Messerschmitz");

		Button::Ptr btn = Button::create();
		btn->setText("hinzufügen und belassen");
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onAhaClicked);
		win->getRootPane()->add(btn);

		btn = Button::create();
		btn->setText("tschüß");
		sambag::disco::Font f = btn->getFont();
		//f.fontFace = "monospaced";
		f.size = 50;
		btn->setFont(f);
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onByeClicked);
		win->getRootPane()->add(btn);

		win->getRootPane()->validate();
		win->open();
		sambag::disco::Window::startMainLoop();
	}
	win.reset();
	std::cout<<"bye"<<std::endl;
}

