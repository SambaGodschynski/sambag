
#include <sambag/disco/components/WindowToolkit.hpp>
#include <sambag/disco/components/Window.hpp>
#include <sambag/com/ArbitraryType.hpp>
#include "TestAppEditor.hpp"
#include <sambag/disco/Geometry.hpp>
#include <sambag/disco/components/Button.hpp>
#include <sambag/disco/components/FlowLayout.hpp>

extern void* hInstance;
//=============================================================================
//	Klasse TestAppEditor:
//=============================================================================
//-----------------------------------------------------------------------------
TestAppEditor::TestAppEditor ( void *ptr ) : AEffEditor((AudioEffect*)ptr) {
	size.left = 0;
	size.top = 0;
	size.right = 1100;
	size.bottom = 400;
}
//-----------------------------------------------------------------------------
TestAppEditor::~TestAppEditor () {
}
//-----------------------------------------------------------------------------
bool TestAppEditor::open( void *ptr ) {
	AEffEditor::open(ptr);
	using namespace sambag::com;
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	ArbitraryType::Ptr pData = createObject(
		std::make_pair(ptr, hInstance)
	);
	sambag::disco::Rectangle _size(Point2D(size.left, size.top), 
		Point2D(size.right, size.bottom)
	);
	window = getWindowToolkit()->createNestedWindow(pData, _size);
	Button::Ptr btn = Button::create();
	btn->setText("Ruebenzahl");
	window->getContentPane()->setLayout(FlowLayout::create());
	window->getContentPane()->add(btn);
	window->validate();
	return true;
}
//-----------------------------------------------------------------------------
void TestAppEditor::close() {
	AEffEditor::close();
}
//-----------------------------------------------------------------------------
bool TestAppEditor::getRect (ERect** rect) {
	*rect = &size;
	return true;
}
//-----------------------------------------------------------------------------
void TestAppEditor::idle() {
	/*if (window)
		window->invalidateWindow();*/
}



