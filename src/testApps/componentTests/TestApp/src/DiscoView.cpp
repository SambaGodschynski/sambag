/*
 * DiscoView.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: samba
 */

#include "DiscoView.hpp"
#include <sambag/disco/svg/SvgRoot.hpp>
#include <sambag/disco/svg/HtmlColors.hpp>

#include <sambag/disco/components/RootPane.hpp>
#include <sambag/disco/components/events/MouseEvent.hpp>
#include <sambag/disco/components/RedrawManager.hpp>
#include <sambag/disco/components/Button.hpp>
#include <sambag/disco/components/CheckBox.hpp>
#include <sambag/disco/components/Menu.hpp>
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>
#include <sambag/disco/components/Window.hpp>
#include <sambag/disco/components/FramedWindow.hpp>
#include <sambag/disco/components/BoxLayout.hpp>
#include <sambag/disco/components/Panel.hpp>
#include <sambag/disco/components/PopupMenu.hpp>
#include <sambag/disco/components/Label.hpp>
#include <sambag/disco/components/List.hpp>
#include <sambag/disco/components/MenuSelectionManager.hpp>
#include <sambag/disco/components/Viewport.hpp>
#include <sambag/disco/components/Scrollbar.hpp>
#include <sambag/disco/components/GridLayout.hpp>
#include <sambag/disco/components/ScrollPane.hpp>
#include <sambag/disco/components/ColumnBrowser.hpp>
#include <sambag/disco/components/TitledBorder.hpp>
#include <sambag/disco/components/SvgComponent.hpp>
#include <sambag/disco/components/ui/basic/BasicButtonUI.hpp>
#include <sambag/com/ICommand.hpp>
#include <sambag/disco/FileResourceManager.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/timer/timer.hpp>
#include <boost/filesystem.hpp>
#include <assert.h>
#include <sambag/disco/components/events/MouseEventRecorder.hpp>


#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.VC90.CRT' version='9.0.21022.8' processorArchitecture='X86' publicKeyToken='1fc8b3b9a1e18e3b' language='*'\"")

enum { /*Views*/
	ACME = 1,
	SURPRISE,
	BORDERLINE,
	SCROLLERCOASTER,
	LIST,
	MILLER,
    SVG,
	NUM_VIEWS
};

#ifdef WIN32
#include <crtdbg.h>
#define SAMBAG_WINONLY(x) x
#else
#define SAMBAG_WINONLY(x)
#endif

namespace sd = sambag::disco;
namespace sdc = sambag::disco::components;

void trackMouse(void *src, const sdc::events::MouseEvent &ev);
void onRootMouse(void *src, const sdc::events::MouseEvent &ev);
void onACMEButton(void *src, const sdc::events::ActionEvent &ev);
void onClearTxtField(void *src, const sdc::events::ActionEvent &ev);
void stopTimer(void *src, const sdc::events::ActionEvent &ev);
void startTimer(void *src, const sdc::events::ActionEvent &ev);


std::vector<sdc::FramedWindow::Ptr> win =
		std::vector<sdc::FramedWindow::Ptr>(NUM_VIEWS);

sdc::Timer::Ptr timerInf;

static const std::string INPUT_LABEL = "inputLabel";
static const int INPUT_LABEL_SIZE = 40;
static const float FFFF = 5.f;
static const float FFF = 1.f;
static const float FF = .5f;
static const float F = .1f;

float currScrollSpeed = F;

sdc::PopupMenuPtr createAcmePopup() {
	using namespace sambag::disco::components;
	PopupMenuPtr acmePopup = PopupMenu::create();
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
	return acmePopup;
}

void setScrolling(void *src, const sdc::events::ActionEvent &ev, float scrolling) {
	if (scrolling==0)
		currScrollSpeed = 0;
	currScrollSpeed += scrolling;
}

sdc::PopupMenuPtr createSurprisePopup() {
	using namespace sambag::disco::components;
	PopupMenuPtr surprisePopup = PopupMenu::create();
	//
	MenuItem::Ptr btn = MenuItem::create();
	btn->setText("----");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, -FFFF)
	);
	//
	btn = MenuItem::create();
	btn->setText("---");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, -FFF)
	);
	//
	btn = MenuItem::create();
	btn->setText("--");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, -FF)
	);
	//
	btn = MenuItem::create();
	btn->setText("-");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, -F)
	);
	//
	btn = MenuItem::create();
	btn->setText("0");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, 0.f)
	);
	//
	btn = MenuItem::create();
	btn->setText("+");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, F)
	);
	//
	btn = MenuItem::create();
	btn->setText("++");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, FF)
	);
	//
	btn = MenuItem::create();
	btn->setText("+++");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, FFF)
	);
	//
	btn = MenuItem::create();
	btn->setText("++++");
	surprisePopup->add(btn);
	btn->EventSender<events::ActionEvent>::addEventListener(
		boost::bind(&setScrolling, _1, _2, FFFF)
	);
	return surprisePopup;
}

template <int View>
void createWindow() {}

template <>
void createWindow<BORDERLINE>() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	win[BORDERLINE] = sdc::FramedWindow::create(win[0]);
	win[BORDERLINE]->setTitle("Borderline Window");
	win[BORDERLINE]->setWindowBounds(sambag::disco::Rectangle(110,100,430,280));
	AContainerPtr c = Panel::create();
	Scrollbar::Ptr scr = Scrollbar::create(
			Scrollbar::HORIZONTAL, 0, 0, 0, 100
	);
	scr->setPreferredSize(Dimension(250, 25));
	c->setSize(Dimension(300,300));
	c->add(scr);

	scr = Scrollbar::create(Scrollbar::VERTICAL, 50, 0, 0, 100);
	scr->setPreferredSize(Dimension(20, 250));
	c->add(scr);

	win[BORDERLINE]->getContentPane()->add(c);
}


sdc::AContainerPtr createList() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;

	StringList::Ptr list = StringList::create();
	ScrollPane::Ptr scroll = ScrollPane::create(list);
	list->setFont(list->getFont().setSize(14));

	std::string max = "";
	for (int i=0; i<10000; ++i) {
		std::stringstream ss;
		ss<<"nomber "<<i;
		std::string str = ss.str();
		if (max.length() < str.length())
				max = str;
		list->addElement(str);
	}
	//list->setFixedCellWidth(400.);
	//list->setPrototypeCellValue("0123456789012345");
	return scroll;
}

template <>
void createWindow<LIST>() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	win[LIST] = sdc::FramedWindow::create(win[0]);
	win[LIST]->setTitle("L.I.S.T.");
	win[LIST]->setWindowBounds(sambag::disco::Rectangle(110,100,430,280));
	Panel::Ptr pane = Panel::create();
	pane->setLayout(GridLayout::create(0,1));
	pane->add(createList());
	win[LIST]->getContentPane()->add(pane);
}

typedef sdc::ColumnBrowser<std::string> Browser;

void createContent(Browser::Ptr miller) {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	using namespace boost::filesystem;
	typedef Browser::ModelDataType Content;
	directory_iterator end_it;
	directory_iterator it("/");
	for ( ; it!=end_it; ++it ) {
		std::string name = it->path().filename().string();
		if ( is_directory ( *it ) ) {
			Browser::Node n = miller->addNode(miller->getRootNode(), name);
			miller->addNode(n, "<empty>");

		} else {
			miller->addNode(miller->getRootNode(), name);
		}
	} //for
	miller->updateLists();
}

void updateContent(Browser::Ptr miller, const Browser::Path &path) {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	using namespace boost::filesystem;
	if ( !miller->isFolder(path.back()) ) {
		return;
	}
	std::string loc = "/" + miller->pathToString(path); 
	if (!exists(loc))
		return;
	Browser::Node branch = path.back();
	std::list<Browser::Node> nodes;
	miller->getChildren(branch, nodes);
	if (nodes.size() > 1) { // content allready set
		return;
	}
	if (miller->getNodeData(nodes.back())!="<empty>") { // content allready set
		return;
	}
	try {
		directory_iterator end_it;
		directory_iterator it(loc);
		for ( ; it!=end_it; ++it ) {
			Browser::Node n;
			std::string name = it->path().filename().string();
			if (!nodes.empty()) { // use dummy node
				n = nodes.back();
				miller->setNodeData(n, name);
				nodes.clear();
			} else {
				n = miller->addNode(branch, name);
			}
			if ( is_directory ( *it ) ) {
				miller->addNode(n, "<empty>");
			}
		} //for
	} catch(...) {
	}
	miller->updateLists();	
}

void onMillerPathChanged(void *src,
	const Browser::Event &ev)
{
	Browser::Ptr browser = boost::dynamic_pointer_cast<Browser>(ev.getSource());
	SAMBAG_ASSERT(browser);
	std::cout<<browser->selectionPathToString()<<std::endl;
	updateContent(browser, browser->getSelectionPath());
}

template <>
void createWindow<MILLER>() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	win[MILLER] = sdc::FramedWindow::create(win[0]);
	win[MILLER]->setTitle("Miller Columns");
	win[MILLER]->setWindowBounds(sambag::disco::Rectangle(110,100,900,880));

	Browser::Ptr miller = Browser::create();
	win[MILLER]->getContentPane()->add(miller);
	Browser::Node dummy = miller->addNode(miller->getRootNode(), "Dummy Folder");
	Browser::Node dummyContent = miller->addNode(dummy, "Dummy Content");
	miller->addNode(dummy, "Dummy File");
	miller->addNode(dummyContent, "Dummy File2");
	Browser::Node dummyFolder = miller->addNode(dummyContent, "Dummy Folder");
	miller->addNode(dummyFolder, "Dummy File3");
	createContent(miller);
	miller->EventSender<Browser::Event>::addEventListener(
		&onMillerPathChanged
	);
}

void onScrollTimer(void *src, const sdc::TimerEvent &ev, 
	boost::weak_ptr<sdc::Viewport> viewport) 
{
	if (currScrollSpeed == 0.f)
		return;
	sdc::Viewport::Ptr vp = viewport.lock();
	if (!vp)
		return;
	SAMBAG_BEGIN_SYNCHRONIZED(win[SURPRISE]->getRootPane()->getTreeLock())
		sd::Point2D p = vp->getViewPosition();
		p.y( p.y() + currScrollSpeed );
		vp->setViewPosition(p);
	SAMBAG_END_SYNCHRONIZED
}

template <>
void createWindow<SURPRISE>() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	win[SURPRISE] = sdc::FramedWindow::create(win[0]);
	win[SURPRISE]->setTitle("Surprise Window");
	win[SURPRISE]->setWindowBounds(sambag::disco::Rectangle(110,100,430,280));
	win[SURPRISE]->getContentPane()->setLayout(FlowLayout::create());
	win[SURPRISE]->getContentPane()->setComponentPopupMenu(createSurprisePopup());

	const int NUM = 1000;
	AContainerPtr con = Panel::create();
	//con->setOpaque(false);

	con->EventSender<sdc::events::MouseEvent>::
			addEventListener(&onRootMouse);

	con->EventSender<sdc::events::MouseEvent>::
			addEventListener(&trackMouse);

	con->setSize(Dimension(300, 1000));
	sdc::Viewport::Ptr vp = Viewport::create();
	vp->setView(con);
	con->setLayout(BoxLayout::create(con, BoxLayout::Y_AXIS));
	win[SURPRISE]->getContentPane()->add(vp);
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
		boost::bind( &onScrollTimer, _1, _2, vp ),
		vp
	);
	timer->start();
}

void printCompInfos(sdc::AComponent::Ptr c) {
	using namespace sambag::com;
	struct P {
		static void print(const std::string &str, int ident) {
			while (ident-->0)
				std::cout<<'\t';
			std::cout<<str<<std::endl;
		}
	};
	P::print(c->getName() + ":",0);
	P::print("size: " + toString(c->getSize()),1);
	P::print("preferred size:" + toString(c->getPreferredSize()),1);
}

template <>
void createWindow<SCROLLERCOASTER>() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	win[SCROLLERCOASTER] = sdc::FramedWindow::create(win[0]);
	win[SCROLLERCOASTER]->setTitle("ScrollerCoaster Window");
	win[SCROLLERCOASTER]->setWindowBounds(sambag::disco::Rectangle(110,100,430,280));

	const int NUM = 500;
	AContainerPtr con = Panel::create();
	con->setSize(Dimension(300, 1000));
	sdc::ScrollPane::Ptr vp = ScrollPane::create(con);
	//vp->setHorizontalScrollBarPolicy(ScrollPane::HORIZONTAL_SCROLLBAR_NEVER);
	con->setLayout(BoxLayout::create(con, BoxLayout::Y_AXIS));
	win[SCROLLERCOASTER]->getContentPane()->add(vp);
	for (int i=0; i<NUM; ++i) {
		std::stringstream ss;
		ss << i+1;
		Label::Ptr el = Label::create();
		el->setText(ss.str());
		el->setFont(el->getFont().setSize((NUM-i)*0.9));
		el->setIcon(getResourceManager().getImage("IconA"));
		con->add(el);
	}
	win[SCROLLERCOASTER]->getContentPane()->validate();
	std::cout<<vp->getBounds()<<std::endl;
}

template <>
void createWindow<ACME>() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	win[ACME] = sdc::FramedWindow::create(win[0]);
	win[ACME]->setTitle("ACME Window");
	win[ACME]->setWindowBounds(sambag::disco::Rectangle(110,100,430,280));


	Panel::Ptr con = Panel::create();
	win[ACME]->getContentPane()->add(con);
	win[ACME]->getContentPane()->setComponentPopupMenu(createAcmePopup());

	con->EventSender<sdc::events::MouseEvent>::
			addEventListener(&onRootMouse);
	con->EventSender<sdc::events::MouseEvent>::
			addEventListener(&trackMouse);

	for (int i = 0; i < 10; ++i) {
		std::stringstream ss;
		ss << i;
		Button::Ptr btn = Button::create();
		btn->EventSender<sdc::events::ActionEvent>::
				addEventListener(&onACMEButton);
		btn->EventSender<sdc::events::MouseEvent>::
				addEventListener(&trackMouse);
		btn->setText(ss.str());
		btn->getFont().setFontFace("monospace");
		btn->EventSender<sdc::events::MouseEvent>::
				addEventListener(&trackMouse);
		con->add(btn);
	}
	for (int i = 0; i < 26; ++i) {
		char c[] = {'A' + i, 0};
		Button::Ptr btn = Button::create();
		btn->EventSender<sdc::events::ActionEvent>::
				addEventListener(&onACMEButton);
		btn->setText(std::string(c));
		btn->getFont().setFontFace("monospace");
		btn->EventSender<sdc::events::MouseEvent>::
				addEventListener(&trackMouse);
		con->add(btn);
	}
	// checkbox
	CheckBox::Ptr chkbx = CheckBox::create();
	chkbx->setText("sometimes yes!");
	con->add(chkbx);
	// label
	Panel::Ptr labelcon = Panel::create();
    labelcon->setName("Output:");
	TitledBorder::Ptr border = TitledBorder::create();
	Label::Ptr label = Label::create();
	label->getFont().setFontFace("monospace").setSize(INPUT_LABEL_SIZE);
	labelcon->add(label);
	label->setBorder(border);
	con->add(labelcon);
	win[ACME]->getRootPane()->addTag(label, INPUT_LABEL);
}

void onSvgMouse(void *, const sdc::events::MouseEvent &ev) {
   	using namespace sambag::disco;
	using namespace sambag::disco::components;
    using namespace sambag::disco::components::events;
    AComponent::Ptr c = ev.getSource();
    if (ev.getType() == MouseEvent::DISCO_MOUSE_CLICKED) {
        std::cout<<c->toString()<<std::endl;
        const svg::HtmlColors::ColorMap &colors = svg::HtmlColors::getColorMap();
        int counter = rand() % colors.size();
        svg::HtmlColors::ColorMap::const_iterator it = colors.begin();
        while (--counter>0) {
            ++it;
        }
        c->setBackground(it->second);
    }
}

template <>
void createWindow<SVG>() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	win[SVG] = sdc::FramedWindow::create(win[0]);
	win[SVG]->setTitle("SVG Component");
	win[SVG]->setWindowBounds(sambag::disco::Rectangle(110,100,800,600));
    
    SvgComponent::Ptr svg = SvgComponent::create();
    svg->setSvgFilename("testimages/ComponentTestfield.svg");
    win[SVG]->getContentPane()->add(svg);
    std::vector<SvgComponent::Dummy::Ptr> dummies;
    svg->getDummiesByClass(".disco", dummies);
    BOOST_FOREACH(AComponent::Ptr x, dummies) {
        x->EventSender<sdc::events::MouseEvent>::addEventListener(
           &onSvgMouse
        );
    }
    
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
	win[ACME]->getRootPane()->getComponentsByTag(INPUT_LABEL, l);
	if (l.empty())
		return;
	Label::Ptr label = boost::dynamic_pointer_cast<Label>(l.back());
	if (!label)
		return;
	label->setText("");
	win[ACME]->getRootPane()->validate();
	label->getFont().setSize(INPUT_LABEL_SIZE);
}

void trackMouse(void *src, const sdc::events::MouseEvent &ev) {
	using namespace sambag::disco::components;
	using namespace sambag::disco;
	//std::cout<<ev.toString()<<std::endl;
}

void handlePopupMouse(sdc::PopupMenuPtr popup, const sdc::events::MouseEvent &ev) {
	using namespace sambag::disco::components;
	static PopupMenuPtr currPopup;
	if (ev.getButtons() != sdc::events::MouseEvent::DISCO_BTN2) {
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
		popup->setInvoker(ev.getSource());
		popup->showPopup(
			ev.getLocationOnScreen()
		);
	}
	currPopup = popup;
}

void onRootMouse(void *src, const sdc::events::MouseEvent &ev) {
	using namespace sambag::disco::components;
	if (ev.getType() != events::MouseEvent::DISCO_MOUSE_CLICKED)
		return;
	PopupMenuPtr menu = ev.getSource()->getComponentPopupMenu();
	if (!menu)
		return;
	handlePopupMouse(menu, ev);
}

void onACMEButton(void *src, const sdc::events::ActionEvent &ev) {
	using namespace sambag::disco::components;
	Button::Ptr b = boost::dynamic_pointer_cast<Button>(ev.getSource());
	if (!b) {
		std::cout<<ev.getSource()->toString()<<std::endl;
		return;
	}
	std::list<AComponentPtr> l;
	win[ACME]->getRootPane()->getComponentsByTag(INPUT_LABEL, l);
	if (l.empty())
		return;
	Label::Ptr label = boost::dynamic_pointer_cast<Label>(l.back());
	if (!label)
		return;
	std::string txt = label->getText() + b->getText();
	label->setText(txt);
	if (txt.length() > 15) {
		label->getFont().setSize( (15 / (float)txt.length()) * INPUT_LABEL_SIZE );
	}
	win[ACME]->getContentPane()->validate();
	win[ACME]->getContentPane()->redraw();
}

template <int View>
void onBtnCreate(void *src, const sdc::events::ActionEvent &ac) {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	if (!win[View]) {
		createWindow<View>();
	}
	win[View]->getContentPane()->validate();
    win[View]->setEnabled(false);
	win[View]->open();

}

void onByeClicked ( void *src, const sdc::events::ActionEvent &ac) {
	if (win[0])
		win[0]->close();
}

void pathChanged(void *src, const sdc::MenuSelectionManagerChanged &ev) {
//	std::cout<<ev.getSrc().getSelectedPath()<<std::endl;
}

void timedCallbackInf(void *src, const sdc::TimerEvent &ev) {
	static int i=0;
	std::cout<<"timed callbackInf(" << ++i << ")" << std::flush << std::endl;
	//while(1);
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
	timerInf = Timer::create(1000);
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

void initResources() {
	sambag::disco::FileResourceManager::init("./testimages");
	sambag::disco::FileResourceManager &m = 
		sambag::disco::FileResourceManager::instance();
	m.registerImage("IconA","disco-floor.png");
}

void initLogger() {
    sambag::com::addLogFile("testApp.log");
    SAMBAG_LOG_ERR<<"running";
}
int main() {

	initLogger();

	SAMBAG_WINONLY(
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); //VS memory tracking
	)
	initResources();
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	std::cout<<"hi"<<std::endl;
	std::cout<<getDiscoFactory()->implDetails()<<std::endl;
	std::cout<<getWindowToolkit()->getScreenSize()<<std::endl;
    getWindowToolkit()->setMouseEventCreatorPrototype(
        sambag::disco::components::events::MouseEventRecorder::create()
    );
	initTimer();
	{ // extra scope (bye message should occur after releasing all objs)
		win[0] = sdc::FramedWindow::create();
		win[0]->setDefaultCloseOperation(Window::EXIT_ON_CLOSE);
		win[0]->setWindowBounds(sambag::disco::Rectangle(0,0,230,200));
		win[0]->setTitle("Messerschmitz 1.0");
		win[0]->getContentPane()->setLayout(FlowLayout::create());
		win[0]->getContentPane()->EventSender<sdc::events::MouseEvent>::addEventListener(
			&trackMouse
		);
		win[0]->getContentPane()->setMouseWheelEventsEnabled(true);
		Button::Ptr btn = Button::create();
		btn->setText("open ACME Panel");
		//btn->setTooltipText("open new ACME panel");
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onBtnCreate<ACME>);
		btn->getFont().setFontFace("monospace");
		win[0]->getContentPane()->add(btn);
		btn->EventSender<sdc::events::MouseEvent>::addEventListener(
			&trackMouse
		);

		btn = Button::create();
		btn->setText("open Surprise Panel");
		btn->setTooltipText("open new Surprise panel");
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onBtnCreate<SURPRISE>);
		btn->getFont().setFontFace("monospace");
		win[0]->getContentPane()->add(btn);

		btn = Button::create();
		btn->setText("open Borderline Panel");
		btn->setTooltipText("open new Borderline panel");
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onBtnCreate<BORDERLINE>);
		btn->getFont().setFontFace("monospace");
		win[0]->getContentPane()->add(btn);

		btn = Button::create();
		btn->setText("open Scrollercoaster");
		btn->setTooltipText("open new Scrollercoaster panel");
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onBtnCreate<SCROLLERCOASTER>);
		btn->getFont().setFontFace("monospace");
		win[0]->getContentPane()->add(btn);

		btn = Button::create();
		btn->setText("the L.I.S.T.");
		btn->setTooltipText("open new List panel");
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onBtnCreate<LIST>);
		win[0]->getContentPane()->add(btn);

		btn = Button::create();
		btn->setText("Miller Milch");
		btn->setTooltipText("open new Miller panel");
		btn->setIcon(getResourceManager().getImage("IconA"));
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onBtnCreate<MILLER>);
		win[0]->getContentPane()->add(btn);
        
        btn = Button::create();
		btn->setText("SAV UGGER");
		btn->setTooltipText("open SVG component TestWindow");
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onBtnCreate<SVG>);
		win[0]->getContentPane()->add(btn);

		btn = Button::create();
		btn->setText("tschüß");
		btn->setTooltipText("I should not be shown.");
		btn->setTooltipText("");
		btn->getFont().setFontFace("monospace").setSize(50);
		btn->EventSender<sdc::events::ActionEvent>::addEventListener(&onByeClicked);
		btn->setMouseWheelEventsEnabled(true);

		win[0]->getContentPane()->add(btn);
		win[0]->validate();
		//win[0]->pack();
		win[0]->open();

		MenuSelectionManager::defaultManager().
		EventSender<MenuSelectionManagerChanged>::addEventListener
			( &pathChanged );

		try {
			sdc::Window::startMainLoop();
		} catch(const std::runtime_error &ex) {
			std::cout<<ex.what()<<std::endl;
		}
	}
	win[0].reset();
	std::cout<<"bye"<<std::endl;
	return 0;
}

