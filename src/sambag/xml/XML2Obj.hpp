#ifndef XML2OBJ_H
#define XML2OBJ_H

#include <boost/unordered_map.hpp>
#include <map>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/function.hpp>
#include <sstream>
#include "ticpp/ticpp.h"

namespace sambag { namespace xml {
//=============================================================================
/** @class XML2Object Parser:
  Builds an object structure using a given XML string.
  You can regsiter a class related to a tag name.
  You have to set a object base class as template argument.
  Every registered object has to be an virtual subobject of this base class.
  Every base class object has to implement an add( BaseClass::Ptr obj ) method.
  Every base class has to define a Ptr typedef which is a shared_ptr type.
  Every object class has to implement a static create()
  respective a create(ClosureType*) method dependent whether a ClosureType is setted.
  Every object class has to implement a setTagName( string txt ) method.
  which returns a Ptr object.
  You can register an attributes with an attributeType a tag class and the
  Object Type which have a set( attributeType value, TagClass ) method.
  The convertion from string to the attr. type value will be done by
  the stringstream::>> operator.
  When object created and all attributes setted a "object created"
  boost::signal will be sent.
*/
//=============================================================================
struct NoClosureType{};
template <typename ObjectType, typename ClosureType>
typename ObjectType::Ptr _create(ObjectType* alwaysNULL, ClosureType*closure) {
	return typename ObjectType::Ptr( ObjectType::create(closure) );
}
//-----------------------------------------------------------------------------
template <typename ObjectType>
typename ObjectType::Ptr _create(ObjectType* alwaysNULL, NoClosureType*) {
	return typename ObjectType::Ptr( ObjectType::create() );
}
//=============================================================================
template<typename BaseType, typename ClosureType=NoClosureType>
class XML2Object {
public:
	//-------------------------------------------------------------------------
	typedef boost::signal<void(typename BaseType::Ptr)> CreatedSignal;
	//-------------------------------------------------------------------------
	typedef boost::function<void(typename BaseType::Ptr)> CreatedSignalFunction;
	//-------------------------------------------------------------------------
	CreatedSignal sObjCreated;
private:
	//-------------------------------------------------------------------------
	ClosureType *closure[1];
	//#########################################################################
	// Object Creation:
	//-------------------------------------------------------------------------
	struct IObjectFactory {
		virtual typename BaseType::Ptr create() = 0;
	};
	//-------------------------------------------------------------------------
	template<typename ObjectType>
	struct ObjectFactory: public IObjectFactory {
		//---------------------------------------------------------------------
		ClosureType **closure;
		//---------------------------------------------------------------------
		virtual typename BaseType::Ptr create() {
			ObjectType* t=NULL; // dummy for function matching
			return _create(t, *closure);
		}
	};
	//-------------------------------------------------------------------------
	// relation from tagname to object factory
	typedef std::string TagName;
	typedef boost::unordered_map<TagName, IObjectFactory*> TagMap;
	TagMap tagMap;
	//-------------------------------------------------------------------------
	typename BaseType::Ptr buildObject(const ticpp::Element &el) {
		using namespace boost::algorithm;
		typename TagMap::iterator it = tagMap.find(to_lower_copy(el.Value()));
		if (it == tagMap.end())
			return typename BaseType::Ptr();
		typename BaseType::Ptr obj = it->second->create();
		return obj;
	}
	//#########################################################################
	// Attribute Setter
	//
	//-------------------------------------------------------------------------
	struct IAttributeSetter {
		virtual void set(typename BaseType::Ptr obj,
				const std::string &attrValue) = 0;
	};
	//-------------------------------------------------------------------------
	template<typename AttrType, typename TagClassType, typename ObjectType>
	struct AttributeSetter: public IAttributeSetter {
		virtual void set(typename BaseType::Ptr obj,
				const std::string &strValue) {
			ObjectType *tObj = dynamic_cast<ObjectType*> (obj.get());
			if (!tObj)
				return;
			std::stringstream ss;
			ss << strValue;
			AttrType value;
			ss >> value;
			tObj->set(value, TagClassType());
		}
	};
	//-------------------------------------------------------------------------
	template<typename TagClassType, typename ObjectType>
	struct AttributeSetter<std::string, TagClassType, ObjectType> : public IAttributeSetter {
		virtual void set(typename BaseType::Ptr obj,
				const std::string &strValue) {
			ObjectType *tObj = dynamic_cast<ObjectType*> (obj.get());
			if (!tObj)
				return;
			tObj->set(strValue, TagClassType());
		}
	};
	//-------------------------------------------------------------------------
	typedef std::string AttributeName;
	typedef std::multimap<AttributeName, IAttributeSetter*> AttributeMap;
	AttributeMap attrMap;
	//-------------------------------------------------------------------------
	void setAttribute(typename BaseType::Ptr obj, const ticpp::Attribute &attr) {
		using namespace boost::algorithm;
		typedef typename AttributeMap::iterator It;
		std::pair<It, It> range = attrMap.equal_range(to_lower_copy( attr.Name() ));
		// try every possible entries (stupid)
		for ( It it=range.first; it!=range.second; ++it) {
			it->second->set(obj, attr.Value());
		}
	}
	//-------------------------------------------------------------------------
	void setAttributes(typename BaseType::Ptr obj, const ticpp::Element &el) {
		ticpp::Iterator < ticpp::Attribute > attribute;
		attribute = attribute.begin(&el);
		for (; attribute != attribute.end(); attribute++) {
			setAttribute(obj, *attribute);
		}
	}
	//#########################################################################
	// building
	//-------------------------------------------------------------------------
	/**
	 *  walks rekursively through xml element structure and creates objects if
	 *  tagnames are registered.
	 * @param element: current xml emelment
	 * @param givenRoot: setted if the root object is already created (opt.)
	 * @return created object ptr
	 */
	typename BaseType::Ptr walk(
			const ticpp::Element &element,
			typename BaseType::Ptr givenRoot = typename BaseType::Ptr() )
	{
		// create new object
		typename BaseType::Ptr node = (givenRoot) ? givenRoot : buildObject(element);
		if (!node)
			return typename BaseType::Ptr();
		// iterate through child elements
		ticpp::Iterator < ticpp::Element > child;
		for (child = child.begin(&element); child != child.end(); child++) {
			typename BaseType::Ptr app = walk(*child); // rekusive call
			if (!app)
				continue;
			node->add(app);
		}
		setAttributes(node, element);
		node->setXmlText(element.GetTextOrDefault(""));
		sObjCreated(node); // fire objCreated signal
		return node;
	}
	//-------------------------------------------------------------------------
	/**
	 * builds object structure. Container has to append ObjectType objects
	 * with a push_back method.
	 * @param doc
	 * @param givenRoot setted if the root object is already created (opt.)
	 * @return root object ptr
	 */
	typename BaseType::Ptr build(
			ticpp::Document &doc,
			typename BaseType::Ptr givenRoot = typename BaseType::Ptr())
	{
		ticpp::Element* pElem = doc.FirstChildElement();
		return walk(*pElem, givenRoot);
	}
public:
	//--------------------------------------------------------------------------
	void setClosure(ClosureType *_closure) {
		closure[0] = _closure;
	}
	//--------------------------------------------------------------------------
	/**
	 * XML2Object<BaseObject, ClosureType=NO_TYPE>
	 * Closure is a pointer which comes as parameter for the
	 * ObjectType::create() method. If no ClosureType setted this create
	 * will be called without this closure parameter.
	 * @param closure
	 */
	XML2Object(ClosureType *closure=NULL) {
		setClosure(closure);
	}
	//--------------------------------------------------------------------------
	const ClosureType * const getClosure() const {
		return *closure;
	}
	//--------------------------------------------------------------------------
	// frees IOBjectFactory objects
	~XML2Object() {
		//release IFactory Objects
		typename TagMap::iterator tagIt = tagMap.begin();
		for (; tagIt != tagMap.end(); ++tagIt) {
			delete tagIt->second;
			tagIt->second = NULL;
		}
		// release IAttributeSetter Objects
		typename AttributeMap::iterator atIt = attrMap.begin();
		for (; atIt != attrMap.end(); ++atIt) {
			delete atIt->second;
			atIt->second = NULL;
		}
	}
	//-------------------------------------------------------------------------
	CreatedSignal & getCreatedSignal() const {
		return sObjCreated;
	}
	//-------------------------------------------------------------------------
	boost::signals::connection addObjectCreatedSlot(CreatedSignalFunction f) {
		return sObjCreated.connect(f);
	}
	//-------------------------------------------------------------------------
	// register object type with related tag string
	template<typename ObjectType>
	void registerObject(const std::string &tagName) {
		using namespace boost::algorithm;
		ObjectFactory<ObjectType> *fact = new ObjectFactory<ObjectType> ();
		fact->closure = closure;
		tagMap.insert(std::make_pair(to_lower_copy(tagName), fact));
	}
	//-------------------------------------------------------------------------
	/**
	 * TODO: replace buildWithXmlString and buildWithXmlFile with buildWithXmlStream(istream)
	 *
	 * @param xml:  a valid xml document as string
	 * @param givenRoot: you are able to use a already created root element.
	 * @return the root object containing a object representation of the xml structure.
	 */
	typename BaseType::Ptr buildWithXmlString(
			const std::string &xml,
			typename BaseType::Ptr givenRoot = typename BaseType::Ptr())
	{
		ticpp::Document doc;
		doc.Parse(xml);
		return build(doc, givenRoot);
	}
	//-------------------------------------------------------------------------
	/**
	 * TODO: replace buildWithXmlString and buildWithXmlFile with buildWithXmlStream(istream)
	 *
	 * @param xml: location of a valid xml document.
	 * @param givenRoot: you are able to use a already created root element.
	 * @return the root object containing a object representation of the xml structure.
	 */
	typename BaseType::Ptr buildWithXmlFile(
			const std::string &filename,
			typename BaseType::Ptr givenRoot = typename BaseType::Ptr())
	{
		ticpp::Document doc(filename);
		doc.LoadFile();
		return build(doc, givenRoot);
	}
	//-------------------------------------------------------------------------
	/**
	 * template: AttrType,
	 *           TagClassType,
	 *           ObjectType
	 * @param attrName
	 */
	template<typename AttrType, typename TagClassType, typename ObjectType>
	void registerAttribute(const std::string &attrName) {
		IAttributeSetter *setter = new AttributeSetter<AttrType, TagClassType,
				ObjectType> ();
		attrMap.insert(std::make_pair(attrName, setter));
	}
};
}
} // namespace
#endif
