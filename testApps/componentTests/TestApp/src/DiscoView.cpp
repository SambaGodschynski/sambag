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
#include <sambag/disco/components/Menu.hpp>
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>
#include <sambag/disco/components/Window.hpp>
#include <sambag/disco/components/FramedWindow.hpp>
#include <sambag/disco/components/BoxLayout.hpp>
#include <sambag/disco/components/Panel.hpp>
#include <sambag/disco/components/PopupMenu.hpp>
#include <sambag/disco/components/Label.hpp>
#include <sambag/disco/components/MenuSelectionManager.hpp>
#include <sambag/disco/components/Viewport.hpp>
#include <sambag/com/ICommand.hpp>
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
void stopTimer(void *src, const sdc::events::ActionEvent &ev);
void startTimer(void *src, const sdc::events::ActionEvent &ev);
void onAcmeMouse(void *src, const sdc::events::MouseEvent &ev);
void onSurpriseMouse(void *src, const sdc::events::MouseEvent &ev);


sdc::FramedWindow::Ptr win;
sdc::FramedWindow::Ptr win2;
sdc::FramedWindow::Ptr win3;
sdc::PopupMenu::Ptr acmePopup;
sdc::PopupMenu::Ptr surprisePopup;
sdc::Timer::Ptr timerInf;

static const std::string INPUT_LABEL = "inputLabel";
static const int INPUT_LABEL_SIZE = 40;
static const float FFF = 10.f;
static const float FF = 5.f;
static const float F = 1.f;

float currScrollSpeed = F;

void createAcmePopup() {
	using namespace sambag::disco::components;
	acmePopup = PopupMenu::create(win2->getRootPane());
	const std::string CLEAR = "clear all";
	const std::string items[] = {"select all", "find sibling",
			"but I say immer", "helter selter", "add nachwuk banst...", CLEAR};
	const int NUM = sizeof(items) / sizeof(items[0]);
	for (int i = 0; i < NUM; ++i) {
		MenuItem::Ptr btn = MenuItem::create();
		btn->setText(items[i]);
		acmePopup->add(btn);
		if (items[i] != CLEAR)
			continue;
		btn->EventSender<events::ActionEvent>::addEventListener(&onClearTxtField);
	}

	MenuItem::Ptr btn = MenuItem::create();
	btn->setText("start Timer");
	acmePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(&startTimer);

	btn = MenuItem::create();
	btn->setText("stop Timer");
	acmePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(&stopTimer);


	Menu::Ptr menu = Menu::create();
	menu->setText("feuer feuer");
	for (size_t i=0; i<10; ++i) {
		std::stringstream ss;
		ss << "actione nummere " << i;
		MenuItem::Ptr btn = MenuItem::create();
		btn->setText(ss.str());
		menu->add(btn);
	}
	acmePopup->add(menu);

	menu = Menu::create();
	menu->setText("wasser");
	for (size_t i=0; i<5; ++i) {
		std::stringstream ss;
		ss << "römer gehen in das:" << i;
		MenuItem::Ptr btn = MenuItem::create();
		btn->setText(ss.str());
		menu->add(btn);
	}
	Menu::Ptr subsub = Menu::create();
	subsub->setText("holy anker");
	for (size_t i=0; i<5; ++i) {
		std::stringstream ss;
		ss << i;
		MenuItem::Ptr btn = MenuItem::create();
		btn->setText(ss.str());
		subsub->add(btn);
	}
	menu->add(subsub);
	acmePopup->add(menu);
}

void setScrolling(void *src, const sdc::events::ActionEvent &ev, float scrolling) {
	currScrollSpeed = scrolling;
}

void createSurprisePopup() {
	using namespace sambag::disco::components;
	surprisePopup = PopupMenu::create(win3->getRootPane());
	MenuItem::Ptr btn = MenuItem::create();
	btn->setText("---");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, -FFF)
	);
	btn = MenuItem::create();
	btn->setText("--");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, -FF)
	);
	btn = MenuItem::create();
	btn->setText("-");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, -F)
	);
	btn = MenuItem::create();
	btn->setText("0");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, 0.f)
	);
	btn = MenuItem::create();
	btn->setText("+");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, F)
	);
	btn = MenuItem::create();
	btn->setText("++");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, FF)
	);
	btn = MenuItem::create();
	btn->setText("+++");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, FFF)
	);
}

boost::weak_ptr<sdc::Viewport> viewport;

void scrollDown(void *src, const sdc::TimerEvent &ev) {
	if (currScrollSpeed == 0.f)
		return;
	sdc::Viewport::Ptr vp = viewport.lock();
	if (!vp)
		return;
	sd::Point2D p = vp->getViewPosition();
	p.y( p.y() + currScrollSpeed );
	vp->setViewPosition(p);
}

void createSurpriseWindow() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	win3 = sdc::FramedWindow::create(win);
	win3->setTitle("Surprise Window");
	win3->setWindowBounds(Rectangle(110,100,430,280));

	const int NUM = 1000;
	AContainerPtr con = Panel::create();

	con->EventSender<sdc::events::MouseEvent>::
			addEventListener(&onSurpriseMouse);

	con->EventSender<sdc::events::MouseEvent>::
			addEventListener(&trackMouse);

	con->setSize(Dimension(300, 1000));
	sdc::Viewport::Ptr vp = Viewport::create();
	viewport = vp;
	vp->setView(con);
	//vp->setScrollMode(Viewport::SIMPLE_SCROLL_MODE);
	//vp->setScrollMode(Viewport::BACKINGSTORE_SCROLL_MODE);
	con->setLayout(BoxLayout::create(con, BoxLayout::Y_AXIS));
	//con->setLayout(ALayoutManagerPtr());
	win3->getRootPane()->add(vp);
	for (int i=0; i<NUM; ++i) {
		std::stringstream ss;
		ss << i;
		Label::Ptr el = Label::create();
		el->setText(ss.str());
		el->setFont(el->getFont().setSize((i+1)*0.9));
		con->add(el);
	}
	Timer::Ptr timer = Timer::create(10);
	timer->setNumRepetitions(-1);
	timer->EventSender<TimerEvent>::addTrackedEventListener(
		&scrollDown,
		viewport
	);
	timer->start();
}

void createACMEWindow() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	win2 = sdc::FramedWindow::create(win);
	win2->setTitle("ACME Window");
	win2->setWindowBounds(Rectangle(110,100,430,280));

	win2->getRootPane()->EventSender<sdc::events::MouseEvent>::
			addEventListener(&onAcmeMouse);
	win2->getRootPane()->EventSender<sdc::events::MouseEvent>::
			addEventListener(&trackMouse);

	RootPanePtr con = win2->getRootPane();
	for (int i = 0; i < 10; ++i) {
		std::stringstream ss;
		ss << i;
		Button::Ptr btn = Button::create();
		btn->EventSender<sdc::events::ActionEvent>::
				addEventListener(&onButton);
		btn->EventSender<sdc::events::MouseEvent>::
				addEventListener(&trackMouse);
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


void stopTimer(void *src, const sdc::events::ActionEvent &ev) {
	if (timerInf)
		timerInf->stop();
}
void startTimer(void *src, const sdc::events::ActionEvent &ev) {
	if (timerInf)
		timerInf->start();
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
	using namespace sambag::disco::components;
	using namespace sambag::disco;
	//std::cout<<ev.toString()<<std::endl;
	Point2D scr = ev.getSource()->getLocationOnScreen(ev.getLocation());
	Point2D comp = ev.getSource()->getLocationOnComponent(scr);
	std::cout<<ev.getSource()->toString()<<" | "<<scr<<" / "<<comp<<std::endl;
}

void handlePopupMouse(sdc::PopupMenuPtr popup, const sdc::events::MouseEvent &ev) {
	using namespace sambag::disco::components;
	static PopupMenuPtr currPopup;
	if (ev.getType() != events::MouseEvent::MOUSE_CLICKED)
		return;
	if (ev.getButtons() != sdc::events::MouseEvent::BUTTON2) {
		if (currPopup)
			currPopup->hidePopup();
		return;
	}
	if (!popup->isPopupVisible()) {
		MenuSelectionManager &m = MenuSelectionManager::defaultManager();
		m.clearSelectedPath();
		IMenuElement::MenuElements p;
		p.push_back(popup);
		m.setSelectedPath(p);
		popup->showPopup(
			ev.getLocationOnScreen()
		);
	}
	currPopup = popup;
}

void onAcmeMouse(void *src, const sdc::events::MouseEvent &ev) {
	if (!acmePopup) {
		createAcmePopup();
	}
	handlePopupMouse(acmePopup, ev);
}

void onSurpriseMouse(void *src, const sdc::events::MouseEvent &ev) {
	if (!surprisePopup) {
		createSurprisePopup();
	}
	handlePopupMouse(surprisePopup, ev);
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
		createACMEWindow();
	}
	win2->getRootPane()->validate();
	win2->open();
}

void onSurpriseClicked ( void *src, const sdc::events::ActionEvent &ac) {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	if (!win3) {
		createSurpriseWindow();
	}
	win3->getRootPane()->validate();
	win3->open();
}

void onByeClicked ( void *src, const sdc::events::ActionEvent &ac) {
	if (win)
		win->close();
}

void onMainClose(void*, const sdc::OnCloseEvent &ev) {
	if (win2)
		win2->close();
}

void pathChanged(void *src, const sdc::MenuSelectionManagerChanged &ev) {
	std::cout<<ev.getSrc().getSelectedPath()<<std::endl;
}

void timedCallbackInf(void *src, const sdc::TimerEvent &ev) {
	static int i=0;
	std::cout<<"timed callbackInf(" << ++i << ")" << std::flush << std::endl;
}

void timedCallback5(void *src, const sdc::TimerEvent &ev) {
	static int i=0;
	std::cout<<"timed callback5(" << ++i << ")" << std::flush << std::endl;
}

void timedCallbackOnce(void *src, const sdc::TimerEvent &ev) {
	std::cout<<"timed once" << std::flush << std::endl;
}

void initTimer() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	timerInf = Timer::create(10000);
	timerInf->setNumRepetitions(-1);
	timerInf->EventSender<TimerEvent>::addEventListener(&timedCallbackInf);
	timerInf->start();

	Timer::Ptr timer2 = Timer::create(1000);
	timer2->setNumRepetitions(4);
	timer2->EventSender<TimerEvent>::addEventListener(&timedCallback5);
	timer2->start();

	Timer::Ptr timer3 = Timer::create(500);
	timer3->EventSender<TimerEvent>::addEventListener(&timedCallbackOnce);
	timer3->start();
}

int main() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	std::cout<<"hi"<<std::endl;
	std::cout<<getWindowToolkit()->getScreenSize()<<std::endl;
	initTimer();
	{ // extra scope (bye message should occur after releasing all objs)
		win = sdc::FramedWindow::create();
		win->setWindowBounds(Rectangle(100,100,230,200));
		win->setTitle("Messerschmitz 1.0");

		Button::Ptr btn = Button::create();
		btn->setText("open ACME Panel");
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onAhaClicked);
		btn->getFont().setFontFace("monospace");
		win->getRootPane()->add(btn);

		btn = Button::create();
		btn->setText("open Surprise Panel");
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onSurpriseClicked);
		btn->getFont().setFontFace("monospace");
		win->getRootPane()->add(btn);

		btn = Button::create();
		btn->setText("tschüß");
		btn->getFont().setFontFace("monospace").setSize(50);
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onByeClicked);
		win->getRootPane()->add(btn);

		win->validate();
		win->pack();
		win->open();

		MenuSelectionManager::defaultManager().
		EventSender<MenuSelectionManagerChanged>::addEventListener
			( &pathChanged );

		sdc::Window::startMainLoop();
	}
	win.reset();
	std::cout<<"bye"<<std::endl;
}

