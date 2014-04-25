#!/usr/bin/python
from LuaClassParser import *
import re
import time
import argparse
import sys

class LuaClassBuilder(LuaClassParser):
    def __loadTemplates(self):
        p=sys.path[0]
        f=open(p+"/LuaClass.tmpl.hpp","r")
        self.header=f.read()
        f.close()
        f=open(p+"/LuaClass.tmpl.cpp","r")
        self.impl=f.read()
        f.close()
    def build(self, str):
        self.ast = self.parse(str, "class")
        self.__loadTemplates()
        self.__processClass()
        self.__processFunctionsHeader()
        self.__processFields()
        self.__processNS()
        self.__processFunctionsImpl()
        return self.header, self.impl
    def __replaceHeader(self, a, b):
        self.header = self.header.replace(a, b)

    def __replaceImpl(self, a, b):
        self.impl = self.impl.replace(a, b)

    def __replace(self, a, b):
        self.__replaceHeader(a,b)
        self.__replaceImpl(a,b)

    def __getComments(self, l):
        res=[]
        for x in l:
            if len(x['docComment'])>0:
                res+=x['docComment']
        res = filter(lambda x: len(x)>0, res)
        return res

    def __getCommentStr(self, l):
        c = self.__getComments(l)
        if len(c)==0:
            return "* @brief TODO"
        return reduce(lambda x,y: "%s\n\t%s" % (x,y), c)

    def __processClass(self):
        self.__replace("$$CLASS_NAME$$", self.ast['name'])
        self.__replaceHeader("$$EXTENDS$$", reduce(lambda x,y: "%s::%s" % (x,y), self.ast['extends']))
        includes = self.ast['includes']
        includes = map(lambda x: "#include <%s>" % x, includes)
        self.__replaceHeader("$$INCLUDE$$", reduce(lambda x,y: "%s\n%s" % (x,y), includes))
        self.__replace("$$DATE$$", time.asctime())


    def __processArgs(self, toReplace, entry, args, argform):
        if args == None:
            entry=entry.replace(toReplace,"")
            #entry=entry.replace("%,", "")
            entry = re.sub("%, *", "", entry)
        else:
            arglist=argform.strip()
            arglist=arglist.replace("%typeref", self.__getRefType(self.__getType(args[0]['type'])))
            arglist=arglist.replace("%type", self.__getType(args[0]['type']))
            arglist=arglist.replace("%name", args[0]['name'])
            arglist=arglist.replace("%i", "1")
            for i in range(1,len(args)):
                type=self.__getType(args[i]['type'])
                tmp = ", " + argform.replace ("%typeref", self.__getRefType(type))
                tmp = tmp.replace ("%type", type)
                tmp = tmp.replace ("%name", args[i]['name'])
                tmp = tmp.replace ("%i", str(i+1))
                arglist+=tmp
            entry=entry.replace(toReplace, arglist)
            entry=entry.replace("%,",",")
        return entry

    def __preFunct(self, name, form, argform, argforms=[]):
        res=[]
        ast = self.ast[name]
        if ast==None:
            return []
        for x in self.ast[name]:
            ret = x['return_']
            if not isinstance(ret, basestring):
                ret = "sambag::lua::IgnoreReturn%s" % ret['value']
            entry=form
            entry=entry.replace("%comment", "/**\n\t" + self.__getCommentStr(x['comment']) + "\n\t*/")
            entry=entry.replace("%name",x['name'])
            entry=entry.replace("%type", ret)
            if ret=="void":
                entry=entry.replace("%return", "")
            else:
                entry=entry.replace("%return", "return")
            args=x['args']
            i=2
            for x in argforms:
                entry=self.__processArgs("%args"+str(i), entry, args, x)
                i+=1
            entry=self.__processArgs("%args", entry, args, argform)
            res.append(entry)
        return res

    def __processNS(self):
        ast = self.ast['namespace']
        if ast==None:
            return
        ast=ast['ns']
        if ast==None:
            return
        ns=""
        c=0
        for x in ast:
            ns+="namespace " + x + " { "
            c+=1
        self.__replace("$$NS$$", ns)
        self.__replace("$$NS_END$$", "}"*c)
        
    def __getRefType(self, type):
        if type=="std::string":
            return "const %s &" % type
        return type
        
    def __getType(self, type):
        if type=="string":
            return "std::string"
        return type

    def __preFields(self, name, form):
        res=[]
        ast = self.ast[name]
        if ast==None:
            return []
        res=[]
        for x in self.ast[name]:
            entry=form
            entry=entry.replace("%name",x['name'])
            type=self.__getType(x['type'])
            entry=entry.replace("%typeref", self.__getRefType(type))
            entry=entry.replace("%type", type)
            entry=entry.replace("%value", x['value'])
            res.append(entry)
        return res

    
    def __preTListRange(self, l, s, lgt):
        e=min(len(l), s+lgt+1)
        if e<=s:
            return "Loki::NullType()"

        tl="LOKI_TYPELIST%i(" % lgt
        tl+="Frx_"+l[s]+"_Tag"
        for i in range(s+1, e):
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
  
    def __processFunctionsHeader(self):
        #ftags
        fs=self.__preFunct('functions', "SAMBAG_LUA_FTAG(%name, %type (%args));", "%type")
        fs=reduce(lambda x,y:"%s\n\t%s"%(x,y), fs)
        self.__replaceHeader("$$F_TAGS$$", fs)
        #type lists
        tags=self.__preFunct('functions', "%name", "")
        tl=self.__preTlist(tags, "Functions")
        self.__replaceHeader("$$F_LISTS$$", tl)
        #fdefs
        fs=self.__preFunct('functions', "%comment\n\tvirtual %type %name(lua_State *lua%, %args) = 0;", "%typeref %name")
        fs=reduce(lambda x,y: "%s\n\t%s"%(x,y), fs)
        self.__replaceHeader("$$F_IMPL$$", fs)
        #lc impl
        fs=self.__preFunct('lcFDefs', "bool script_impl_%name();", "")
        fs=reduce(lambda x,y:"%s\n\t%s"%(x,y), fs)
        self.__replaceHeader("$$LUA_CALL_IMPL$$", fs)
        #lc call
        auto="%type %name(lua_State *lua%, %args);"
        man="void raw_%name(lua_State *lua, int narg, int nret);"
        fs=self.__preFunct('lcFDefs', auto+"\n\t"+man, "%typeref %name")
        fs=reduce(lambda x,y:"%s\n\t%s"%(x,y), fs)
        self.__replaceHeader("$$LUA_CALL_FS$$", fs)

    def __processFields(self):
        cname = self.ast['name']
        fields=self.__preFields('fields', "virtual %type get_%name(lua_State *lua);")
        fields+=self.__preFields('fields', "virtual set_%name(lua_State *lua, %typeref value);")
        fields=reduce(lambda x,y:"%s\n\t%s"%(x,y), fields)
        self.__replaceHeader("$$FIELDS$$", fields)
        #setter impl
        f="""void %s::set_%s(lua_State *lua, %s value) {
        using namespace sambag::lua;
        push(lua, value);
        lua_setfield(lua, -1, "%s");
}
        """ % (cname, "%name", "%typeref", "%name")
        setter=self.__preFields('fields', f)
        setter=reduce(lambda x,y:"%s\n%s"%(x,y), setter)
        
        #getter impl
        f="""%s %s::get_%s(lua_State *lua) {
        using namespace sambag::lua;
        lua_getfield(lua, -1, "%s");
        boost::tuple<%s> res;
        pop(lua, res);
        return boost::get<0>(res);
}
        """ % ("%type", cname, "%name", "%name", "%type")
        getter=self.__preFields('fields', f)
        getter=reduce(lambda x,y:"%s\n%s"%(x,y), getter)
        
        getsetter=setter+"\n"+getter
        self.__replaceImpl("$$FIELD_SETTER_GETTER$$", getsetter)
        #field init
        fields=self.__preFields('fields', "set_%name(lua, %value);")
        fields=reduce(lambda x,y:"%s\n\t%s"%(x,y), fields)
        self.__replaceImpl("$$LUA_INIT_FIELDS$$", fields)
        

    def __processBinds(self, fs, s, e):
        e=min(len(fs), e+1)
        if e<=s:
            return "boost::make_tuple()"
        strFs="boost::make_tuple(" + fs[s]
        for i in range(s+1,e):
            strFs+=",\n\t\t" + fs[i]
        return strFs+")"

    def __processFunctionsImpl(self):                            
        #binding
        cname = self.ast['name']
        fs=self.__preFunct('functions', "boost::bind(&"+cname+"::%name, self%, %args)", "_%i")
        num=len(fs)/10
        regs=""
        unregs=""
        for i in range(0,num+1):
            binds = self.__processBinds(fs, i*10,i*10+9)
            regs+="registerClassFunctions<Functions%i, TupleAcessor>(\n\tlua,\n\t" % (i+1)
            regs+=binds + ",\n\t"
            regs+="index, \n\t"
            regs+="getUId() \n\t"
            regs+="); \n\n\t"
            unregs+="unregisterClassFunctions<Functions%i>(obj->getUId());\n\t" % (i+1)
        unregs+="unregisterClassFunctions<MetaFunctions>(obj->getUId());\n\t"
        self.__replaceImpl("$$LUA_REGISTER$$", regs)
        self.__replaceImpl("$$LUA_UNREGISTER$$", unregs)
        #lc impl
        impl="""
{
        return sambag::lua::hasFunction(lua, "%name");
}
"""
        fs=self.__preFunct('lcFDefs', "bool "+cname+"::script_impl_%name()" + impl, "")
        fs=reduce(lambda x,y:"%s\n%s"%(x,y), fs)
        self.__replaceImpl("$$LUA_CALL_IMPL$$", fs)
        #lc call
        impl="""
{
        %return __CallImpl<%type>::hauRein(lua, "%name", boost::tuple<%args2>(%args3));
}
"""
        auto="%type " + cname +  "::%name(lua_State *lua%, %args)" + impl
        impl="""
{
        lua_getglobal(lua, "%name");
        if (!lua_isfunction(lua, -1)==1) {
            throw sambag::lua::NoFunction();
        }
        if (lua_pcall(lua, narg, nret, 0)!=0) {
            throw sambag::lua::ExecutionFailed(std::string(lua_tostring(lua, -1)));
        }    
}
"""
        man="void " + cname + "::raw_%name(lua_State *lua, int narg, int nret)" + impl
        fs=self.__preFunct('lcFDefs', auto+"\n"+man, "%typeref %name", ["%type", "%name"])
        fs=reduce(lambda x,y:"%s\n%s"%(x,y), fs)
        self.__replaceImpl("$$LUA_CALL_FS$$", fs)
        
        
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='LuaCpp class builder')
    parser.add_argument("input", help="the input file")
    parser.add_argument("--otype", choices=['header', 'impl', 'doc'], nargs="*")
    args = parser.parse_args()
    f=open(args.input,"r")
    txt=f.read();
    f.close()
    builder=LuaClassBuilder()
    header, impl = builder.build(txt)
    for x in args.otype:
        if x=="header":
            print header
        if x=="impl":
            print impl
