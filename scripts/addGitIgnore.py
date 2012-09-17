import sys
import os
import os.path
import re

root = "../src/"

SAMBAG_LIB = "sambag"
SAMBAG_TESTLIB = "sambag_tests"

ignore = ["CMakeFiles"]

def process(currDir):
    _dir = os.path.abspath(currDir)
    print "write %s/.gitignore" %(_dir, )
    f = open(_dir+"/.gitignore", "r")
    l = f.readlines()
    f.close()
    tmp = []
    for x in l:
        if re.match("\s*$", x):
            continue
        if x.strip() in ignore:
            continue
        tmp.append(x)
    for x in ignore:
        tmp.append(x)
    f = open(_dir+"/.gitignore", "w")
    for x in tmp:
        f.write(x+'\n')
    f.close()

for currDir, subDirs, files in os.walk(root):
    if re.match(".*?CMakeFiles", currDir):
        continue
    process(currDir)




