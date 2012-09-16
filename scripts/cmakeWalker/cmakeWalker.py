import sys
import os

inDir = "../../src/"


def createCmakeFile(currDir):
    return open(currDir+"/"+"CMakeLists.txt", "w")

def processSubDirs(currDir, subDir):
    pass

def processFiles(currDir, files):
    pass

def process(currDir, subDirs, files):
    processSubDirs(currDir, files)
    processFiles(currDir, files)



def walk(root):
    for currDir, subDirs, files in os.walk(root):
        process(currDir, subDirs, files)

walk(inDir)

