#include <cppunit/config/SourcePrefix.h>
#include "XML2ObjTest.hpp"
#include "sambag/xml/XML2Obj.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/function.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include "ticpp/ticpp.h"
#include <boost/foreach.hpp>


const std::string XML01 = "<root>this is it</root>";
const std::string XML02 = std::string("<root id='01'>") +
                                        "<objectA id='02'/>" +
                                      "</root>";
const std::string XML03 = std::string("<root id='01'>") +
                                           "<objectA id='02'>" +
                                             "<objectB id='03' descr='b-type'/>" +
                                           "</objectA>"
                                         "<objectA id='04'/>" +
                                       "</root>";

const std::string XML04 = std::string("<root id='01'>") +
		                                "<objectA id='02'  x='10' />" +
		                                 "<objectB id='03' descr='b-type' x='20'/>" +
		                              "</root>";

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( tests::XML2ObjectTest );
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Attribute Tags
//------------------------------------------------------------------------------
struct Attr_Id {};
//------------------------------------------------------------------------------
struct Attr_Descr{};
//------------------------------------------------------------------------------
struct Attr_X{};
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Object Classes which are registered in the XML2Object Parser
//------------------------------------------------------------------------------
struct BaseObject {
  typedef boost::shared_ptr<BaseObject> Ptr;
  typedef std::list<Ptr> ObjectList;
  ObjectList oL;
  std::string xmlText;
  static int numObjects;
  int id;
  BaseObject() : id(-1) { ++numObjects; }
  virtual std::string getClassName() const { return "BaseObject"; }
  virtual void setXmlText( const std::string & str) { xmlText = str; }
  virtual void set ( int _id, const Attr_Id &tag ) {
    id = _id;
  }
  virtual void add ( BaseObject::Ptr obj ) {
    oL.push_back(obj);
  }
  virtual void add ( BaseObject  *obj ) { // for test newcreator
  }
};
//------------------------------------------------------------------------------
int BaseObject::numObjects = 0;
//----------------------------------------------------------------------------
static bool createdSignalPassed = false;
//------------------------------------------------------------------------------
struct Root : public BaseObject {
  typedef boost::shared_ptr<Root> Ptr;
  typedef std::pair<double, int> Closure;
  Closure closure;
  Root() {}
  static Ptr create(Closure *closure=NULL) {
    Ptr p( new Root() );
    if (closure!=NULL)
    	p->closure = *closure;
    return p;
  }
  virtual std::string getClassName() const { return "root"; }
  void objCreated ( BaseObject::Ptr obj, const std::string &str ) {
    using namespace std;
    createdSignalPassed = true;
  }
};
//------------------------------------------------------------------------------
struct ObjectA : public BaseObject {
  typedef boost::shared_ptr<ObjectA> Ptr;
  int x;
  ObjectA() : x(0) {}
  static Ptr create() {
    return Ptr( new ObjectA() );
  }
  virtual std::string getClassName() const { return "ObjectA"; }
  virtual void set ( int _x, const Attr_X & ) {
	  x = _x;
  }
};
//------------------------------------------------------------------------------
struct ObjectB : public BaseObject {
  typedef boost::shared_ptr<ObjectB> Ptr;
  std::string descr;
  int x;
  ObjectB() : x(0) {}
  static Ptr create() {
    return Ptr( new ObjectB() );
  }
  virtual std::string getClassName() const { return "ObjectB"; }
  virtual void set ( const std::string &_descr, const Attr_Descr& ) {
    descr = _descr;
  }
  virtual void set ( int _x, const Attr_X & ) {
 	  x = _x;
   }
};
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// object and xml structure compare
//----------------------------------------------------------------------------
struct NotEqual {
  std::string what;
  NotEqual( const std::string &what ) : what(what) {}
};
//----------------------------------------------------------------------------
std::string _cmp( BaseObject &obj, ticpp::Element &element ) {
  using namespace boost::algorithm;
  // compare object names
  std::string objName = to_lower_copy( obj.getClassName() );
  std::string xmlName = to_lower_copy( element.Value() );
  trim(objName); trim(xmlName);
  if ( objName!=xmlName ) return objName + "!=" + xmlName;
  // compare attributes
  // id:
  std::string strId = element.GetAttribute("id");
  int eId = strId == "" ? -1 : atoi ( strId.c_str() );
  if ( eId != obj.id )
    return "object id not equal (" + objName + "!=" + xmlName + ")";
  // descr only on ObjectB
  ObjectB *objB = dynamic_cast<ObjectB*>( &obj );
  if (!objB)
    return "ok";
  if ( objB->descr != element.GetAttribute("descr") )
    return "object descr not equal";
  return "ok";
}
//----------------------------------------------------------------------------
// walks rekursively through xml element and object structure and compares.
// throws exception if xml elements and objects not equal.
void walk ( BaseObject::Ptr obj, ticpp::Element &element ) {
  std::string ret = _cmp( *(obj.get()), element );
  if ( ret != "ok" ) 
    throw NotEqual(ret);
  // iterate through child elements
  BaseObject::ObjectList::iterator it = obj->oL.begin();
  // ocurrs error with VS2008 when init: it = child.begin( &element );
  // because the constructor value string is invalid.
  ticpp::Iterator< ticpp::Element > child; 
  child = child.begin( &element );
  while(true) {
    if ( it == obj->oL.end() && child == child.end() ) break;
	if ( it == obj->oL.end() || child == child.end() ) { 
      std::string w = it == obj->oL.end() ? "xml has more elements" : "there are more objects";
      throw NotEqual( "structure not equal: " + w );
    }
    walk( *it, *child );
    it++; child++;
  }
}
//-----------------------------------------------------------------------------
// returns error string when structure not equal otherwise "ok".
std::string compareRekursive ( BaseObject::Ptr obj, ticpp::Element &element ) {
  try {
    walk( obj, element );
  } catch ( const NotEqual &ex ) {
    return ex.what;
  }
  return "ok";
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// tests
namespace tests {
//=============================================================================
void XML2ObjectTest::testConstructor() {
//=============================================================================
  using namespace sambag::xml;
  XML2Object<BaseObject> xml2Obj;
}
//=============================================================================
void XML2ObjectTest::testClosure() {
	using namespace sambag::xml;
	typedef std::pair<double, int> Closure;
	Closure pv(1.0, 10);
	XML2Object<BaseObject, SharedWithClosure<Closure>::Creator > xml2Obj;
	xml2Obj.closure = pv;
	xml2Obj.registerObject<Root>("root");
	BaseObject::Ptr base = xml2Obj.buildWithXmlString("<root/>");
	Root::Ptr root = boost::shared_dynamic_cast<Root>(base);
	CPPUNIT_ASSERT( root );
	CPPUNIT_ASSERT( root->closure == pv );
}
//=============================================================================
void XML2ObjectTest::testNewCreator() {
	using namespace sambag::xml;
	XML2Object<BaseObject, NewCreator> xml2Obj;
	xml2Obj.registerObject<Root>("root");
	BaseObject *base = xml2Obj.buildWithXmlString("<root/>");
	Root *root = dynamic_cast<Root*>(base);
	CPPUNIT_ASSERT( root );
	delete root;
}
//=============================================================================
void XML2ObjectTest::testBuildStructure() {
//=============================================================================
  using namespace sambag::xml;
  XML2Object<BaseObject> xml2Obj;
  // register obj. created slot
  // register attributtes
  xml2Obj.registerAttribute<int, Attr_Id, BaseObject>("id");
  xml2Obj.registerAttribute<std::string, Attr_Descr, ObjectB>("descr");
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>without registered classes
  BaseObject::Ptr ptr = xml2Obj.buildWithXmlString ( XML01 );
  CPPUNIT_ASSERT( !ptr );
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>XML01
  xml2Obj.registerObject<Root>("root");
  Root::Ptr root = Root::create();
  XML2Object<BaseObject>::CreatedSignalFunction f =
		  boost::bind( &Root::objCreated, root.get(), _1, _2 );
  xml2Obj.addObjectCreatedSlot(f);
  ptr = xml2Obj.buildWithXmlString ( XML01, root );
  CPPUNIT_ASSERT( ptr );
  CPPUNIT_ASSERT_EQUAL( std::string("this is it"), ptr->xmlText );
  ticpp::Document doc;
  doc.Parse ( XML01 );
  std::string ret = compareRekursive ( ptr, *( doc.FirstChildElement() ) );
  CPPUNIT_ASSERT_EQUAL( std::string("ok"), ret );
  CPPUNIT_ASSERT(createdSignalPassed);
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>XML02
  xml2Obj.registerObject<ObjectA>("objectA");
  ptr = xml2Obj.buildWithXmlString ( XML02 );
  CPPUNIT_ASSERT( ptr );
  doc = ticpp::Document();
  doc.Parse ( XML02 );
  ret = compareRekursive ( ptr, *doc.FirstChildElement() );
  CPPUNIT_ASSERT_EQUAL( std::string("ok"), ret );
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>XML03
  xml2Obj.registerObject<ObjectB>("objectB");
  ptr = xml2Obj.buildWithXmlString ( XML03 );
  CPPUNIT_ASSERT( ptr );
  doc = ticpp::Document();
  doc.Parse ( XML03 );
  ret = compareRekursive ( ptr, *doc.FirstChildElement() );
  CPPUNIT_ASSERT_EQUAL( std::string("ok"), ret );
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>XML03
  xml2Obj.registerAttribute<int, Attr_X, ObjectA>("x");
  xml2Obj.registerAttribute<int, Attr_X, ObjectB>("x");
  //xml2Obj.registerAttribute<int, Attr_X, ObjectA>("x");
  ptr = xml2Obj.buildWithXmlString ( XML04 );
  CPPUNIT_ASSERT( ptr );
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>assert x
  BOOST_FOREACH( BaseObject::Ptr obj, ptr->oL ) {
	  if (obj->getClassName() == "ObjectA") {
		  ObjectA::Ptr oa = boost::shared_dynamic_cast<ObjectA, BaseObject>(obj);
		  CPPUNIT_ASSERT_EQUAL( 10, oa->x);
	  }
	  if (obj->getClassName() == "ObjectB") {
		  ObjectB::Ptr ob = boost::shared_dynamic_cast<ObjectB, BaseObject>(obj);
	  	  CPPUNIT_ASSERT_EQUAL( 20, ob->x);
	  }
  }
}


} // namespace tests

