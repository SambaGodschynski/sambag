/*
 * X11Window.cpp
 *
 *  Created on: Thu Jun  7 11:14:06 2012
 *      Author: Johannes Unger
 */
#ifdef DISCO_USE_WIN32

#include "Win32WindowImpl.hpp"
#include <boost/foreach.hpp>
#include <sambag/disco/components/WindowToolkit.hpp>
#include <sambag/com/Common.hpp>
#include <sambag/com/ArbitraryType.hpp>
#include <math.h>

#include <Windowsx.h> // GET_X_LPARAM

namespace sambag { namespace disco { namespace components {
namespace {
	template <typename T>
	void debugMsg(HWND hwnd, const T &msg)
	{
		std::cout<<"["<<std::hex<<hwnd<<"]:"
		<<msg<<";"<<std::endl<<std::flush;
	}
	WNDCLASS  nullWndClass = {0};
	const std::string STR_WNDCLASS = "_discowindowimpl";
	const std::string STR_NESTEDWNDCLASS = "_disconestedwindowimpl";
	void initWndClass(WNDCLASS &wc, const WndClassManager::WndClassId &id) 
	{
		// common values
		wc.lpfnWndProc   = &Win32WindowImpl::__wndProc_;
		wc.hInstance     = id.second;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);

		// specific values
		if (id.first == STR_WNDCLASS) {
			// wc.lpszClassName = id.first.c_str(); <= dosent work
			 wc.lpszClassName = STR_WNDCLASS.c_str();
		}
		if (id.first == STR_NESTEDWNDCLASS) {
			wc.style = CS_DBLCLKS | CS_GLOBALCLASS;
			wc.lpszClassName = STR_NESTEDWNDCLASS.c_str();
		}
		// register class
		if( FAILED(RegisterClass(&wc)) )
			throw std::runtime_error ("Win32 RegisterClass failed.");
	}
	HINSTANCE getHInstance() 
	{
		HINSTANCE res;
		sambag::com::ArbitraryType::Ptr obj =
			getGlobalUserData("win32.hinstance");
		if (!obj) {
			return GetModuleHandle(NULL);
		}
		sambag::com::get(obj, res);
		return res;
	}
	Rectangle _getWin32AbsBounds(HWND hWnd) 
	{
		RECT r = {0};
		GetClientRect(hWnd, &r);
		POINT pos = {0};
		ClientToScreen(hWnd, &pos);
		return Rectangle (Point2D(pos.x, pos.y), r.right, r.bottom);
	}
	Rectangle _getWin32RelBounds(HWND hWnd) 
	{
		RECT r = {0};
		GetClientRect(hWnd, &r);
		return Rectangle (Point2D(r.left, r.top), Point2D(r.right, r.bottom));
	}
} // namespace(s)
//=============================================================================
// class WndClassManager	
//=============================================================================
//-----------------------------------------------------------------------------
WndClassManager::WndClassMap WndClassManager::wndClassMap;
//-----------------------------------------------------------------------------
WndClassManager::WndClassHolder::~WndClassHolder() {
	UnregisterClass(wndClassId.first.c_str(), wndClassId.second);
}
//-----------------------------------------------------------------------------
WndClassManager::WndClassHolder::Ptr 
WndClassManager::WndClassHolder::create(const WndClassId &id) 
{
	Ptr res(new WndClassHolder());
	res->wndClassId = id;
	res->wndClass = nullWndClass;
	initWndClass(res->wndClass, id);
	return res;
}
//-----------------------------------------------------------------------------
WndClassManager::WndClassHolder::Ptr 
WndClassManager::getWndClassHolder(const std::string &name, HINSTANCE hi) 
{
	WndClassId id(name, hi);
	WndClassHolder::WPtr holder = wndClassMap[id];
	WndClassHolder::Ptr res = holder.lock();
	if (!res) {
		res = WndClassHolder::create(id);
		wndClassMap[id] = res;
	}
	return res;
}
//=============================================================================
//  Class Win32WindowImpl
//=============================================================================
//-----------------------------------------------------------------------------
int Win32WindowImpl::instances = 0;
//-----------------------------------------------------------------------------
Win32WindowImpl::WinMap Win32WindowImpl::winmap;
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
disco::Win32Surface::Ptr Win32WindowImpl::getSurface(HDC dc) {
	using namespace sambag;
	//if (!surface) { //see: issue #238
		Rectangle r = getBounds();
		surface = disco::Win32Surface::create(dc,
			(int)r.getWidth(), (int)r.getHeight());
	//}
	return surface;
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::update(HDC dc) {
	disco::Win32Surface::Ptr sf = getSurface(dc);
	this->processDraw( sf );
	cairo_surface_flush(sf->getCairoSurface());
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::initAsNestedWindow(ArbitraryType::Ptr osParent, 
	const Rectangle &area) 
{
	++instances;
	if (!osParent)
		return;
	HWND parent = NULL;
	void *_parent = NULL;
	sambag::com::get(osParent, _parent);
	parent = (HWND)_parent;
	if (!parent)
		return;
	HINSTANCE hI = getHInstance();
	// wndclass
	wndClassHolder = WndClassManager::getWndClassHolder(STR_NESTEDWNDCLASS, hI);
	const WNDCLASS & nestedWndClass = wndClassHolder->getWndClass();
	// create window
	DWORD styleEx = 0; //WS_EX_COMPOSITED;
	if (getFlag(WindowFlags::WND_ALWAYS_ON_TOP)) 
	{
		styleEx |= WS_EX_TOPMOST;
	}
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
		ss<<"GetLastError = "<<err;
		throw std::runtime_error(ss.str());
	}

	// register win
	winmap[win] = this;
	visible = true;
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::createWindow(HWND parent) {
	++instances;
	HINSTANCE hI = getHInstance();
	DWORD styleEx = WS_EX_COMPOSITED;
	if (getFlag(WindowFlags::WND_ALWAYS_ON_TOP)) 
	{
		styleEx |= WS_EX_TOPMOST;
	}
	DWORD style = WS_VISIBLE;
	DWORD overlapped = WS_OVERLAPPED								  | 
					   WS_CAPTION									  | 
					   (getFlag(WND_NO_SYSTEM_MENU) ? 0 : WS_SYSMENU) | 
					   (getFlag(WND_RESIZEABLE) ? WS_THICKFRAME : 0)  | 
					   WS_MINIMIZEBOX								  |
					   WS_MAXIMIZEBOX;
	
	style |= getFlag(WND_FRAMED) ? overlapped : WS_POPUP | WS_BORDER; 
	// wndclass
	wndClassHolder = WndClassManager::getWndClassHolder(STR_WNDCLASS, hI);
	const WNDCLASS & wndClass = wndClassHolder->getWndClass();
	// create window
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
	visible = false;
	Ptr hold = self.lock();
	if (hold) { // if not we came from destrucor
		onDestroy();
	}
	unregisterWindow();
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
void Win32WindowImpl::unregisterWindow() {
	if (win==NULL)
		return;
	winmap.erase(win);
	win = 0;
	--instances;
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::_close() {
	if (!visible)
		return;
	invalidateSurface();
	DestroyWindow(win);
	unregisterWindow();
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
	if (win && visible) {
		DestroyWindow(win);
	}
}
//-----------------------------------------------------------------------------
bool Win32WindowImpl::isVisible() const {
	return visible;
}
//-----------------------------------------------------------------------------
Rectangle Win32WindowImpl::getHostBounds() const {
	HWND parent = GetParent(win);
	if (parent) {
		return _getWin32AbsBounds(parent);
	}
	return Rectangle();
}
//-----------------------------------------------------------------------------
Rectangle Win32WindowImpl::getBounds() const {
	return bounds;
}
//-------------------------------------------------------------------------
void Win32WindowImpl::invalidateSurface() {
	surface.reset();
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
	InvalidateRect(win, NULL, ERASE_BG);
}
//-----------------------------------------------------------------------------
void Win32WindowImpl::updateWindowToBounds(const Rectangle &r) {
	if (r.getDimension() != bounds.getDimension()) {
		invalidateSurface();
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
LRESULT CALLBACK Win32WindowImpl::__wndProc_(HWND hWnd, UINT message, 
	WPARAM wParam, LPARAM lParam) 
{
	Win32WindowImpl *win = getWin32WindowImpl(hWnd); //can be NULL
	Win32WindowImpl::Ptr hold;
	if (win)
		hold = win->getPtr();
	
	int mbuttons = 0;
	int x=0; int y=0;

	PAINTSTRUCT ps;
	HDC hdc;

	switch(message) {
	case WM_SETFOCUS:
		break;
	case WM_DESTROY:
		if (!win)
			break;
		win->destroyWindow();
		if (win->getFlag(WindowFlags::EXIT_ON_CLOSE)) 
		{
			PostQuitMessage(0);
		}
		break;
	case WM_CLOSE:
		if (win)
			win->close();
		break;
	case WM_PAINT : {
		if (!win)
			break;
		if (win->getFlag(WindowFlags::WND_RAW)) {
			break;
		}
		hdc = BeginPaint(hWnd, &ps);
		win->update(hdc);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_MOVE : {
		if (!win) {
			break;
		}
		if (win->getFlag(WindowFlags::WND_NESTED)) {
			win->updateWindowToBounds(_getWin32RelBounds(hWnd));
		} else {
			win->updateWindowToBounds(_getWin32AbsBounds(hWnd));
		}
		break;
	}
	case WM_SIZE : {
		if (!win) {
			break;
		}
		if (win->getFlag(WindowFlags::WND_NESTED)) {
			win->updateWindowToBounds(_getWin32RelBounds(hWnd));
		} else {
			win->updateWindowToBounds(_getWin32AbsBounds(hWnd));
		}
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
