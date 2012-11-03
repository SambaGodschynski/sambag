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
  which returns a Ptr object.
  You can register an attributes with an attributeType a tag class and the
  Object Type which have a set( attributeType value, TagClass ) method.
  The convertion from string to the attr. type value will be done by
  the stringstream::>> operator.
  When object created and all attributes setted a "object created"
  boost::signal will be sent.
  
  TODO: make configurable using policy classes:
	- CreatorPolicy
	- SetterPolicy
	- FromStringConvertionPolicy
*/
//=============================================================================
template <class T>
struct SharedCreateCreator {
	typedef boost::shared_ptr<T> PointeeType;
	PointeeType create() {
		return T::create();
	}
	SharedCreateCreator(){}
	template <class U>
	SharedCreateCreator(const SharedCreateCreator<U> &b) {
	}
};
//=============================================================================
template <class T>
struct NewCreator {
	typedef T* PointeeType;
	PointeeType create() {
		return new T();
	}
	NewCreator(){}
	template <class U>
	NewCreator(const NewCreator<U> &b) {
	}
};
//=============================================================================
template <class Closure>
struct SharedWithClosure {
	template <class T>
	struct Creator {
		Closure closure;
		typedef boost::shared_ptr<T> PointeeType;
		PointeeType create() {
			return T::create(&closure);
		}
		Creator(){}
		template <class U>
		Creator(const Creator<U> &b) {
			closure = b.closure;
		}
	};
};
//=============================================================================
template<
	class BaseType, 
	template <class> class CreatorPolicy = SharedCreateCreator
>
class XML2Object : public CreatorPolicy<BaseType>
{
public:
	//-------------------------------------------------------------------------
	typedef typename CreatorPolicy<BaseType>::PointeeType BaseTypePtr;
	//-------------------------------------------------------------------------
	typedef std::string TagName;
	//-------------------------------------------------------------------------
	typedef boost::signal<void(BaseTypePtr, const TagName&)> CreatedSignal;
	//-------------------------------------------------------------------------
	typedef boost::function<void(BaseTypePtr, const TagName&)>
		CreatedSignalFunction;
	//-------------------------------------------------------------------------
	CreatedSignal sObjCreated;
private:
	//#########################################################################
	// Object Creation:
	//-------------------------------------------------------------------------
	struct IObjectFactory {
		virtual BaseTypePtr create() = 0;
	};
	//-------------------------------------------------------------------------
	template<typename ObjectType>
	struct ObjectFactory: public IObjectFactory {
		CreatorPolicy<ObjectType> creator;
		virtual BaseTypePtr create() {
			return creator.create();
		}
	};
	//-------------------------------------------------------------------------
	// relation from tagname to object factory
	typedef boost::unordered_map<TagName, IObjectFactory*> TagMap;
	TagMap tagMap;
	//-------------------------------------------------------------------------
	BaseTypePtr buildObject(const ticpp::Element &el) {
		using namespace boost::algorithm;
		typename TagMap::iterator it = tagMap.find(to_lower_copy(el.Value()));
		if (it == tagMap.end())
			return BaseTypePtr();
		BaseTypePtr obj = it->second->create();
		return obj;
	}
	//#########################################################################
	// Attribute Setter
	//
	//-------------------------------------------------------------------------
	struct IAttributeSetter {
		virtual void set(BaseTypePtr obj,
				const std::string &attrValue) = 0;
	};
	//-------------------------------------------------------------------------
	template<typename AttrType, typename TagClassType, typename ObjectType>
	struct AttributeSetter: public IAttributeSetter {
		virtual void set(BaseTypePtr obj,
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
		virtual void set(BaseTypePtr obj,
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
	void setAttribute(BaseTypePtr obj, const ticpp::Attribute &attr) {
		using namespace boost::algorithm;
		typedef typename AttributeMap::iterator It;
		std::pair<It, It> range = attrMap.equal_range(to_lower_copy( attr.Name() ));
		// try every possible entries (stupid)
		for ( It it=range.first; it!=range.second; ++it) {
			it->second->set(obj, attr.Value());
		}
	}
	//-------------------------------------------------------------------------
	void setAttributes(BaseTypePtr obj, const ticpp::Element &el) {
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
	BaseTypePtr walk(
			const ticpp::Element &element,
			BaseTypePtr givenRoot = BaseTypePtr() )
	{
		// create new object
		BaseTypePtr node = (givenRoot) ? givenRoot : buildObject(element);
		if (!node)
			return BaseTypePtr();
		// iterate through child elements
		ticpp::Iterator < ticpp::Element > child;
		for (child = child.begin(&element); child != child.end(); child++) {
			BaseTypePtr app = walk(*child); // rekusive call
			if (!app)
				continue;
			node->add(app);
		}
		setAttributes(node, element);
		node->setXmlText(element.GetTextOrDefault(""));
		sObjCreated(node, element.Value()); // fire objCreated signal
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
	BaseTypePtr build(
			ticpp::Document &doc,
			BaseTypePtr givenRoot = BaseTypePtr())
	{
		ticpp::Element* pElem = doc.FirstChildElement();
		return walk(*pElem, givenRoot);
	}
public:
	//--------------------------------------------------------------------------
	/**
	 * XML2Object<BaseObject, ClosureType=NO_TYPE>
	 * Closure is a pointer which comes as parameter for the
	 * ObjectType::create() method. If no ClosureType setted this create
	 * will be called without this closure parameter.
	 * @param closure
	 */
	XML2Object() {
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
	const CreatedSignal & getCreatedSignal() const {
		return sObjCreated;
	}
	//-------------------------------------------------------------------------
	boost::signals::connection
	addObjectCreatedSlot(const CreatedSignalFunction &f)
	{
		return sObjCreated.connect(f);
	}
	//-------------------------------------------------------------------------
	// register object type with related tag string
	template<typename ObjectType>
	void registerObject(const std::string &tagName) {
		using namespace boost::algorithm;
		ObjectFactory<ObjectType> *fact = new ObjectFactory<ObjectType> ();
		fact->creator = *this;
		tagMap.insert(std::make_pair(to_lower_copy(tagName), fact));
	}
	//-------------------------------------------------------------------------
	/**
	 * TODO: replace buildWithXmlString and buildWithXmlFile with buildWithXmlStream(istream)
	 *
	 * @param xml:  a valid xml document as string
	 * @return the root object containing a object representation of the xml structure.
	 */
	BaseTypePtr buildWithXmlString(
			const std::string &xml /*default value dosent work with MVSC
									 (complaining about typename) so we use overload*/)
	{
		ticpp::Document doc;
		doc.Parse(xml);
		return build(doc, BaseTypePtr());
	}
	//-------------------------------------------------------------------------
	/**
	 * TODO: replace buildWithXmlString and buildWithXmlFile with buildWithXmlStream(istream)
	 *
	 * @param xml:  a valid xml document as string
	 * @param givenRoot: you are able to use a already created root element.
	 * @return the root object containing a object representation of the xml structure.
	 */
	BaseTypePtr buildWithXmlString(
			const std::string &xml,
			BaseTypePtr givenRoot)
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
	BaseTypePtr buildWithXmlFile(
			const std::string &filename,
			BaseTypePtr givenRoot = BaseTypePtr())
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
