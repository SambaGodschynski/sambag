import sys
import os
import os.path
import re

inDir = "../src/"

add = """
SET ( SAMBAG_TESTAPPSOURCES
	testApps/componentTests/TestApp/src/DiscoView.cpp
)
#unit tests
SET( UNIT_TESTSOURCE testApps/sambag_tests.cpp ${SAMBAG_TESTSOURCES})
#unit test resources
IF(WIN32)
  SET (UNIT_TESTSOURCE ${UNIT_TESTSOURCE} win32_resources/resources.rc)
ENDIF(WIN32)

add_executable(unit_tests ${UNIT_TESTSOURCE})
target_link_libraries (unit_tests sambag ${SAMBAG_CLIBS})
#testapp
add_executable(testApp ${SAMBAG_TESTAPPSOURCES})
target_link_libraries (testApp sambag ${SAMBAG_CLIBS})

"""

ignoreDirs = ("TestFolders", "CMakeFiles", ".*testApps.*")

class Walker():
    fHandler = None
    currDir = ""
    root = ""
    testSource = []
    source = []
    def __init__(self, root):
        self.root = root
        
        for currDir, subDirs, files in os.walk(root):
               self.process(currDir, subDirs, files)
               
        self.fHandler = self.createCmakeFile()
        self.writeList("SAMBAG_SOURCES", self.source)
        self.writeList("SAMBAG_TESTSOURCES",self.testSource)
        self.writeLine("add_library(sambag ${SAMBAG_SOURCES})")
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
    
    def processSubDirs(self, subDirs):
        pass


    def processFiles(self, files):
        for x in files:
            full = os.path.relpath(self.currDir, self.root) +'/'+x
            if not re.match(".*?\.cp{0,2}$", x):
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


