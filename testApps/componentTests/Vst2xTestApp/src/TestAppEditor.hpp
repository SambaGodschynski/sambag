
#ifndef DISCO_TESTAPPEDITOR_H
#define DISCO_TESTAPPEDITOR_H

#include "aeffeditor.h"
#include <sambag/disco/components/Forward.hpp>

//=============================================================================
class TestAppEditor : public AEffEditor {
//=============================================================================
public:
private:
	//-------------------------------------------------------------------------
	sambag::disco::components::WindowPtr window;
	//-------------------------------------------------------------------------
	ERect size;
protected:
	//-------------------------------------------------------------------------
	virtual bool open (void *ptr);
	//-------------------------------------------------------------------------
	virtual void close ();
public:
	//-------------------------------------------------------------------------
	TestAppEditor( void *ptr );
	//-------------------------------------------------------------------------
	virtual ~TestAppEditor();
	//-------------------------------------------------------------------------
	virtual bool getRect (ERect** rect);
};

#endif


