/*
 * DiscoView.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: samba
 */

#include "DiscoView.hpp"
#include <sambag/com/AbstractType.hpp>
#include <sambag/disco/components/RootPane.hpp>
#include <sambag/disco/components/tests/TestComponents.hpp>
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <sambag/disco/components/events/MouseEventCreator.hpp>
#include <sambag/disco/components/RedrawManager.hpp>
#include <sambag/disco/components/FlowLayout.hpp>
#include <sambag/disco/components/ui/UIManager.hpp>
#include <sambag/disco/components/ui/basic/BasicLookAndFeel.hpp>
#include <sambag/disco/components/Button.hpp>
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/com/ICommand.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/timer/timer.hpp>
#include <assert.h>

static FpsCallback fpsCallback = NULL;

void registerFPSCallbackHandler(FpsCallback cllbk) {
	fpsCallback = cllbk;
}

sambag::disco::components::RootPane::Ptr root;
sambag::disco::Dimension dim(640, 480);
sambag::disco::components::events::MouseEventCreator::Ptr mev;
sambag::disco::IImageSurface::Ptr bff = sambag::disco::
		getDiscoFactory()->createImageSurface(dim.width(), dim.height());
sambag::disco::ISurface::Ptr display;

int disco_fps = 0;
int fcounter = 0;
boost::timer::cpu_timer fpsTimer;
boost::timer::nanosecond_type startTime;

static const boost::timer::nanosecond_type ONE_SEC_IN_NS(1000000000LL);

static inline boost::timer::nanosecond_type getTimerElapsed() {
	return fpsTimer.elapsed().user;
}

void calcFPS() {
	if (fpsTimer.is_stopped())
		fpsTimer.start();
	using boost::timer::nanosecond_type;

	nanosecond_type end(getTimerElapsed());
	nanosecond_type ns = end - startTime;

	if (ns>ONE_SEC_IN_NS) {
		// call observer
		if (fpsCallback)
			fpsCallback(fcounter);
		fcounter = 0;
		startTime=getTimerElapsed();
		return;
	}
	fcounter++;
}


void processDraw() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	static bool ini = true;
	IDrawContext::Ptr cn = getDiscoFactory()->createContext(display);
	if (ini) {
		root->draw(root->getDrawContext());
		ini=false;
		return;
	}
	RedrawManager::currentManager(root)->drawDirtyRegions();
	cn->drawSurface(bff);
}

void init(int &outWidth, int &outHeight) {
	using namespace sambag::disco;
	outWidth = dim.width();
	outHeight = dim.height();
}

struct ButtonCommand : public sambag::com::ICommand {
	typedef boost::shared_ptr<ButtonCommand> Ptr;
	int id;
	static Ptr create(int id) {
		return Ptr(new ButtonCommand(id));
	}
	virtual void execute() {
		std::cout<<id<<" pressed"<<std::endl;
	}
	ButtonCommand(int id) : id(id) {}
};

void createWindow(sambag::disco::ISurface::Ptr surf) {
	assert(surf);
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	using namespace sambag::disco::components::events;
	display = surf;
	root = RootPane::create(bff);
	ui::UIManager::instance().installLookAndFeel(root,
			ui::basic::BasicLookAndFeel::create());
	root->setSize(dim);
	// content
	tests::TestContainer::Ptr cn = tests::TestContainer::create();
	cn->setLayout( FlowLayout::create() );
	cn->setPreferredSize(Dimension(150,150));
	for (int i = 0; i < 10; ++i) {
		std::stringstream ss;
		ss << i;
		Button::Ptr btn = Button::create();
		btn->setText(ss.str());
		btn->setButtonCommand(ButtonCommand::create(i));
		cn->add(btn);
	}
	Button::Ptr btn = Button::create();
	btn->setText("#");
	btn->setButtonCommand(ButtonCommand::create(10));
	cn->add(btn);
	btn = Button::create();
	btn->setText("*");
	btn->setButtonCommand(ButtonCommand::create(11));
	cn->add(btn);

	btn = Button::create();
	btn->setText("do it! bitch");
	root->add(cn);
	root->add(btn);
	root->validate();
	root->setVisible(true);
	mev = sambag::disco::components::events::MouseEventCreator::create(root);
}

void handleMouseButtonPressEvent(int x, int y, int buttons) {
	mev->createPressEvent(x, y, buttons);
}
void handleMouseButtonReleaseEvent(int x, int y, int buttons) {
	mev->createReleaseEvent(x, y, buttons);
}
void handleMouseMotionEvent(int x, int y) {
	mev->createMoveEvent(x, y);
}

