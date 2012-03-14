L_SAMBAG = ../../src
L_CPSQLITE = $(L_SAMBAG)/sambag/cpsqlite
L_VECTORN = $(L_SAMBAG)/sambag/math
L_COM = $(L_SAMBAG)/sambag/com
L_XML2OBJ = $(L_SAMBAG)/sambag/xml
L_SQLITE = ../../../sqlite
L_CPPUNIT = ../../../cppunit-1.12.1
L_TESTAPP = ..
L_TICPP = ../../../tinyxml

sambag_tests.exe: DataBase.o \
		sambag_sqlite.o \
		FileSystem.o \
		$(L_TESTAPP)/sambag_tests.cpp \
		$(L_CPSQLITE)/tests/DBTest.cpp \
		$(L_VECTORN)/VectorN.hpp \
		$(L_VECTORN)/tests/VectorNTest.cpp \
		$(L_COM)/tests/FileSystemTest.cpp  \
		$(L_XML2OBJ)/XML2Obj.hpp \
		$(L_XML2OBJ)/tests/XML2ObjTest.cpp \
		$(L_XML2OBJ)/tests/XML2ObjTest.hpp 
		g++ $(L_TESTAPP)/sambag_tests.cpp \
		$(L_TICPP)/ticpp/ticpp.cpp -DTIXML_USE_TICPP\
		$(L_TICPP)/ticpp/tinystr.cpp \
		$(L_TICPP)/ticpp/tinyxml.cpp \
		$(L_TICPP)/ticpp/tinyxmlerror.cpp \
		$(L_TICPP)/ticpp/tinyxmlparser.cpp \
		$(L_CPSQLITE)/tests/DBTest.cpp \
		$(L_VECTORN)/tests/VectorNTest.cpp \
		$(L_COM)/tests/FileSystemTest.cpp \
		$(L_XML2OBJ)/tests/XML2ObjTest.cpp \
		DataBase.o \
		sambag_sqlite.o \
		FileSystem.o \
		-I$(L_SAMBAG) \
		-I$(L_TICPP) \
		-I$(L_SQLITE) \
		-lcppunit \
		-ldl \
		-lboost_filesystem \
		-lboost_signals \
		-o sambag_tests.exe

FileSystem.o: $(L_COM)/FileSystem.hpp \
	     $(L_COM)/FileSystem.cpp 
	     g++ -c $(L_COM)/FileSystem.cpp


DataBase.o : $(L_CPSQLITE)/DataBase.cpp\
	     $(L_CPSQLITE)/DataBase.hpp\
	     $(L_CPSQLITE)/Parameter.hpp 
	     g++ -c $(L_CPSQLITE)/DataBase.cpp -I$(L_SQLITE) \
	     -Lboost_filesystem -I$(L_SAMBAG)

sambag_sqlite.o : $(L_CPSQLITE)/sambag_sqlite.c
	     	  gcc -c $(L_CPSQLITE)/sambag_sqlite.c -I$(L_SQLITE)



	
