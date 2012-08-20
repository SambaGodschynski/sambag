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
#include <sambag/disco/components/WindowToolkit.hpp>
#include <sambag/com/Common.hpp>
#include <sambag/com/ArbitraryType.hpp>

#include <Windowsx.h> // GET_X_LPARAM

namespace {
	template <typename T>
	void debugMsg(HWND hwnd, const T &msg) {
		std::cout<<"["<<std::hex<<hwnd<<"]:"
		<<msg<<";"<<std::endl<<std::flush;
	}
	int nbWndClassRegs = 0;
	int nbWndNestedClassRegs = 0;
	WNDCLASS wndClass = {0};
	WNDCLASS nestedWndClass = {0};
}

namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class Win32WindowImpl
//=============================================================================
//-----------------------------------------------------------------------------
int Win32WindowImpl::instances = 0;
//-----------------------------------------------------------------------------
Win32WindowImpl::WinMap Win32WindowImpl::winmap;
//-----------------------------------------------------------------------------
void Win32WindowImpl::initWindowClass(HINSTANCE hI) {
	if ( nbWndClassRegs++ > 0 ) {
		SAMBAG_ASSERT(wndClass.hInstance == hI);
		return;
	}
	wndClass.lpfnWndProc   = &Win32WindowImpl::wndProc;
	wndClass.hInstance     = hI;
	wndClass.hbrBackground = 0; //(HBRUSH)(COLOR_BACKGROUND);
	wndClass.lpszClassName = "discowindowimpl";
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	if( FAILED(RegisterClass(&wndClass)) )
		throw std::runtime_error ("Win32 RegisterClass failed.");
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::initNestedWindowClass(HINSTANCE hI) {
	if ( nbWndNestedClassRegs++ > 0 ) {
		SAMBAG_ASSERT(nestedWndClass.hInstance == hI);
		return;
	}
	nestedWndClass.style = CS_GLOBALCLASS | CS_DBLCLKS;
	nestedWndClass.cbClsExtra  = 0; 
	nestedWndClass.cbWndExtra  = 0; 
	nestedWndClass.hbrBackground = 0;
	nestedWndClass.lpszMenuName  = 0; 
	nestedWndClass.lpfnWndProc   = &Win32WindowImpl::wndProc;
	nestedWndClass.hInstance     = hI;
	nestedWndClass.lpszClassName = "disconestedwindowimpl";
	nestedWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	if( FAILED(RegisterClass(&nestedWndClass)) )
		throw std::runtime_error ("Win32 RegisterClass failed.");
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::destroyWindowClass() {
	if ( --nbWndClassRegs != 0 )
		return;
	UnregisterClass("discowindowimpl", wndClass.hInstance);
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::destroyNestedWindowClass() {
	if ( --nbWndNestedClassRegs != 0 )
		return;
	UnregisterClass("disconestedwindowimpl", nestedWndClass.hInstance);
}
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
void Win32WindowImpl::drawAll() {
	BOOST_FOREACH(WinMap::value_type &obj, winmap) {
		//obj.second->processDraw();
		InvalidateRect(obj.second->win, NULL, false);
	}
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::update() {
	createSurface();
	this->processDraw();
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::initAsNestedWindow(ArbitraryType::Ptr osParent, 
	const Rectangle &area) 
{
	++instances;
	std::pair<void*, void*> parentData(NULL, NULL);
	sambag::com::get(osParent, parentData);
	if (!parentData.first)
		return;
	HWND parent = (HWND)parentData.first;
	HINSTANCE hI = (HINSTANCE)parentData.second;
	initNestedWindowClass(hI);

	DWORD styleEx = 0; //WS_EX_TRANSPARENT | WS_EX_COMPOSITED;
	DWORD style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	bounds = area;
	win = CreateWindowEx ( styleEx, 
		nestedWndClass.lpszClassName, 
		"Window",
		style,
		0,
		0,
		(int)bounds.width(),
		(int)bounds.height(),
		parent, // HWND hWndParent
		NULL,   // HMENU hMenu
		hI,     // HINSTANCE hInstance
		NULL    // LPVOID lpParam
	);
	if (!win) {
		std::stringstream ss;
		DWORD err = GetLastError();
		ss<<"window creation failed: "<<err;
		throw std::runtime_error(ss.str());
	}

	// register win
	winmap[win] = this;
	visible = true;
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::createWindow(HWND parent) {
	++instances;
	HINSTANCE hI = GetModuleHandle(NULL);
	initWindowClass(hI);
	
	DWORD styleEx = 0; //WS_EX_TRANSPARENT | WS_EX_COMPOSITED;
	DWORD style = WS_VISIBLE;
	style |= getFlag(WND_FRAMED) ? WS_OVERLAPPEDWINDOW : WS_POPUP | WS_BORDER; 

	win = CreateWindowEx ( styleEx, 
		wndClass.lpszClassName, 
		title.c_str(),
		style,
		(int)bounds.x0().x(),
		(int)bounds.x0().y(),
		(int)bounds.width(),
		(int)bounds.height(),
		parent, // HWND hWndParent
		NULL,   // HMENU hMenu
		hI,     // HINSTANCE hInstance
		NULL    // LPVOID lpParam
	);

	if (!win) {
		std::stringstream ss;
		DWORD err = GetLastError();
		ss<<"window creation failed: "<<err;
		throw std::runtime_error(ss.str());
	}

	updateBoundsToWindow();

	// register win
	winmap[win] = this;

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
	visible = false;
	if (getFlag(WND_NESTED)) {
		destroyNestedWindowClass();
	} else {
		destroyWindowClass();
	}
	--instances;
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::close() {
	getWindowToolkit()->invokeLater(
		boost::bind(&Win32WindowImpl::_close, this)
	);
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::open(AWindowImplPtr parent) {
	getWindowToolkit()->invokeLater(
		boost::bind(&Win32WindowImpl::_open, this, parent)
	);
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::_close() {
	if (!visible)
		return;
	PostMessage(win, WM_CLOSE, 0, 0);
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::_open(AWindowImplPtr parent) {
	if (visible)
		return;
	Win32WindowImpl *p = dynamic_cast<Win32WindowImpl*>(parent.get());
	HWND pHwnd = NULL;
	if (p) {
		pHwnd = p->getHwnd();
	}
	createWindow(pHwnd);
}
//-----------------------------------------------------------------------------
Win32WindowImpl::~Win32WindowImpl() {
	_close();
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

	RECT wRect;
	SetRect ( &wRect, 0, 0, (int)bounds.width(), (int)bounds.height() );
	AdjustWindowRectEx (&wRect, 
		GetWindowLong (win, GWL_STYLE), 
		FALSE, GetWindowLong (win, GWL_EXSTYLE)
	);
	int width = wRect.right - wRect.left;
	int height = wRect.bottom - wRect.top;
	//SetWindowPos (win, HWND_TOP, bounds.x0().x(), 0, width, height, SWP_NOMOVE);
	SetWindowPos(win, 
		HWND_TOP,
		(int)bounds.x0().x(),
		(int)bounds.x0().y(),
		width,
		height,
		0
	);
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::invalidateWindow(const Rectangle &area) {
	enum { ERASE_BG = false };
	if (!win)
		return;
	if (area == NULL_RECTANGLE) {
		InvalidateRect(win, NULL, ERASE_BG);
		return;
	}
	RECT wRect;
	SetRect ( &wRect, (int)area.x0().x(), 
		(int)area.x0().y(), 
		(int)area.width(), 
		(int)area.height() 
	);
	InvalidateRect(win, &wRect, ERASE_BG);
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
	case WM_SETFOCUS:
		break;
	case WM_DESTROY:
		win->destroyWindow();
		if (instances==0)
			if (!win->getFlag(WND_NESTED))
				PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_PAINT : {
		if (win) {
			win->update();
		}
		ValidateRect(hWnd, NULL);
		break;
	}
	case WM_MOVE : {
		RECT r = {0};
		GetClientRect(hWnd, &r);
		Rectangle nr(Point2D(r.left, r.top), Point2D(r.right, r.bottom));
		if (win)
			win->updateWindowToBounds(nr);
		break;
	}
	case WM_SIZE : {
		RECT r = {0};
		GetClientRect(hWnd, &r);
		Rectangle nr(Point2D(r.left, r.top), Point2D(r.right, r.bottom));
		if (win)
			win->updateWindowToBounds(nr);
		break;
	}
	case WM_LBUTTONDOWN : 
		SetCapture(hWnd);
		x = GET_X_LPARAM(lParam); 
		y = GET_Y_LPARAM(lParam);
		if (win)
			win->handleMouseButtonPressEvent(x, y, 1);
		break;
	case WM_RBUTTONDOWN :
		SetCapture(hWnd);
		x = GET_X_LPARAM(lParam); 
		y = GET_Y_LPARAM(lParam);
		if (win)
			win->handleMouseButtonPressEvent(x, y, 2);
		break;
	case WM_MBUTTONDOWN :
		SetCapture(hWnd);
		x = GET_X_LPARAM(lParam); 
		y = GET_Y_LPARAM(lParam);
		if (win)
			win->handleMouseButtonPressEvent(x, y, 4);
		break;
	case WM_LBUTTONUP : 
		ReleaseCapture();
		x = GET_X_LPARAM(lParam); 
		y = GET_Y_LPARAM(lParam);
		if (win)
			win->handleMouseButtonReleaseEvent(x, y, 1);
		break;
	case WM_RBUTTONUP :
		ReleaseCapture();
		x = GET_X_LPARAM(lParam); 
		y = GET_Y_LPARAM(lParam);
		if (win)
			win->handleMouseButtonReleaseEvent(x, y, 2);
		break;
	case WM_MBUTTONUP :
		ReleaseCapture();
		x = GET_X_LPARAM(lParam); 
		y = GET_Y_LPARAM(lParam);
		if (win)
			win->handleMouseButtonReleaseEvent(x, y, 4);
		break;
	case WM_MOUSEMOVE :
		x = GET_X_LPARAM(lParam); 
		y = GET_Y_LPARAM(lParam);
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
