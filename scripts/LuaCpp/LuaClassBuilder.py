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

    def __preFunct(self, name):
        tags=""
        tList=[]
        ast = self.ast[name]
        if ast==None:
            return "", []
        for x in self.ast[name]:
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
        return tags, tList
    
    def __preTListRange(self, l, s, lgt):
        if len(l)==0:
            return "Loki::NullType"
        tl="LOKI_TYPELIST%i(" % lgt
        tl+="Frx_"+l[0]+"_Tag"
        for i in range(s, s+lgt-1):
            tl+=", \n\t"+"Frx_"+l[i]+"_Tag"
        tl+=")"
        return tl

    def __preTlist(self,l, name):
        i=0
        tl=""
        while (True):
            r=len(l)-i
            if r<=0:
                break
            lgt=min(10, r)
            tl+="typedef " + self.__preTListRange(l,i,lgt) + " %s%i;" % (name,(i/10+1))
            tl+="\n\n\t"
            i+=lgt
        return tl
  
    def __processFunctions(self):
        tags, tList = self.__preFunct('functions')
        mTags, mtList = self.__preFunct('metaFunctions')
        tags+=mTags
        self.__replaceHeader("$$F_TAGS$$", tags)
        tl=self.__preTlist(tList, "Functions")
        if len(mtList)>10:
            raise Exception("MetaFunctions>10 not supported")
        tl+=self.__preTlist(mtList, "MetaFunctions")
        self.__replaceHeader("$$F_LISTS$$", tl)
        



f=open("example.luaCpp","r")
txt=f.read();
f.close()
builder=LuaClassBuilder()
builder.build(txt)

