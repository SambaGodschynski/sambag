#!/usr/bin/python
from LuaClassParser import *


class LuaClassBuilder(LuaClassParser):
    def __loadTemplates(self):
        f=open("LuaClass.tmpl.hpp","r")
        self.header=f.read()
        f.close()
        f=open("LuaClass.tmpl.cpp","r")
        self.impl=f.read()
        f.close()
    def build(self, str):
        self.ast = self.parse(str, "class")
        self.__loadTemplates()
        self.__processClass()
        self.__processFunctions()
        print self.header
    def __replaceHeader(self, a, b):
        self.header = self.header.replace(a, b)

    def __processClass(self):
        self.__replaceHeader("$$CLASS_NAME$$", self.ast['name'])
        self.__replaceHeader("$$EXTENDS$$", self.ast['extends'])

    def __processFunctions(self):
        tags=""
        tList=[]
        for x in self.ast['functions']:
            ret = x['return_']
            if not isinstance(ret, basestring):
                ret = "sambag::lua::IgnoreReturn%s" % ret['value']
            tag="SAMBAG_LUA_FTAG(" + x['name'] + ", " + ret + "("
            tList.append(x['name'])
            args=x['args']
            if args != None:
                tag+=args[0]['type']
                for i in range(1,len(args)):
                    tag+= "," + args[i]['type']
            tags+="\t"+tag+"));\n"
        self.__replaceHeader("$$F_TAGS$$", tags)
        if len(tList)==0:
            return
        
        tl="LOKI_TYPELIST%i(" % len(tList)
        tl+=tList[0]
        for i in range(1, len(tList)):
            tl+=", \n\t"+"Frx_"+tList[i]+"_Tag"
        tl+=");"
        self.__replaceHeader("$$F_LISTS$$", tl)
        



f=open("example.luaCpp","r")
txt=f.read();
f.close()
builder=LuaClassBuilder()
builder.build(txt)

