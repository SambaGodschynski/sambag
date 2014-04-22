#!/usr/bin/python
from LuaClassParser import *
import re

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
    def __processClass(self):
        self.__replace("$$CLASS_NAME$$", self.ast['name'])
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
                #entry=entry.replace("%,", "")
                entry = re.sub("%, *", "", entry)
            else:
                arglist=argform.strip()
                arglist=arglist.replace("%type", args[0]['type'])
                arglist=arglist.replace("%name", args[0]['name'])
                arglist=arglist.replace("%i", "1")
                for i in range(1,len(args)):
                    tmp = ", " + argform.replace ("%type", args[i]['type'])
                    tmp = tmp.replace ("%name", args[i]['name'])
                    tmp = tmp.replace ("%i", str(i+1))
                    arglist+=tmp
                entry=entry.replace("%args",arglist)
                entry=entry.replace("%,",",")
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
        self.__replace("$$NS_END$$", "}"*c + " // namespace(s)")
        
    def __preFields(self, name, form):
        res=[]
        ast = self.ast[name]
        if ast==None:
            return []
        res=[]
        for x in self.ast[name]:
            entry=form
            entry=entry.replace("%name",x['name'])
            entry=entry.replace("%type", x['type'])
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
        fs+=self.__preFunct('metaFunctions', "SAMBAG_LUA_FTAG(%name, %type (%args));", "%type")
        fs=reduce(lambda x,y:"%s\n\t%s"%(x,y), fs)
        self.__replaceHeader("$$F_TAGS$$", fs)
        #type lists
        tags=self.__preFunct('functions', "%name", "")
        mTags=self.__preFunct('metaFunctions',"%name", "")
        if len(mTags)>10:
            raise Exception("MetaFunctions>10 not supported")
        tl=self.__preTlist(tags, "Functions")
        if len(mTags)==0:
            tl+="typedef Loki::NullType MetaFunctions;\n\t"
        else:
            tl+=self.__preTlist(mTags, "MetaFunctions")  
        self.__replaceHeader("$$F_LISTS$$", tl)
        #fdefs
        fs=self.__preFunct('functions', "virtual %type %name(lua_State *lua%, %args) = 0;", "%type %name")
        fs+=self.__preFunct('MetaFunctions', "virtual %type %name(lua_State *lua%, %args) = 0;", "%type %name")
        fs=reduce(lambda x,y: "%s\n\t%s"%(x,y), fs)
        self.__replaceHeader("$$F_IMPL$$", fs)

    def __processFields(self):
        cname = self.ast['name']
        fields=self.__preFields('fields', "virtual %type get_%name(lua_State *lua);")
        fields+=self.__preFields('fields', "virtual set_%name(lua_State *lua, %type value);")
        fields=reduce(lambda x,y:"%s\n\t%s"%(x,y), fields)
        self.__replaceHeader("$$FIELDS$$", fields)
        #setter impl
        lsetter=self.__preFields('fields', "%type " + cname + "::get_%name(lua_State *lua)")
        setter=""
        for x in lsetter:
            setter+="\n//" + "-"*78+"\n"
            setter+= x+"\n{"
            setter+= "\n}"
        #getter impl
        lgetter=self.__preFields('fields', "void " + cname + "::set_%name(lua_State *lua, %type value)")
        getter=""
        for x in lgetter:
            getter+="\n//" + "-"*78+"\n"
            getter+= x+"\n{"
            getter+= "\n}"
        getsetter=setter+getter
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
        cname = self.ast['name']
        fs=self.__preFunct('functions', "boost::bind(&"+cname+"::%name, self%, %args)", "_%i")
        mfs=self.__preFunct('metaFunctions', "boost::bind(&"+cname+"::%name, self%, %args)", "_%i")
        binds = self.__processBinds(fs, 0,9)
        mBinds = self.__processBinds(mfs, 0,9)
        regs="""int index = createClass<Functions1, MetaFunctions, TupleAccessor>(
        lua,
        """
        regs+=binds + ",\n\t"
        regs+=mBinds + ", \n\t"
        regs+="getUId() \n\t"
        regs+="); \n\n\t"

        #remain functions
        num=len(fs)/10
        unregs = "unregisterClassFunctions<Functions1>(obj->getUId());\n\t"
        for i in range(0,num):
            binds = self.__processBinds(fs, (i+1)*10,(i+1)*10+9)
            regs+="registerClassFunctions<Functions%i, TupleAcessor>(\n\tlua,\n\t" % (i+2)
            regs+=binds + ",\n\t"
            regs+="index, \n\t"
            regs+="getUId() \n\t"
            regs+="); \n\n\t"
            unregs+="unregisterClassFunctions<Functions%i>(obj->getUId());\n\t" % (i+2)
        unregs+="unregisterClassFunctions<MetaFunctions>(obj->getUId());\n\t"
        self.__replaceImpl("$$LUA_REGISTER$$", regs)
        self.__replaceImpl("$$LUA_UNREGISTER$$", unregs)

f=open("example.luaCpp","r")
txt=f.read();
f.close()
builder=LuaClassBuilder()
header, impl = builder.build(txt)
print header
print impl
