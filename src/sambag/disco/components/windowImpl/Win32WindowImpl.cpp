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

#include <Windowsx.h> // GET_X_LPARAM

namespace {
	template <typename T>
	void debugMsg(HWND hwnd, const T &msg) {
		std::cout<<"["<<std::hex<<hwnd<<"]:"
		<<msg<<";"<<std::endl<<std::flush;
	}
	WNDCLASS  nullWndClass = {0};
	const char STR_WNDCLASS[] = "_discowindowimpl";
	const char STR_NESTEDWNDCLASS[] = "_disconestedwindowimpl";
	typedef std::pair<std::string, HINSTANCE> WndClassId;
	struct WndClassHolder {
		typedef boost::shared_ptr<WndClassHolder> Ptr;
		WndClassId wndClassId;
		WNDCLASS wndClass;
		WndClassHolder(const WndClassId &wndClassId) :
		wndClassId(wndClassId), wndClass(nullWndClass) 
		{
		}
		~WndClassHolder() 
		{
			UnregisterClass(wndClassId.first.c_str(), 
				wndClassId.second);
		}
	};
	typedef std::map<WndClassId, WndClassHolder::Ptr> WndClassMap;
	WndClassMap wndClassMap;
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
WNDCLASS & Win32WindowImpl::registerWindowClass(HINSTANCE hI) {
	WndClassId id(std::string(STR_WNDCLASS), hI);
	WndClassMap::iterator it = wndClassMap.find(id);
	if ( it != wndClassMap.end())
		return it->second->wndClass;
	WndClassHolder::Ptr holder(new WndClassHolder(id));
	wndClassMap.insert(std::make_pair(id, holder));

	WNDCLASS &wndClass = holder->wndClass;
	wndClass.lpfnWndProc   = &Win32WindowImpl::wndProc;
	wndClass.hInstance     = hI;
	wndClass.hbrBackground = 0; //(HBRUSH)(COLOR_BACKGROUND);
	wndClass.lpszClassName = STR_WNDCLASS;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	if( FAILED(RegisterClass(&wndClass)) )
		throw std::runtime_error ("Win32 RegisterClass failed.");
	return wndClass;
}
//-----------------------------------------------------------------------------
WNDCLASS & Win32WindowImpl::registerNestedWindowClass(HINSTANCE hI) {
	WndClassId id(std::string(STR_NESTEDWNDCLASS), hI);
	WndClassMap::iterator it = wndClassMap.find(id);
	if ( it != wndClassMap.end())
		return it->second->wndClass;
	WndClassHolder::Ptr holder(new WndClassHolder(id));
	wndClassMap.insert(std::make_pair(id, holder));

	WNDCLASS &nestedWndClass = holder->wndClass;
	nestedWndClass.style = CS_DBLCLKS | CS_GLOBALCLASS;
	nestedWndClass.lpfnWndProc   = &Win32WindowImpl::wndProc;
	nestedWndClass.hInstance     = hI;
	nestedWndClass.lpszClassName = STR_NESTEDWNDCLASS;
	nestedWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	if( FAILED(RegisterClass(&nestedWndClass)) )
		throw std::runtime_error ("Win32 RegisterClass failed.");
	return nestedWndClass;
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
	disco::Win32Surface::Ptr sf = getSurface();
	this->processDraw( sf );
	cairo_surface_flush(sf->getCairoSurface());
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
	WNDCLASS & nestedWndClass = registerNestedWindowClass(hI);

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
	HINSTANCE hI = GetModuleHandle(NULL);
	WNDCLASS & wndClass = registerWindowClass(hI);
	
	DWORD styleEx = WS_EX_TRANSPARENT | WS_EX_COMPOSITED;
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
Rectangle Win32WindowImpl::getBounds() const {
	return bounds;
}
//-------------------------------------------------------------------------
void Win32WindowImpl::invalidateSurface() {
	surface.reset();
}
//-----------------------------------------------------------------------------
disco::Win32Surface::Ptr Win32WindowImpl::getSurface() {
	using namespace sambag;
	if (!surface) {
		Rectangle r = getBounds();
		surface = disco::Win32Surface::create(win,
			(int)r.getWidth(), (int)r.getHeight());
	}
	return surface;
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
LRESULT CALLBACK Win32WindowImpl::wndProc(HWND hWnd, UINT message, 
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
		break;
	case WM_CLOSE:
		if (win)
			win->close();
		break;
	case WM_PAINT : {
		if (!win)
			break;
		if (win->getFlag(WindowFlags::WND_RAW))
			break;
		hdc = BeginPaint(hWnd, &ps);
		win->update();
		EndPaint(hWnd, &ps);
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
