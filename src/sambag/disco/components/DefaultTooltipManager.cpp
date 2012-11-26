/*
 * DefaultTooltipManager.cpp
 *
 *  Created on: Fri Nov 23 14:43:39 2012
 *      Author: Johannes Unger
 */

#include "DefaultTooltipManager.hpp"
#include "AComponent.hpp"
#include <boost/foreach.hpp>
#include "Window.hpp"
#include "ui/UIManager.hpp"
#include "Label.hpp"

namespace sambag { namespace disco { namespace components {
struct TooltipWindow : public Window {
	typedef boost::shared_ptr<TooltipWindow> Ptr;
	static TooltipWindow::Ptr sharedInstance;
	Label::Ptr label;
	TooltipWindow(Window::Ptr parent) : Window(parent) {}
	virtual void postConstructor();
	void setText(const std::string &txt); 
	static Ptr create(AComponentPtr invoker);
};
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
TooltipWindow::Ptr TooltipWindow::sharedInstance;
//-----------------------------------------------------------------------------
void TooltipWindow::setText(const std::string &txt) {
	label->setText(txt);
	Dimension size = label->getMinimumSize();
	size.width( size.width() );
	size.height( size.height() + 10.);
	label->setBounds(Rectangle(0, 0, size.width(), size.height()));
	setWindowSize(size);
}
//-----------------------------------------------------------------------------
TooltipWindow::Ptr TooltipWindow::create(AComponentPtr invoker) {
	WindowPtr parent = invoker->getTopLevelAncestor();
	if (!parent)
		return TooltipWindow::Ptr();
	Ptr res(new TooltipWindow(parent));
	// transfer invokers laf
	ui::UIManager &uim = ui::getUIManager();
	ui::ALookAndFeel::Ptr laf = uim.getLookAndFeel(parent->getRootPane());
	uim.installLookAndFeel(res->getRootPane(), laf);
	res->self = res;
	res->postConstructor();
	res->initWindow();
	res->setText(invoker->getTooltipText());
	return res;
}
//-----------------------------------------------------------------------------
void TooltipWindow::postConstructor() {	
	AContainer::Ptr cnt = getContentPane();
	cnt->setLayout( ALayoutManager::Ptr() );
	label = Label::create();
	label->setForeground(ColorRGBA(0.0, 0.0, 0.0));
	cnt->add(label);
	cnt->setBackground(ColorRGBA(0.8, 0.8, 0.8));
}

typedef Loki::SingletonHolder<DefaultTooltipManager> DefaultTooltipManagerHolder;
//=============================================================================
//  Class DefaultTooltipManager
//=============================================================================
//-----------------------------------------------------------------------------
DefaultTooltipManager::DefaultTooltipManager() :
	dismissDelay(10),
	initialDelay(1000), 
	reshowDelay(10),
	enabled(true)
{
}
//-----------------------------------------------------------------------------
DefaultTooltipManager::~DefaultTooltipManager() {
	timerConnection.disconnect();
	BOOST_FOREACH(ConnectionMap::value_type &val, connectionMap) {
		val.second.disconnect();
	}
}
//-----------------------------------------------------------------------------
DefaultTooltipManager & DefaultTooltipManager::instance() {
	return DefaultTooltipManagerHolder::Instance();
}
//-----------------------------------------------------------------------------
int DefaultTooltipManager::getDismissDelay() const {
	return 0; //dismissDelay;
}
//-----------------------------------------------------------------------------
int DefaultTooltipManager::getInitialDelay() const {
	return initialDelay;
}
//-----------------------------------------------------------------------------
int DefaultTooltipManager::getReshowDelay() const {
	return 0; //reshowDelay;
}
//-----------------------------------------------------------------------------
bool DefaultTooltipManager::isEnabled() const {
	return enabled;
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::registerComponent(AComponentPtr component)
{
	if (connectionMap.find(component) != connectionMap.end())
		return;
	connectionMap[component] = 
		component->EventSender<events::MouseEvent>::addEventListener(
			boost::bind(&DefaultTooltipManager::onMouse, this, _1, _2)
		);
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::setDismissDelay(int milliseconds) {
	
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::setEnabled(bool flag) {
	enabled = flag;
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::setInitialDelay(int milliseconds) {
	initialDelay = milliseconds;
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::setReshowDelay(int milliseconds) {
	reshowDelay = milliseconds;
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::unregisterComponent(AComponentPtr component)
{
	ConnectionMap::iterator it = connectionMap.find(component);
	if (it==connectionMap.end())
		return;
	it->second.disconnect();
	connectionMap.erase(it);
}
//-----------------------------------------------------------------------------
Timer::Ptr DefaultTooltipManager::getTimer() {
	if (timer)
		return timer;
	timer = Timer::create(initialDelay);
	timerConnection = timer->EventSender<TimerEvent>::addEventListener(
		boost::bind(&DefaultTooltipManager::onTimer, this, _1, _2)
	);
	return timer;
}
//-----------------------------------------------------------------------------
Point2D DefaultTooltipManager::determineLocation(const events::MouseEvent &ev) 
{
	Point2D res = ev.getLocationOnScreen();
	res.x( res.x() + 15. );
	res.y( res.y() + 50. );
	return res;
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::onMouse(void *src, const events::MouseEvent &ev) {
	enum { Filter = 
		events::MouseEvent::DISCO_MOUSE_ENTERED |
		events::MouseEvent::DISCO_MOUSE_EXITED
	};
	location = determineLocation(ev);
	events::MouseEventSwitch<Filter>::delegate(ev, *this);
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::mouseEntered(const events::MouseEvent &ev) {
	currentObject = ev.getSource();
	getTimer()->start();
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::mouseExited(const events::MouseEvent &ev) {
	currentObject.reset();
	getTimer()->stop();
	hideTooltip(ev.getSource());
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::onTimer(void *src, const TimerEvent &ev) {
	AComponentPtr obj = currentObject.lock();
	if (!obj)
		return;
	showTooltip(obj);
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::showTooltip(AComponentPtr c) {
	if (!enabled)
		return;
	TooltipWindow::sharedInstance = TooltipWindow::create(c);
	TooltipWindow::sharedInstance->setWindowLocation(location);
	TooltipWindow::sharedInstance->open();
}
//-----------------------------------------------------------------------------
void DefaultTooltipManager::hideTooltip(AComponentPtr c) {
	if (TooltipWindow::sharedInstance)
		TooltipWindow::sharedInstance->close();
}
}}} // namespace(s)
