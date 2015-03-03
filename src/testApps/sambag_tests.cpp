#include <iostream>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <stdlib.h>
#include <sambag/com/UnitEx.hpp>

#ifdef WIN32
#include <crtdbg.h>
#define SAMBAG_WINONLY(x) x
#else
#define SAMBAG_WINONLY(x)
#endif


#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.VC90.CRT' version='9.0.21022.8' processorArchitecture='X86' publicKeyToken='1fc8b3b9a1e18e3b' language='*'\"")


//=============================================================================
//                       CPPUnit TestApp fuer cpsqlite
//=============================================================================
int main ( const int argc, char **argv ) {
	SAMBAG_WINONLY(
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); //VS memory tracking
	)
    
    sambag::com::unitex::processArguments(argc, argv);
	using namespace std;
	//using namespace tests;
	cout<<"* cpsqlite TestApp *"<<endl;
	cout<<"==================================="<<endl;
	if (sambag::com::unitex::ignoreKnownIssues) {
        cout<<"! ignoring known issues !"<<endl;
    }
    
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


    // important stuff happens next
   std::ofstream xmlFileOut("sambag_testresult.xml");
   CPPUNIT_NS::XmlOutputter xmlOut(&collectedresults, xmlFileOut);
   xmlOut.write();

	return 0;
}
