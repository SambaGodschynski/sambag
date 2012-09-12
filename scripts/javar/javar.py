import re
from Tkinter import *
import argparse

"""
todo use inputfile such as:
old1, new1
old2, new2
"""
globalRepl = {}
paramRepl = {}

args = None


def readRepFile(fname):
    d = {}
    f = open(fname, "r")
    l = f.readlines()
    f.close()
    for x in l:
        if re.match("\s*#.*", x): continue
        r = x.split(',')
        if len(r) > 2: continue
        d[r[0].strip()] = r[1].strip()
    return d

def initRepFiles():
    global globalRepl
    global paramRepl
    if not args.globalRep == None:
      globalRepl = readRepFile(args.globalRep)
    if not args.paramRep == None:
      paramRep = readRepFile(args.paramRep)

def readfile(fn):
    f = open(fn, "r")
    txt = f.read()
    f.close()
    return txt

def replace(fdict):
    #global
    g = globalRepl
    for x in fdict.keys():
        for y in g.keys():
            if fdict[x] == None:
                continue
            fdict[x] = fdict[x].replace(y, g[y])
    #param
    p = paramRepl
    for x in p.keys():
        fdict['param'] = fdict['param'].replace(x, p[x])

    return fdict
    

def prepare(txt):
    l=[]
    #remove all comments
    for m in re.finditer('/\*+.*?\*+/', txt,re.DOTALL):
        l.append((m.start(),m.end()))
    for m in re.finditer('//.*', txt):
        l.append((m.start(),m.end()))
    tmp = txt
    for x in l:
        tmp = tmp.replace(txt[x[0]:x[1]], '$')
    f = open("testout.txt", 'w')
    f.write(tmp)
    f.close()
    return tmp


def _out(s, txt):
    return s + txt + '\n'

def _outhpp(data):
    s = ""
    s = _out(s, "\t//-------------------------------------------------------------------------")
    s = _out(s, '\t' + data['comment'])
    s = _out(s, '\t' + data['ret'] + ' ' + data['name'] + '(' + data['param'] + ');')
    return s

def _outcpp(data):
    s = ""
    s = _out(s, "//-----------------------------------------------------------------------------")
    s = _out(s, data['ret'] + ' ' + args.classname + '::' + data['name'] + '(' + data['param'] + ') {')
    s = _out(s, '\tSAMBA_LOG_NOT_YET_IMPL();')
    if not data['ret'] == 'void':
        ret = '0'
        if not re.match('(int|float|double|long|bool)', data['ret']):
            ret = data['ret'] + '()'
        s = _out(s, '\treturn ' + ret + ';')
    s = _out(s, '}')
    return s

def out(l):
    s = ""
    for x in l:
        data=x[3]
        replace(data)
        if args.context == 'hpp':
            s = s + _outhpp(data)
        else:
            s = s + _outcpp(data)
    if args.outfile == None:
        print s
        

def scan(txt):
    s = '\s*'
    S = '\s+'
    scope = '(?P<scope>public|protected|private){0,1}'
    static = '(?P<static>static){0,1}'
    ident = '[a-zA-Z0-9_-]+'
    _type = '([a-zA-Z0-9_\[\]-]+)'
    ret = '(?P<ret>'+_type+')'
    param = '.*?'#'('+s+ _type + S + ident + ",{0,1})+"
    paraml = '\((?P<param>'+s+param+s+')\)'
    l = []
    name='(?P<name>'+ident+')'
    tmp = txt#prepare(txt)
    comment = '(?P<comment>/\*+.*?\*+/){0,1}\s*'
    f = '(?P<f>'+scope+s+static+s+ret+S+name+s+paraml+')'
    for m in re.finditer(comment+f, tmp, re.DOTALL):
        d = {}
        d['scope'] = m.group('scope')
        d['static'] = m.group('static')
        d['ret'] = m.group('ret')
        d['comment'] = m.group('comment')
        d['name'] = m.group('name')
        d['param'] = m.group('param')
        l.append((m.group('f').strip(), m.start(), m.end(), d))
    return l
    
class mywidgets:
    def __init__(self,root):
        frame=Frame(root)
        self.listfr(frame, "outlist")
        self.makeMenuBar(frame)
        frame.pack()
        return
        
    def listfr(self, frame, name):
        textfr = Frame(frame)
        text = Listbox(textfr,height = 30,width = 80,background='white',
                       selectmode=MULTIPLE, state=NORMAL)
        setattr(self, name, text)
        scroll = Scrollbar(textfr)
        text.configure(yscrollcommand = scroll.set)
        text.pack(side = LEFT)
        scroll.pack(side = RIGHT,fill = Y)
        textfr.pack(side = TOP)

    def setdata(self, data):
        for x in data:
            self.outlist.insert("end", x[0])
        self.data = data
    def doIt(self):
        il = map(int, self.outlist.curselection())
        l = []
        for x in il:
            l.append( self.data[x] )
        out(l)
            
    def makeMenuBar(self,frame):
        menubar = Frame(frame,relief = RAISED,borderwidth = 1)
        menubar.pack()
        mb = Button(menubar,text = 'do it!', command = self.doIt)
        mb.pack(padx = 25,side = RIGHT)

parser = argparse.ArgumentParser(description=
"""
Pareses Java files for class methods and extract these to an appropriated
c++ format (which I prefer). Note:
The parser far away from being perfect.
""")
parser.add_argument('--classname','-n',  
		dest='classname', 
		help='the classname', 
		required=True)
parser.add_argument('--in', '-i', 
		dest='infile', 
		help='filename for input',
                required=True
                )
parser.add_argument('--out', '-o', 
		dest='outfile', 
		help='filename for output', 
		required=False)
parser.add_argument('--context', '-c', 
		dest='context', 
		help='hpp / cpp',
                choices=['hpp', 'cpp'],
		required=True)
parser.add_argument('--globalRep', '-g', 
		dest='globalRep', 
		help='the global replacment file')

parser.add_argument('--paramRep', '-p', 
		dest='paramRep', 
		help='the param replacment file')


args = parser.parse_args()
initRepFiles()
root = Tk()
mainApp = mywidgets(root)
root.title('javaextract')

txt = readfile(args.infile)
l = scan(txt)
mainApp.setdata(l)

root.mainloop()
