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
#include <sambag/disco/IDiscoFactory.hpp>
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

void onMouse(void *, const sambag::disco::components::events::MouseEvent &ev) {
	using namespace std;
	using namespace sambag::disco;
	cout<<ev.toString()<<endl;
	if (ev.getType() == ev.MOUSE_ENTERED) {
		if (ev.getSource() != root)
			ev.getSource()->setBackground(ColorRGBA(1));
			ev.getSource()->redraw();
	}
	if (ev.getType() == ev.MOUSE_EXITED || ev.getType() == ev.MOUSE_RELEASED) {
		if (ev.getSource() != root)
			ev.getSource()->setBackground(ColorRGBA(0));
			ev.getSource()->redraw();
	}
	if (ev.getType() == ev.MOUSE_CLICKED) {
		if (ev.getSource() != root)
			ev.getSource()->setBackground(ColorRGBA(1,1));
			ev.getSource()->redraw();
	}
}

void createWindow(sambag::disco::ISurface::Ptr surf) {
	assert(surf);
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	using namespace sambag::disco::components::events;
	display = surf;
	root = RootPane::create(bff);
	root->setSize(dim);
	root->EventSender<MouseEvent>::addEventListener(&onMouse);
	enum {NUM_COMPOS=25};
	AComponent::Ptr comps[NUM_COMPOS];
	// init components
	int i = 0;
	for ( ; i < 13; ++i) {
		std::stringstream ss;
		ss << "TestComponent" << i;
		comps[i] = tests::TestComponent::create();
		comps[i]->setName(ss.str());
		comps[i]->setBounds(Rectangle(0,0,50 + i*10, 55 + i*10));
		comps[i]->setBackground(ColorRGBA(0,0,0,1));
		comps[i]->setForeground(ColorRGBA(1,1,1,1));
		comps[i]->EventSender<MouseEvent>::addEventListener(&onMouse);
		comps[i]->putClientProperty("index", sambag::com::createObject(i));
		root->add(comps[i]);
	}
	AContainer::Ptr con = tests::TestContainer::create();
	con->setName("TestSubContaine");
	con->setLayout( FlowLayout::create() );
	con->setPreferredSize(Dimension(150, 100));
	root->add(con);
	for (; i < NUM_COMPOS; ++i) {
		std::stringstream ss;
		ss << "TestComponent" << i;
		comps[i] = tests::TestComponent::create();
		comps[i]->setName(ss.str());
		comps[i]->setBounds(Rectangle(0,0,30, 30));
		comps[i]->setBackground(ColorRGBA(0,0,0,1));
		comps[i]->setForeground(ColorRGBA(1,1,1,1));
		comps[i]->EventSender<MouseEvent>::addEventListener(&onMouse);
		comps[i]->putClientProperty("index", sambag::com::createObject(i));
		con->add(comps[i]);
	}
	root->validate();
	root->setVisible(true);
	mev = sambag::disco::components::events::MouseEventCreator::create(root);
}

void handleMouseButtonPressEvent(int x, int y, int buttons) {
	mev->createClickEvent(x, y, buttons);
}
void handleMouseButtonReleaseEvent(int x, int y, int buttons) {
	mev->createReleaseEvent(x, y, buttons);
}
void handleMouseMotionEvent(int x, int y) {
	mev->createMoveEvent(x, y);
}

