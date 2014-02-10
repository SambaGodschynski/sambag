import sys
import os
import os.path
import re

inDir = "../src/"

add = """
SET ( SAMBAG_TESTAPPSOURCES
	testApps/componentTests/TestApp/src/DiscoView.cpp
)

IF(APPLE)
  add_library(sambag SHARED ${SAMBAG_SOURCES})
  target_link_libraries (sambag ${SAMBAG_CLIBS})
  SET(SAMBAG_EXT_LIBS sambag)
ELSE(APPLE)
  add_library(sambag ${SAMBAG_SOURCES})
  SET(SAMBAG_EXT_LIBS sambag ${SAMBAG_CLIBS})
ENDIF(APPLE)



#unit tests
SET( UNIT_TESTSOURCE testApps/sambag_tests.cpp ${SAMBAG_TESTSOURCES})
#unit test resources
IF(WIN32)
  SET (UNIT_TESTSOURCE ${UNIT_TESTSOURCE} win32_resources/resources.rc)
ENDIF(WIN32)

add_executable(unit_tests ${UNIT_TESTSOURCE})
target_link_libraries (unit_tests ${SAMBAG_EXT_LIBS})

add_executable(test_shm_counterpart sambag/com/tests/test_shm_counterpart.cpp)
target_link_libraries (test_shm_counterpart ${SAMBAG_EXT_LIBS})

#testapp
add_executable(testApp ${SAMBAG_TESTAPPSOURCES})
target_link_libraries (testApp ${SAMBAG_EXT_LIBS})

"""

ignoreDirs = ("TestFolders", 
              "CMakeFiles", 
              ".*testApps.*")

ignoreFiles = ("test_shm_counterpart.cpp","sqlite3.c")

class Walker():
    fHandler = None
    currDir = ""
    root = ""
    testSource = []
    source = []
    mmsource = []
    def __init__(self, root):
        self.root = root
        
        for currDir, subDirs, files in os.walk(root):
               self.process(currDir, subDirs, files)
               
        self.fHandler = self.createCmakeFile()
        self.writeList("SAMBAG_SOURCES", self.source)
        self.writeList("SAMBAG_MMSOURCES", self.mmsource)
        self.writeLine("IF(APPLE)")
        self.writeLine('  set_source_files_properties(${SAMBAG_MMSOURCES} PROPERTIES COMPILE_FLAGS "-x objective-c++")')
        self.writeLine("  SET(SAMBAG_SOURCES ${SAMBAG_SOURCES} ${SAMBAG_MMSOURCES})")
        self.writeLine("ENDIF(APPLE)")
        self.writeList("SAMBAG_TESTSOURCES",self.testSource)
        self.writeLine(add)
        self.fHandler.close()

    def writeList(self, name, data):
        self.writeLine("SET ( %s" % (name))
        for x in data:
            self.writeLine("\t%s" % (x))
        self.writeLine(")")
        
    def writeLine(self, string):
        self.fHandler.write("%s\n" % (string))
        
    def createCmakeFile(self):
        return open(self.root+"/"+"CMakeLists.txt", "w")

    def passDir(self, _dir):
        for x in ignoreDirs:
            if re.match("%s" % (x), _dir):
                return False
        return True

    def passFile(self, _file):
        for x in ignoreFiles:
            if re.match("%s" % (x), _file):
                return False
        return True
    
    def processSubDirs(self, subDirs):
        pass


    def processFiles(self, files):
        for x in files:
            full = ( os.path.relpath(self.currDir, self.root) +'/'+x ).replace("\\","/")
            name, ext = os.path.splitext(x)
            if ext == ".mm":
                self.mmsource.append(full)
            if not re.match("\.cp{0,2}$", ext):
                continue
            if not self.passFile(x):
                continue
            if self.isTest(self.currDir):
                self.testSource.append(full)
            else:
                self.source.append(full)

    def isTest(self, _dir):
        if re.match(".*?test.*", _dir):
            return True
        return False
    
    def process(self, currDir, subDirs, files):
        if not self.passDir(currDir):
            return
        self.currDir = currDir
        #process
        self.processSubDirs(subDirs)
        self.processFiles(files)

w = Walker(inDir)


