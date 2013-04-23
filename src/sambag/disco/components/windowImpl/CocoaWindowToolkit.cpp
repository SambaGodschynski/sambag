/*
 * CocoaWindowToolkit.cpp
 *
 *  Created on: Thu Jun  7 10:44:57 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_COCOA

#include "CocoaWindowToolkit.hpp"
#include "WindowImpl.hpp"
#include "CocoaWindowImpl.hpp"
#include "cocoaimpl/_CocoaWindowImpl.h"
#include <sambag/disco/components/Window.hpp>
#include <sambag/disco/components/Timer.hpp>
#include <boost/thread.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class CocoaWindowToolkit
//=============================================================================
//-----------------------------------------------------------------------------
bool CocoaWindowToolkit::mainLoopRunning = false;
//-----------------------------------------------------------------------------
CocoaWindowToolkit::CocoaWindowToolkit() {
}
//-----------------------------------------------------------------------------
CocoaWindowToolkit::~CocoaWindowToolkit() {
	TimerPolicy::tearDownTimer();
}
//-----------------------------------------------------------------------------
void CocoaWindowToolkit::startMainLoop() {
	//invokeLater(&checkWindowCount, 1000);
	mainLoop();
}
//-----------------------------------------------------------------------------
AWindowImplPtr CocoaWindowToolkit::createWindowImpl(AWindowImplPtr parent) const 
{
	AWindowImplPtr res =
		WindowImpl<CocoaWindowImpl>::create(parent);
	return res;
}
//-------------------------------------------------------------------------
WindowPtr CocoaWindowToolkit::createNestedWindow( ArbitraryType::Ptr osParent, 
	const Rectangle &area )
{
	AWindowImpl::Ptr windowImpl =
		WindowImpl<CocoaWindowImpl>::create(osParent, area);
	return Window::create(windowImpl);
}
//-----------------------------------------------------------------------------
Dimension CocoaWindowToolkit::getScreenSize() const {
	_CocoaToolkitImpl::Number w=0, h=0;
	_CocoaToolkitImpl::getScreenDimension(w,h);
	return Dimension(w,h);
}
//-----------------------------------------------------------------------------
CocoaWindowToolkit * CocoaWindowToolkit::getToolkit() {
	typedef Loki::SingletonHolder<CocoaWindowToolkit> FactoryHolder;
	return &FactoryHolder::Instance();
}
//-----------------------------------------------------------------------------
void CocoaWindowToolkit::useWithoutMainloop() {
	TimerPolicy::startUpTimer();
}
//-----------------------------------------------------------------------------
void CocoaWindowToolkit::mainLoop() {
	TimerPolicy::startUpTimer();
	_CocoaToolkitImpl::startMainApp();
	TimerPolicy::tearDownTimer();
}
//-----------------------------------------------------------------------------
void CocoaWindowToolkit::quit() {
}
//-----------------------------------------------------------------------------
void CocoaWindowToolkit::invokeLater(const CocoaWindowToolkit::InvokeFunction &f) 
{
	invokeLater(f, 50);
}
namespace {
	/*
	 * timer callback
	 */
	void _invokeLater(CocoaWindowToolkit::InvokeFunction f)
	{
		f();
	}
} // namespace(s)
//----------------------------------------------------------------------------
void CocoaWindowToolkit::invokeLater(const InvokeFunction &f, int ms) {
	Timer::Ptr tm = Timer::create(ms);
	tm->setNumRepetitions(0);
	tm->EventSender<TimerEvent>::addEventListener(
		boost::bind(&_invokeLater, f)
	);
	tm->start();
}
//-----------------------------------------------------------------------------
void CocoaWindowToolkit::startTimer(Timer::Ptr tm) {
	TimerPolicy::startTimer(tm);
}
//-----------------------------------------------------------------------------
void CocoaWindowToolkit::stopTimer(Timer::Ptr tm) {
	TimerPolicy::stopTimer(tm);
}
///////////////////////////////////////////////////////////////////////////////
WindowToolkit * _getWindowToolkitImpl() {
	return CocoaWindowToolkit::getToolkit();
}
}}} // namespace(s)

#endif // DISCO_USE_COCOA
