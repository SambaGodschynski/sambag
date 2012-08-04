/*
 * X11Window.cpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_WIN32

#include "Win32WindowImpl.hpp"
#include <sambag/disco/Win32Surface.hpp>
#include <boost/foreach.hpp>
#include "X11WindowToolkit.hpp"
#include <sambag/com/Common.hpp>

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Win32WindowImpl
//=============================================================================
//-----------------------------------------------------------------------------
Win32WindowImpl::WinMap Win32WindowImpl::winmap;
//-----------------------------------------------------------------------------
int Win32WindowImpl::instances = 0;
//-----------------------------------------------------------------------------
Win32WindowImpl * Win32WindowImpl::getWin32WindowImpl(HWND win) {
	WinMap::iterator it = winmap.find(win);
	if (it==winmap.end())
		return NULL;
	return it->second;
}
//-----------------------------------------------------------------------------
Win32WindowImpl::Win32WindowImpl() :
	bounds(Rectangle(0,0,1,1)),
	visible(false),
	win(NULL)
{
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::update() {
	this->processDraw();
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::createWindow() {
	if (instances++==0) {
		// init at once
	}
	HINSTANCE hI = GetModuleHandle(NULL);
	WNDCLASS wc      = {0}; 
	wc.lpfnWndProc   = &Win32WindowImpl::wndProc;
	wc.hInstance     = hI;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = "discowindowimpl";
	if( FAILED(RegisterClass(&wc)) )
		return;

	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	win = CreateWindow(wc.lpszClassName,
		title.c_str(),
		style,
        (int)bounds.x0().x(),
		(int)bounds.x0().y(),
		(int)bounds.width(),
		(int)bounds.height(),
		0,0,hI,NULL
	);
	SAMBAG_ASSERT(win);
    
	// register win
	winmap[win] = this;

	// disco stuff
	createSurface();
	SAMBAG_ASSERT(surface);

	visible = true;
	onCreated();
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::destroyWindow() {
	if (win==NULL)
		return;
	// unregister window
	winmap.erase(win);
	surface.reset();
	onDestroy();
	win = 0;
	--instances;
	visible = false;
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::close() {
	if (!visible)
		return;
	PostMessage(win, WM_CLOSE, 0, 0);
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::open() {
	if (visible)
		return;
	createWindow();
}
//-----------------------------------------------------------------------------
Win32WindowImpl::~Win32WindowImpl() {
	close();
}
//-----------------------------------------------------------------------------
bool Win32WindowImpl::isVisible() const {
	return visible;
}
//-----------------------------------------------------------------------------
Rectangle Win32WindowImpl::getBounds() const {
	return bounds;
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::createSurface() {
	using namespace sambag;
	Rectangle r = getBounds();
	surface = disco::Win32Surface::create(win,
			(int)r.getWidth(), (int)r.getHeight());
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::setBounds(const Rectangle &r) {
	Rectangle curr = getBounds();
	if (curr == r)
		return;
	bounds = r;

	if (bounds.getWidth() < 1.)
		bounds.setWidth(1.);
	if (bounds.getHeight() < 1.)
		bounds.setHeight(1.);

	if (!isVisible())
		return;

	updateBoundsToWindow();
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::setTitle(const std::string &_title) {
	title = _title;
	if (!isVisible())
		return;
	updateTitle();
}
//-------------------------------------------------------------------------
std::string Win32WindowImpl::getTitle() const {
	return title;
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::updateBoundsToWindow() {
	if (!win)
		return;

	/*RECT wRect;
	SetRect ( &wRect, bounds.x0().x(), bounds.x0().y(), bounds.width(), bounds.height() );
	AdjustWindowRectEx (&wRect, 
		GetWindowLong (win, GWL_STYLE), 
		FALSE, GetWindowLong (win, GWL_EXSTYLE)
	);
	int width = wRect.right - wRect.left;
	int height = wRect.bottom - wRect.top;
	SetWindowPos (win, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);*/
	SetWindowPos(win, 
		HWND_TOP,
		(int)bounds.x0().x(),
		(int)bounds.x0().y(),
		(int)bounds.width(),
		(int)bounds.height(),
		0
	);
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::updateWindowToBounds(const Rectangle &r) {
	if (r.getDimension() != bounds.getDimension()) {
		createSurface();
		UpdateWindow(win);
	}
	bounds = r;
	boundsUpdated();
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::updateTitle() {
	if (!win)
		return;
	SetWindowText(win, title.c_str());
}
//-----------------------------------------------------------------------------
LRESULT CALLBACK Win32WindowImpl::wndProc(HWND hWnd, UINT message, 
	WPARAM wParam, LPARAM lParam) 
{
	Win32WindowImpl *win = getWin32WindowImpl(hWnd);
	int mbuttons = 0;
	int x=0; int y=0;

	switch(message) {
	case WM_DESTROY:
		win->destroyWindow();
		if (instances==0)
			PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_PAINT : {
		if (win)
			win->update();
		break;
	}
	case WM_SIZE : {
		RECT r = {0};
		GetWindowRect(hWnd, &r);
		Rectangle nr(Point2D(r.left, r.top), Point2D(r.right, r.bottom));
		if (win)
			win->updateWindowToBounds(nr);
		break;
	}
	case WM_LBUTTONDOWN : 
		x = LOWORD(lParam); 
		y = HIWORD(lParam);
		if (win)
			win->handleMouseButtonPressEvent(x, y, 1);
		break;
	case WM_RBUTTONDOWN :
		x = LOWORD(lParam); 
		y = HIWORD(lParam);
		if (win)
			win->handleMouseButtonPressEvent(x, y, 2);
		break;
	case WM_MBUTTONDOWN :
		x = LOWORD(lParam); 
		y = HIWORD(lParam);
		if (win)
			win->handleMouseButtonPressEvent(x, y, 4);
		break;
	case WM_LBUTTONUP : 
		x = LOWORD(lParam); 
		y = HIWORD(lParam);
		if (win)
			win->handleMouseButtonReleaseEvent(x, y, 1);
		break;
	case WM_RBUTTONUP :
		x = LOWORD(lParam); 
		y = HIWORD(lParam);
		if (win)
			win->handleMouseButtonReleaseEvent(x, y, 2);
		break;
	case WM_MBUTTONUP :
		x = LOWORD(lParam); 
		y = HIWORD(lParam);
		if (win)
			win->handleMouseButtonReleaseEvent(x, y, 4);
		break;
	case WM_MOUSEMOVE :
		x = LOWORD(lParam); 
		y = HIWORD(lParam);
		if (win)
			win->handleMouseMotionEvent(x, y);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

}}} // namespace(s)

#endif // DISCO_USE_X11
