#!/usr/bin/python
'''
purpose:
    creates new .h and .cpp file using .txt
    templates. Solution is quick and dirty, so the script
    has to be configured by variables below before
    executing.
'''
import sys
import argparse
import time
import os.path

name = ""
inh = ''
incpp = ''
outdir = ""
repmap = {}


def configGlobal(args):
	global name
	global outdir
	global repmap
	global incpp
	global inh	
	name = args.classname
	outdir = args.outdir
	incpp = args.inCpp
	inh = args.inHeader
	repmap = {'$$$AUTHOR$$$': 'Johannes Unger',
    	'$$$NAME$$$': name,
        '$$$ONCE$$$': 'SAMBAG_' + name.upper() + '_H',
	'$$$DATE$$$': time.asctime()
    }
def readFile(name):
    #returns file as string
    f = open(name, 'r')
    txt = f.read()
    f.close()
    return txt
def writeFile(name, txt):
    #writes txt into name
    if os.path.exists(name):
	print name + " already exists"
	return
    print "writing " + name + "."
    f = open(name, 'w')
    f.write(txt)
    f.close()

def processFile( infile, outfile):
    txt = readFile(infile)
    for k, v in repmap.iteritems():
        txt = txt.replace(k, v);
    writeFile(outdir + outfile, txt)

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description='creates class')
	parser.add_argument('--classname','-n',  
		dest='classname', 
		help='the classname', 
		required=True)
	parser.add_argument('--out', '-o', 
		dest='outdir', 
		help='the outdir', 
		required=True)
	parser.add_argument('--inHeader', 
		dest='inHeader', 
		help='tempate file for header', 
		default='header.txt')
	parser.add_argument('--inCpp', 
		dest='inCpp', 
		help='tempate file for cpp', 
		default='cpp.txt')

	args = parser.parse_args()
	configGlobal(args)
	processFile(inh, name + '.hpp')
	if len(incpp) > 0:
		processFile(incpp, name + '.cpp')	
	print('all done.')

