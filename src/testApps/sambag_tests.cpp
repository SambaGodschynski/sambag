#include <iostream>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <stdlib.h>


#ifdef WIN32
#include <crtdbg.h>
#define SAMBAG_WINONLY(x) x
#else
#define SAMBAG_WINONLY(x)
#endif



//=============================================================================
//                       CPPUnit TestApp fuer cpsqlite
//=============================================================================
int main ( const int argc, char **argv ) {
	SAMBAG_WINONLY(
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); //VS memory tracking
	)
	using namespace std;
	//using namespace tests;
	cout<<"* cpsqlite TestApp *"<<endl;
	cout<<"==================================="<<endl;
	
	// Informiert Test-Listener ueber Testresultate
    CPPUNIT_NS :: TestResult testresult;

    // Listener zum Sammeln der Testergebnisse registrieren
    CPPUNIT_NS :: TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // Listener zur Ausgabe der Ergebnisse einzelner Tests
    CPPUNIT_NS :: BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // Test-Suite ueber die Registry im Test-Runner einfuegen
    CPPUNIT_NS :: TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS :: TestFactoryRegistry :: getRegistry ().makeTest ());
    testrunner.run (testresult);

    // Resultate im Compiler-Format ausgeben
    CPPUNIT_NS :: CompilerOutputter compileroutputter (&collectedresults, std::cerr);
    compileroutputter.write ();



	// Return error code 1 if the one of test failed.
	return collectedresults.wasSuccessful() ? 0 : 1;
}
