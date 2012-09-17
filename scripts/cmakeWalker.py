import sys
import os
import os.path
import re

inDir = "../../src/"

SAMBAG_LIB = "sambag"
SAMBAG_TESTLIB = "sambag_tests"

ignoreDirs = ("TestFolders", "CMakeFiles")

class Walker():
    fHandler = None
    libraryName = SAMBAG_LIB
    currDir = ""
    root = ""
    def __init__(self, root):
        self.root = root
        for currDir, subDirs, files in os.walk(root):
               self.process(currDir, subDirs, files)

    def writeLine(self, string):
        self.fHandler.write("%s\n" % (string))
        
    def createCmakeFile(self):
        return open(self.currDir+"/"+"CMakeLists.txt", "w")

    def addLibrary(self, name, files):
        sources = name#+"_"+"sources"
        self.writeLine("SET (%s" % (sources))
        for x in files:
            self.writeLine("\t%s"%(x))
        self.writeLine(")")
        scmd = "${%s}" % (sources)
        self.writeLine("add_library(%s %s)" % (name, scmd))

    def addSubdir(self, name):
        self.writeLine("add_subdirectory(%s)" % (name))

    def passDir(self, x):
        if x in ignoreDirs:
            return False
        return True
    
    def processSubDirs(self, subDirs):
        for x in subDirs:
            if self.passDir(x):
                self.addSubdir(x)

    def getLibraryName(self):
        res = os.path.relpath(self.currDir, self.root)
        res = res.replace("/", "_")
        return res

    def processFiles(self, files):
        _files = []
        for x in files:
            if not re.match(".*?\.cpp$", x):
                continue
            _files.append(x)
        if len(_files) == 0:
            return
        self.addLibrary(self.getLibraryName(), _files)

    def isTest(self, _dir):
        if re.match(".*?test.*", _dir):
            return True
        return False
    
    def process(self, currDir, subDirs, files):
        self.currDir = currDir
        self.fHandler = self.createCmakeFile()
        #figure out whether test_lib or not
        if self.isTest(currDir):
            self.libraryName = SAMBAG_TESTLIB
        else:
            self.libraryName = SAMBAG_LIB
        #process
        self.processSubDirs(subDirs)
        self.processFiles(files)
        self.fHandler.close()



Walker(inDir)

