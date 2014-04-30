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
        f=open(p+"/LuaClass.tmpl.doc","r")
        self.doc=f.read()
        f.close()
    def build(self, str):
        self.ast = self.parse(str, "class")
        self.__loadTemplates()
        self.__processClass()
        self.__processFunctionsHeader()
        self.__processFields()
        self.__processNS()
        self.__processFunctionsImpl()
        self.__processUserDefs()
        self.__processDoc()
        
    def __replaceHeader(self, a, b):
        self.header = self.header.replace(a, b)

    def __replaceImpl(self, a, b):
        self.impl = self.impl.replace(a, b)

    def __replaceDoc(self, a, b):
        self.doc = self.doc.replace(a, b)

    def __replace(self, a, b):
        self.__replaceHeader(a,b)
        self.__replaceImpl(a,b)

    def __getComments(self, l):
        res=[]
        for x in l:
            try:
                res+=x['docComment']
            except:
                pass
        res = filter(lambda x: len(x)>0, res)
        return res

    def __getCommentStr(self, l):
        c = self.__getComments(l)
        if len(c)==0:
            return "* @brief TODO"
        return reduce(lambda x,y: "%s\n\t%s" % (x,y), c)

    def __processDocLinks(self, str, form):
        it=re.finditer("@see ([^ ]*)", str)
        res=str
        for m in it:
            s=form.replace("%link", m.group(1))
            res=res.replace(m.group(0), s)
        return res

    def __extractTags(self, l, linkform):
        #return tagmap
        l=self.__getComments(l)
        if len(l)==0:
            return {}
        #preprocess lines
        nl=[]
        for x in l:
            m=re.match("\\**(.*)", x)
            x=m.group(1)
            if x == None:
                continue
            x=self.__processDocLinks(x, linkform)
            nl.append(x)
        l=nl
        res={}
        l=reduce(lambda x,y: "%s %s" %(x,y), l)
        it=re.finditer("@(\w+)(.*?)(?=@|$)", l)
        for m in it:
            k=m.group(1)
            if not res.has_key(k):
                res[k]=[]
            res[k].append(m.group(2))
        return res

    def __processDoc(self):
        self.__replaceDoc("$$CLASS_NAME$$", self.ast['name'])
        linkform="[%link]"
        l=self.__extractTags(self.ast['comment'], linkform)
        brief=""
        if l['brief']!=None:
            brief = reduce(lambda x,y:"%s %s"%(x,y), l['brief'])
        self.__replaceDoc("$$CLASS_DOC$$", brief)
        #functions
        fs=""
        argform="%type %name"
        argdocform="%name %doc"
        form="""%name %args
        %fdoc
        %argdoc
        """
        for x in self.ast['functions']:
            f=form
            f=f.replace("%name", x['name'])
            l=self.__extractTags(x['comment'], linkform)
            brief=""
            if l.has_key("brief"):
                brief = reduce(lambda x,y:"%s %s"%(x,y), l['brief'])
            f=f.replace("%fdoc", brief)
            if x['args'] !=None:
                if (l.has_key("param")):
                    adlist=l['param']
                else:
                    adlist=[]
                arg=""
                darg=""
                for y in x['args']:
                    tmp_arg=argform
                    tmp_darg=argdocform
                    tmp_arg=tmp_arg.replace("%name", y['name'])
                    tmp_darg=tmp_darg.replace("%name", y['name'])
                    tmp_arg=tmp_arg.replace("%type", y['type'])
                    if len(adlist)>0:
                        tmp_darg=tmp_darg.replace("%doc", adlist[0])
                        adlist.pop(0)
                    else:
                        tmp_darg=tmp_darg.replace("%doc","")
                    arg+=tmp_arg+" "
                    darg+=tmp_darg+" \n\t"
                f=f.replace("%args", arg)
                f=f.replace("%argdoc", darg)
            else:
                f=f.replace("%args", "")
                f=f.replace("%argdoc", "")
            fs+=f
        self.__replaceDoc("$$FUNCTIONS$$", fs)
        #fields
        for x in self.ast['fields']:
            l=self.__extractTags(x['comment'], linkform)

    def __processClass(self):
        self.__replace("$$CLASS_NAME$$", self.ast['name'])
        self.__replaceHeader("$$EXTENDS$$", reduce(lambda x,y: "%s::%s" % (x,y), self.ast['extends']))
        includes = self.ast['includes']
        if includes != None:
            includes = map(lambda x: "#include <%s>" % x, includes)
            self.__replaceHeader("$$INCLUDE$$", reduce(lambda x,y: "%s\n%s" % (x,y), includes))
        else:
            self.__replaceHeader("$$INCLUDE$$", "")
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

    def __getRetType(self, x):
        ret=x
        if not isinstance(ret, basestring):
            if ret['type'] == "ManuallyReturn":
                val = int(ret['value'])
                if val==1:
                    val=""
                else:
                    val=str(val)
                ret = "sambag::lua::IgnoreReturn%s" % val
            elif ret['type'] == "User":
                ret = ret['value']
        ret=self.__getType(ret)
        return ret

    def __preFunct(self, name, form, argform, argforms=[]):
        res=[]
        ast = self.ast[name]
        if ast==None:
            return []
        for x in self.ast[name]:
            ret = x['return_']
            ret = self.__getRetType(ret)
            entry=form
            entry=entry.replace("%comment", "/**\n\t" + self.__getCommentStr(x['comment']) + "\n\t*/")
            entry=entry.replace("%name",x['name'])
            entry=entry.replace("%type", ret)
            entry=entry.replace("%typeref", self.__getRefType(ret))
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
        e=min(len(l), s+lgt)
        if e<=s:
            return "Loki::NullType()"

        tl="LOKI_TYPELIST_%i(" % lgt
        tl+="Frx_"+l[s]+"_Tag"
        for i in range(s+1, e):
            tl+=", \n\t"+"Frx_"+l[i]+"_Tag"
        tl+=")"
        return tl

    def __preTlist(self,l, name):
        if len(l)==0:
            return "typedef Loki::NullType Functions1;"
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
        if len(fs)==0:
            fs.append("")
        fs=reduce(lambda x,y:"%s\n\t%s"%(x,y), fs)
        self.__replaceHeader("$$F_TAGS$$", fs)
        #type lists
        tags=self.__preFunct('functions', "%name", "")
        tl=self.__preTlist(tags, "Functions")
        self.__replaceHeader("$$F_LISTS$$", tl)
        #fdefs
        fs=self.__preFunct('functions', "%comment\n\tvirtual %type %name(lua_State *lua%, %args) = 0;", "%typeref %name")
        if len(fs)==0:
            fs.append("")
        fs=reduce(lambda x,y: "%s\n\t%s"%(x,y), fs)
        self.__replaceHeader("$$F_IMPL$$", fs)

    def __processFields(self):
        cname = self.ast['name']
        fields=[]
        fields+=self.__preFields('fields', "virtual %type get_%name(lua_State *lua, int index);")
        fields+=self.__preFields('fields', "virtual void set_%name(lua_State *lua, %typeref value, int index);")
        if len(fields)==0:
             self.__replaceHeader("$$FIELDS$$", "")
             self.__replaceImpl("$$FIELD_SETTER_GETTER$$", "")
             self.__replaceImpl("$$LUA_INIT_FIELDS$$", "")
             return
        fields=reduce(lambda x,y:"%s\n\t%s"%(x,y), fields)
        self.__replaceHeader("$$FIELDS$$", fields)
        #setter impl
        f="""void %s::set_%s(lua_State *lua, %s value, int index) {
    using namespace sambag::lua;
    push(lua, value);
    lua_setfield(lua, index, "%s");
}
        """ % (cname, "%name", "%typeref", "%name")
        setter=self.__preFields('fields', f)
        setter=reduce(lambda x,y:"%s\n%s"%(x,y), setter)
        
        #getter impl
        f="""%s %s::get_%s(lua_State *lua, int index) {
    using namespace sambag::lua;
    lua_getfield(lua, index, "%s");
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
        fields=self.__preFields('fields', "set_%name(lua, %value, index);")
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

    def __processUserDefs(self):
        defs = self.ast['userDefs']
        res=""
        if defs != None:
            for x in defs:
                res+="typedef %s %s;\n\t" % (x['type'], x['name'])
        self.__replaceHeader("$$USER_DEFS$$", res)

    def __processFunctionsImpl(self):                            
        #binding
        cname = self.ast['name']
        fs=self.__preFunct('functions', "boost::bind(&"+cname+"::%name, this, lua%, %args)", "_%i")
        num=len(fs)/10
        regs=""
        unregs=""
        for i in range(0,num+1):
            binds = self.__processBinds(fs, i*10,i*10+9)
            regs+="registerClassFunctions<Functions%i, TupleAccessor>(\n\tlua,\n\t" % (i+1)
            regs+=binds + ",\n\t"
            regs+="index, \n\t"
            regs+="getUId() \n\t"
            regs+="); \n\n\t"
            unregs+="unregisterClassFunctions<Functions%i>(getUId());\n\t" % (i+1)
        unregs+="unregisterClassFunctions<MetaFunctions>(getUId());\n\t"
        self.__replaceImpl("$$LUA_REGISTER$$", regs)
        self.__replaceImpl("$$LUA_UNREGISTER$$", unregs)
        
        
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='LuaCpp class builder')
    parser.add_argument("input", help="the input file")
    parser.add_argument("--otype", choices=['header', 'impl', 'doc'], nargs="*")
    args = parser.parse_args()
    f=open(args.input,"r")
    txt=f.read();
    f.close()
    builder=LuaClassBuilder()
    builder.build(txt)
    for x in args.otype:
        if x=="header":
            print builder.header
        if x=="impl":
            print builder.impl
        if x=="doc":
            print builder.doc
        
