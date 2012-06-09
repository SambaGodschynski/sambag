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
#include <sambag/disco/windowImpl/X11Window.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/timer/timer.hpp>
#include <assert.h>

namespace sd = sambag::disco;
namespace sdc = sambag::disco::components;
void onMouse(void *src, const sdc::events::MouseEvent &ev) {
	std::cout<<ev.toString()<<std::endl;
}

sd::Window::Ptr win;
sd::Window::Ptr win2;

void onAhaClicked ( void *src, const sdc::events::ActionEvent &ac) {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	if (!win2) {
		win2 = sd::Window::create();
		win2->setBounds(Rectangle(0,0,230,200));
		win2->getRootPane()->setBounds(Rectangle(0,0,230,200));
		win2->getRootPane()->validate();
	}
	win2->open();
}
void onByeClicked ( void *src, const sdc::events::ActionEvent &ac) {
	if (win2)
		win2->close();
	//win2.reset();
}

int main() {
	{ // extra scope (bye message should occur after releasing all objs)
		using namespace sambag::disco;
		using namespace sambag::disco::components;
		win = sd::Window::create();
		win->setBounds(Rectangle(100,100,230,200));
		win->getRootPane()->setSize(230,200);
		win->getRootPane()->EventSender<sdc::events::MouseEvent>::
				addEventListener(&onMouse);
		//win->setTitle("Window01");

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
		X11WindowImpl::startMainLoop();
	}
	win.reset();
	std::cout<<"bye"<<std::endl;
}

