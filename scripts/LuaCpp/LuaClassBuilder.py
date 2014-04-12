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
        return self.header
    def __replaceHeader(self, a, b):
        self.header = self.header.replace(a, b)

    def __processClass(self):
        self.__replaceHeader("$$CLASS_NAME$$", self.ast['name'])
        self.__replaceHeader("$$EXTENDS$$", self.ast['extends'])

    def __preFunct(self, name, form, argform):
        res=[]
        ast = self.ast[name]
        if ast==None:
            return []
        for x in self.ast[name]:
            ret = x['return_']
            if not isinstance(ret, basestring):
                ret = "sambag::lua::IgnoreReturn%s" % ret['value']
            entry=form
            entry=entry.replace("%name",x['name'])
            entry=entry.replace("%type", ret)
            args=x['args']
            if args == None:
                entry=entry.replace("%args","")
            else:
                arglist=argform
                arglist=arglist.replace("%type", args[0]['type'])
                arglist=arglist.replace("%i", "1")
                for i in range(1,len(args)):
                    tmp = "," + argform.replace ("%type", args[i]['type'])
                    tmp = tmp.replace ("%name", args[i]['name'])
                    tmp = tmp.replace ("%i", str(i+1))
                    arglist+=tmp
                entry=entry.replace("%args",arglist)
            res.append(entry)
        return res
    
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
        fs=self.__preFunct('functions', "SAMBAG_LUA_FTAG(%name, %type (%args));", "%type")
        fs=reduce(lambda x,y:"%s\n\t%s"%(x,y), fs)
        self.__replaceHeader("$$F_TAGS$$", fs)
#tags, tList = self.__preFunct('functions')
        #mTags, mtList = self.__preFunct('metaFunctions')
        #tags+=mTags
        #self.__replaceHeader("$$F_TAGS$$", tags)
        #tl=self.__preTlist(tList, "Functions")
        #if len(mtList)>10:
        #    raise Exception("MetaFunctions>10 not supported")
        #tl+=self.__preTlist(mtList, "MetaFunctions")
        #self.__replaceHeader("$$F_LISTS$$", tl)
        



f=open("example.luaCpp","r")
txt=f.read();
f.close()
builder=LuaClassBuilder()
print builder.build(txt)

